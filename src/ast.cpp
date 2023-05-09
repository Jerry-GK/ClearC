#include "codegen.hpp"
#include "util.hpp"

//Namespace containing all classes involved in the construction of Abstract Syntax Tree (ast)
namespace ast {
    //A program is composed of several declarations
    llvm::Value* Program::codegen(CodeGenerator& Gen) {
        for (auto Decl : *(this->_Decls))
            if (Decl)	//We allow empty-declaration which is represented by NULL pointer.
                Decl->codegen(Gen);
        return NULL;
    }

    //Function declaration
    llvm::Value* FuncDecl::codegen(CodeGenerator& Gen) {
        //Set the argument type list. We need to call "GetLLVMType"
        //to change ast::VarType* type to llvm::Type* type
        std::vector<llvm::Type*> ArgTypes;
        bool isVoidArgs = false;
        for (auto ArgType : *(this->_ArgList)) {
            llvm::Type* LLVMType = ArgType->_VarType->GetLLVMType(Gen);
            if (!LLVMType) {
                throw std::logic_error("Define a function " + this->_Name + " using unknown type(s).");
                return NULL;
            }
            //Check if it is a "void" type
            if (LLVMType->isVoidTy())
                isVoidArgs = true;
            //In C, when the function argument type is an array type, we don't pass the entire array.
            //Instead, we just pass a pointer pointing to its elements
            if (LLVMType->isArrayTy())
                LLVMType = LLVMType->getArrayElementType()->getPointerTo();
            ArgTypes.push_back(LLVMType);
        }
        //Throw an exception if #args >= 2 and the function has a "void" argument.
        if (ArgTypes.size() != 1 && isVoidArgs) {
            throw std::logic_error("Function " + this->_Name + " has invalid number of arguments with type \"void\".");
            return NULL;
        }
        //Clear the arg list of the function only has one "void" arg.
        if (isVoidArgs)
            ArgTypes.clear();

        //Get return type
        llvm::Type* RetType = this->_RetType->GetLLVMType(Gen);
        if (RetType->isArrayTy()) {
            throw std::logic_error("Function " + this->_Name + " returns array type, which is not allowed.");
            return NULL;
        }
        //Get function type
        llvm::FunctionType* FuncType = llvm::FunctionType::get(RetType, ArgTypes, this->_ArgList->_VarArg);
        //Create function
        llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::ExternalLinkage, this->_Name, Gen.Module);
        auto ret = Gen.AddFunction(this->_Name, Func);
        if (ret == ADDFUNC_NAMECONFLICT) {
            throw std::logic_error("Naming conflict for function: " + this->_Name);
            return NULL;
        }
        else if (ret == ADDFUNC_ERROR) {
            //unexpected error
            throw std::logic_error("Unexpected error when adding function: " + this->_Name);
            return NULL;
        }

        //function has been declared or defined
        if (Func->getName() != this->_Name) {
            //Delete the one we just made and get the existing one.
            Func->eraseFromParent();
            Func = Gen.Module->getFunction(this->_Name);

            if (!this->_FuncBody)
            {
                throw std::logic_error("Redeclare function \"" + this->_Name + "\" which has been declared or defined.");
                return NULL;
            }
            //redefine
            if (!Func->empty()) {
                throw std::logic_error("Redefine function \"" + this->_Name + "\" which has been defined.");
                return NULL;
            }
            //define the function with conflict arg types with the previous declaration
            if (Func->getFunctionType() != FuncType) {
                throw std::logic_error("Define function \"" + this->_Name + "\" which has conflict arg types with the previous declaration.");
                return NULL;
            }
            //(declare and) define a function which has been declared is allowed, ignore the second declaration
        }

        //If this function has a body, generate its body's code.
        if (this->_FuncBody) {
            //Create a new basic block to start insertion into.
            llvm::BasicBlock* FuncBlock = llvm::BasicBlock::Create(Context, "entry", Func);
            IRBuilder.SetInsertPoint(FuncBlock);
            //Create allocated space for arguments.
            Gen.PushSymbolTable();	//This variable table is only used to store the arguments of the function
            size_t Index = 0;
            for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
                //Create alloca
                auto Alloca = CreateEntryBlockAlloca(Func, this->_ArgList->at(Index)->_Name, ArgTypes[Index]);
                //Assign the value by "store" instruction
                IRBuilder.CreateStore(ArgIter, Alloca);
                //Add to the symbol table
                if (!Gen.AddVariable(this->_ArgList->at(Index)->_Name, Alloca))
                {
                    throw std::logic_error("Naming conflict or redefinition for local variable: " + this->_ArgList->at(Index)->_Name + "in the args list of function " + this->_Name);
                    return NULL;
                }
            }
            //Generate code of the function body
            Gen.SetCurFunction(Func);
            Gen.PushSymbolTable();
            this->_FuncBody->codegen(Gen);
            Gen.PopSymbolTable();
            Gen.SetCurFunction(NULL);
            Gen.PopSymbolTable();	//We need to pop out an extra variable table.
        }
        return NULL;
    }

    //Function body
    llvm::Value* FuncBody::codegen(CodeGenerator& Gen) {
        //Generate the statements in FuncBody, one by one.
        for (auto& Stmt : *(this->_Content))
            //If the current block already has a terminator,
            //i.e. a "return" statement is generated, stop;
            //Otherwise, continue generating.
            if (IRBuilder.GetInsertBlock()->getTerminator())
                break;
            else
                Stmt->codegen(Gen);
        //return statement is needed for all functions, including void functions.
        if (!IRBuilder.GetInsertBlock()->getTerminator()) {
            throw std::logic_error("Function does not have a return statement (void function needs return as well).");
            return NULL;
        }
        return NULL;
    }

    //Variable declaration
    llvm::Value* VarDecl::codegen(CodeGenerator& Gen) {
        //Get the llvm type
        llvm::Type* VarType = this->_VarType->GetLLVMType(Gen);
        if (VarType == NULL) {
            throw std::logic_error("Define variables with unknown type.");
            return NULL;
        }
        if (VarType->isVoidTy()) {
            throw std::logic_error("Cannot define \"void\" variables.");
            return NULL;
        }
        //Create variables one by one.
        for (auto& NewVar : *(this->_VarList)) {
            //Determine whether the declaration is inside a function.
            //If so, create an alloca;
            //Otherwise, create a global variable.
            if (Gen.GetCurrentFunction()) {
                //Create an alloca.
                auto Alloca = CreateEntryBlockAlloca(Gen.GetCurrentFunction(), NewVar->_Name, VarType);
                if (!Gen.AddVariable(NewVar->_Name, Alloca)) {
                    throw std::logic_error("Naming conflict or redefinition for local variable: " + NewVar->_Name);
                    Alloca->eraseFromParent();
                    return NULL;
                }
                //Assign the initial value by "store" instruction.
                if (NewVar->_InitialExpr) {
                    llvm::Value* Initializer = TypeCasting(NewVar->_InitialExpr->codegen(Gen), VarType);
                    if (Initializer == NULL) {
                        throw std::logic_error("Initialize variable " + NewVar->_Name + " with value of conflict type.");
                        return NULL;
                    }
                    IRBuilder.CreateStore(Initializer, Alloca);
                }
                //TODO: llvm::AllocaInst doesn't has the "constant" attribute, so we need to implement it manually.
                //Unfortunately, I haven't worked out a graceful solution, and the only way I can do is to add a "const"
                //label to the corresponding entry in the symbol table.
            }
            else {
                //create a global variable.
                //Create the constant initializer
                llvm::Constant* Initializer = NULL;
                if (NewVar->_InitialExpr) {
                    //Global variable must be initialized (if any) by a constant.

                    //save the insertion point
                    llvm::BasicBlock* SaveBB = IRBuilder.GetInsertBlock();
                    IRBuilder.SetInsertPoint(Gen.GetEmptyBB());

                    llvm::Value* InitialExpr = TypeCasting(NewVar->_InitialExpr->codegen(Gen), VarType);
                    //EmptyBB is used for detect whether the initial value is a constant!
                    if (IRBuilder.GetInsertBlock()->size() != 0) {
                        throw std::logic_error("Initialize global variable " + NewVar->_Name + " with non-constant value.");
                        return NULL;
                    }
                    if (InitialExpr == NULL) {
                        throw std::logic_error("Initialize variable " + NewVar->_Name + " with value of conflict type.");
                        return NULL;
                    }

                    //retore the insertion point
                    IRBuilder.SetInsertPoint(SaveBB);
                    Initializer = (llvm::Constant*)InitialExpr;
                }
                else {
                    //We must create an undef value manually. If no initializer is given,
                    //this global value will be recognized as "extern" by llvm.
                    Initializer = llvm::UndefValue::get(VarType);
                }
                //Create a global variable
                auto Alloca = new llvm::GlobalVariable(
                    *(Gen.Module),
                    VarType,
                    this->_VarType->_isConst,
                    llvm::Function::ExternalLinkage,
                    Initializer,
                    NewVar->_Name
                );
                if (!Gen.AddVariable(NewVar->_Name, Alloca)) {
                    throw std::logic_error("Naming conflict or redefinition for global variable: " + NewVar->_Name);
                    Alloca->eraseFromParent();
                    return NULL;
                }
            }
        }
        return NULL;
    }

    //Type declaration
    llvm::Value* TypeDecl::codegen(CodeGenerator& Gen) {
        //Add an item to the current typedef symbol table
        //If an old value exists (i.e., conflict), raise an error
        llvm::Type* LLVMType;
        //For struct or union types, firstly we just need to get an opaque struct type
        if (this->_VarType->isStructType())
            LLVMType = ((ast::StructType*)this->_VarType)->GenerateLLVMTypeHead(Gen, this->_Alias);
        else
            LLVMType = this->_VarType->GetLLVMType(Gen);
        if (!LLVMType) {
            throw std::logic_error("Typedef " + this->_Alias + " using undefined types.");
            return NULL;
        }
        if (!Gen.AddType(this->_Alias, LLVMType))
            throw std::logic_error("Naming conflict or redefinition for type: " + this->_Alias);
        //For struct or union types, we need to generate its body
        if (this->_VarType->isStructType())
            ((ast::StructType*)this->_VarType)->GenerateLLVMTypeBody(Gen);
        return NULL;
    }

    //Built-in type
    llvm::Type* BuiltInType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        switch (this->_Type) {
        case _Bool: this->_LLVMType = IRBuilder.getInt1Ty(); break;
        case _Short: this->_LLVMType = IRBuilder.getInt16Ty(); break;
        case _Int: this->_LLVMType = IRBuilder.getInt32Ty(); break;
        case _Long: this->_LLVMType = IRBuilder.getInt64Ty(); break;
        case _Char: this->_LLVMType = IRBuilder.getInt8Ty(); break;
        case _Float: this->_LLVMType = IRBuilder.getFloatTy(); break;
        case _Double: this->_LLVMType = IRBuilder.getDoubleTy(); break;
        case _Void: this->_LLVMType = IRBuilder.getVoidTy(); break;
        default: break;
        }
        return this->_LLVMType;
    }

    //Defined type. Use this class when only an identifier is given.
    llvm::Type* DefinedType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        this->_LLVMType = Gen.FindType(this->_Name);
        if (this->_LLVMType == NULL) {
            throw std::logic_error("Undefined type: \"" + this->_Name + "\"");
            return NULL;
        }
        else return this->_LLVMType;
    }

    //Pointer type.
    llvm::Type* PointerType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        llvm::Type* BaseType = this->_BaseType->GetLLVMType(Gen);
        return this->_LLVMType = llvm::PointerType::get(BaseType, 0U);
    }

    //Array type.
    llvm::Type* ArrayType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        llvm::Type* BaseType = this->_BaseType->GetLLVMType(Gen);
        if (BaseType->isVoidTy()) {
            throw std::logic_error("The base type of array cannot be void.");
            return NULL;
        }
        return this->_LLVMType = llvm::ArrayType::get(BaseType, this->_Length);
    }

    //Struct type.
    llvm::Type* StructType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        //Create an anonymous identified struct type
        this->GenerateLLVMTypeHead(Gen);
        return this->GenerateLLVMTypeBody(Gen);
    }
    llvm::Type* StructType::GenerateLLVMTypeHead(CodeGenerator& Gen, const std::string& __Name) {
        //Firstly, generate an empty identified struct type
        auto LLVMType = llvm::StructType::create(Context, "struct." + __Name);
        //Add to the struct table
        Gen.AddStructType(LLVMType, this);
        return this->_LLVMType = LLVMType;
    }
    llvm::Type* StructType::GenerateLLVMTypeBody(CodeGenerator& Gen) {
        //Secondly, generate its body
        std::vector<llvm::Type*> Members;
        for (auto FDecl : *(this->_StructBody))
            if (FDecl->_VarType->GetLLVMType(Gen)->isVoidTy()) {
                throw std::logic_error("The member type of struct cannot be void.");
                return NULL;
            }
            else
                Members.insert(Members.end(), FDecl->_MemList->size(), FDecl->_VarType->GetLLVMType(Gen));
        ((llvm::StructType*)this->_LLVMType)->setBody(Members);
        return this->_LLVMType;
    }
    size_t StructType::GetElementIndex(const std::string& __MemName) {
        size_t Index = 0;
        for (auto FDecl : *(this->_StructBody))
            for (auto& MemName : *(FDecl->_MemList))
                if (MemName == __MemName)
                    return Index;
                else Index++;
        return -1;
    }

    //Statement block
    llvm::Value* Block::codegen(CodeGenerator& Gen) {
        Gen.PushSymbolTable();
        //Generate the statements in Block, one by one.
        for (auto& Stmt : *(this->_Content))
            //If the current block already has a terminator,
            //i.e. a "break" statement is generated, stop;
            //Otherwise, continue generating.
            if (IRBuilder.GetInsertBlock()->getTerminator())
                break;
            else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
                Stmt->codegen(Gen);
        Gen.PopSymbolTable();
        return NULL;
    }

    //If statement
    llvm::Value* IfStmt::codegen(CodeGenerator& Gen) {
        //Evaluate condition
        //Since we don't allow variable declarations in if-condition (because we only allow expressions there),
        //we don't need to push a symbol table
        llvm::Value* Condition = this->_Condition->codegen(Gen);
        //Cast the type to i1
        if (!(Condition = Cast2I1(Condition))) {
            throw std::logic_error("The condition value of if-statement must be either an integer, or a floating-point number, or a pointer.");
            return NULL;
        }
        //Create "Then", "Else" and "Merge" block
        llvm::Function* CurrentFunc = Gen.GetCurrentFunction();
        llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(Context, "Then");
        llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(Context, "Else");
        llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(Context, "Merge");
        //Create a branch instruction corresponding to this if statement
        IRBuilder.CreateCondBr(Condition, ThenBB, ElseBB);
        //Generate code in the "Then" block
        CurrentFunc->getBasicBlockList().push_back(ThenBB);

        IRBuilder.SetInsertPoint(ThenBB);
        if (this->_Then) {
            Gen.PushSymbolTable();
            this->_Then->codegen(Gen);
            Gen.PopSymbolTable();
        }
        TerminateBlockByBr(MergeBB);
        //Generate code in the "Else" block
        CurrentFunc->getBasicBlockList().push_back(ElseBB);
        IRBuilder.SetInsertPoint(ElseBB);
        if (this->_Else) {
            Gen.PushSymbolTable();
            this->_Else->codegen(Gen);
            Gen.PopSymbolTable();
        }
        TerminateBlockByBr(MergeBB);
        //Finish "Merge" block
        if (MergeBB->hasNPredecessorsOrMore(1)) {
            CurrentFunc->getBasicBlockList().push_back(MergeBB);
            IRBuilder.SetInsertPoint(MergeBB);
        }
        return NULL;
    }

    //For statement
    llvm::Value* ForStmt::codegen(CodeGenerator& Gen) {
        //Create "ForCond", "ForLoop", "ForTail" and "ForEnd"
        llvm::Function* CurrentFunc = Gen.GetCurrentFunction();
        llvm::BasicBlock* ForCondBB = llvm::BasicBlock::Create(Context, "ForCond");
        llvm::BasicBlock* ForLoopBB = llvm::BasicBlock::Create(Context, "ForLoop");
        llvm::BasicBlock* ForTailBB = llvm::BasicBlock::Create(Context, "ForTail");
        llvm::BasicBlock* ForEndBB = llvm::BasicBlock::Create(Context, "ForEnd");
        //Evaluate the initial statement, and create an unconditional branch to "ForCond" block
        //Since we allow variable declarations here, we need to push a new symbol table
        //e.g., for (int i = 0; ...; ...) { ... }
        if (this->_Initial) {
            Gen.PushSymbolTable();
            this->_Initial->codegen(Gen);
        }
        TerminateBlockByBr(ForCondBB);
        //Generate code in the "ForCond" block
        CurrentFunc->getBasicBlockList().push_back(ForCondBB);
        IRBuilder.SetInsertPoint(ForCondBB);
        if (this->_Condition) {
            //If it has a loop condition, evaluate it (cast the type to i1 if necessary).
            llvm::Value* Condition = this->_Condition->codegen(Gen);
            if (!(Condition = Cast2I1(Condition))) {
                throw std::logic_error("The condition value of for-statement must be either an integer, or a floating-point number, or a pointer.");
                return NULL;
            }
            IRBuilder.CreateCondBr(Condition, ForLoopBB, ForEndBB);
        }
        else {
            //Otherwise, it is an unconditional loop condition (always returns true)
            IRBuilder.CreateBr(ForLoopBB);
        }
        //Generate code in the "ForLoop" block
        CurrentFunc->getBasicBlockList().push_back(ForLoopBB);
        IRBuilder.SetInsertPoint(ForLoopBB);
        if (this->_LoopBody) {
            Gen.EnterLoop(ForTailBB, ForEndBB);		//Don't forget to call "EnterLoop"
            Gen.PushSymbolTable();
            this->_LoopBody->codegen(Gen);
            Gen.PopSymbolTable();
            Gen.LeaveLoop();						//Don't forget to call "LeaveLoop"
        }
        //If not terminated, jump to "ForTail"
        TerminateBlockByBr(ForTailBB);
        //Generate code in the "ForTail" block
        CurrentFunc->getBasicBlockList().push_back(ForTailBB);
        IRBuilder.SetInsertPoint(ForTailBB);
        if (this->_Tail)
            this->_Tail->codegen(Gen);
        IRBuilder.CreateBr(ForCondBB);
        //Finish "ForEnd" block
        CurrentFunc->getBasicBlockList().push_back(ForEndBB);
        IRBuilder.SetInsertPoint(ForEndBB);
        if (this->_Initial) {
            Gen.PopSymbolTable();
        }
        return NULL;
    }

    //Switch statement
    llvm::Value* SwitchStmt::codegen(CodeGenerator& Gen) {
        llvm::Function* CurrentFunc = Gen.GetCurrentFunction();
        //Evaluate condition
        //Since we don't allow variable declarations in switch-matcher (because we only allow expressions there),
        //we don't need to push a symbol table.
        llvm::Value* Matcher = this->_Matcher->codegen(Gen);
        //Create one block for each case statement.
        std::vector<llvm::BasicBlock*> CaseBB;
        for (int i = 0; i < this->_CaseList->size(); i++)
            CaseBB.push_back(llvm::BasicBlock::Create(Context, "Case" + std::to_string(i)));
        //Create an extra block for SwitchEnd
        CaseBB.push_back(llvm::BasicBlock::Create(Context, "SwitchEnd"));
        //Create one block for each comparison.
        std::vector<llvm::BasicBlock*> ComparisonBB;
        ComparisonBB.push_back(IRBuilder.GetInsertBlock());
        for (int i = 1; i < this->_CaseList->size(); i++)
            ComparisonBB.push_back(llvm::BasicBlock::Create(Context, "Comparison" + std::to_string(i)));
        ComparisonBB.push_back(CaseBB.back());
        //Generate branches
        for (int i = 0; i < this->_CaseList->size(); i++) {
            if (i) {
                CurrentFunc->getBasicBlockList().push_back(ComparisonBB[i]);
                IRBuilder.SetInsertPoint(ComparisonBB[i]);
            }
            if (this->_CaseList->at(i)->_Condition)	//Have condition
                IRBuilder.CreateCondBr(
                    CreateCmpEQ(Matcher, this->_CaseList->at(i)->_Condition->codegen(Gen)),
                    CaseBB[i],
                    ComparisonBB[i + 1]
                );
            else									//Default
                IRBuilder.CreateBr(CaseBB[i]);
        }
        //Generate code for each case statement
        Gen.PushSymbolTable();
        for (int i = 0; i < this->_CaseList->size(); i++) {
            CurrentFunc->getBasicBlockList().push_back(CaseBB[i]);
            IRBuilder.SetInsertPoint(CaseBB[i]);
            Gen.EnterLoop(CaseBB[i + 1], CaseBB.back());
            this->_CaseList->at(i)->codegen(Gen);
            Gen.LeaveLoop();
        }
        Gen.PopSymbolTable();
        //Finish "SwitchEnd" block
        if (CaseBB.back()->hasNPredecessorsOrMore(1)) {
            CurrentFunc->getBasicBlockList().push_back(CaseBB.back());
            IRBuilder.SetInsertPoint(CaseBB.back());
        }
        return NULL;
    }

    //Case statement in switch statement
    llvm::Value* CaseStmt::codegen(CodeGenerator& Gen) {
        //Generate the statements, one by one.
        for (auto& Stmt : *(this->_Content))
            //If the current block already has a terminator,
            //i.e. a "break" statement is generated, stop;
            //Otherwise, continue generating.
            if (IRBuilder.GetInsertBlock()->getTerminator())
                break;
            else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
                Stmt->codegen(Gen);
        //If not terminated, jump to the next case block
        TerminateBlockByBr(Gen.GetContinueBlock());
        return NULL;
    }

    //Continue statement
    llvm::Value* ContinueStmt::codegen(CodeGenerator& Gen) {
        llvm::BasicBlock* ContinueTarget = Gen.GetContinueBlock();
        if (ContinueTarget)
            IRBuilder.CreateBr(ContinueTarget);
        else
            throw std::logic_error("Continue statement should only be used in loops or switch statements.");
        return NULL;
    }

    //Break statement
    llvm::Value* BreakStmt::codegen(CodeGenerator& Gen) {
        llvm::BasicBlock* BreakTarget = Gen.GetBreakBlock();
        if (BreakTarget)
            IRBuilder.CreateBr(BreakTarget);
        else
            throw std::logic_error("Break statement should only be used in loops or switch statements.");
        return NULL;
    }

    //Return statement
    llvm::Value* ReturnStmt::codegen(CodeGenerator& Gen) {
        llvm::Function* Func = Gen.GetCurrentFunction();
        if (!Func) {
            throw std::logic_error("Return statement should only be used in function bodies.");
            return NULL;
        }
        if (this->_RetVal == NULL) {
            if (Func->getReturnType()->isVoidTy())
                IRBuilder.CreateRetVoid();
            else {
                throw std::logic_error("Expected an expression after return statement.");
                return NULL;
            }
        }
        else {
            llvm::Value* RetVal = TypeCasting(this->_RetVal->codegen(Gen), Func->getReturnType());
            if (!RetVal) {
                throw std::logic_error("The type of return value doesn't match and cannot be cast to the return type.");
                return NULL;
            }
            IRBuilder.CreateRet(RetVal);
        }
    }

    //Subscript, e.g. a[10], b[2][3]
    llvm::Value* Subscript::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* Subscript::codegenPtr(CodeGenerator& Gen) {
        //Get the pointer
        llvm::Value* ArrayPtr = this->_Array->codegen(Gen);
        if (!ArrayPtr->getType()->isPointerTy()) {
            throw std::logic_error("Subscript operator \"[]\" must be applied to pointers or arrays.");
            return NULL;
        }
        //Get the index value
        llvm::Value* Subspt = this->_Index->codegen(Gen);
        if (!(Subspt->getType()->isIntegerTy())) {
            throw std::logic_error("Subscription should be an integer.");
            return NULL;
        }
        //Return pointer addition
        return CreateAdd(ArrayPtr, Subspt, Gen);
    }

    //Operator sizeof() in C
    llvm::Value* SizeOf::codegen(CodeGenerator& Gen) {
        if (this->_Arg1)//Expression
            return IRBuilder.getInt64(Gen.GetTypeSize(this->_Arg1->codegen(Gen)->getType()));
        else if (this->_Arg2)//VarType
            return IRBuilder.getInt64(Gen.GetTypeSize(this->_Arg2->GetLLVMType(Gen)));
        else {//Single identifier
            llvm::Type* Type = Gen.FindType(this->_Arg3);
            if (Type) {
                this->_Arg2 = new DefinedType(this->_Arg3);
                return IRBuilder.getInt64(Gen.GetTypeSize(Type));
            }
            llvm::Value* Var = Gen.FindVariable(this->_Arg3);
            if (Var) {
                this->_Arg1 = new Variable(this->_Arg3);
                return IRBuilder.getInt64(Gen.GetTypeSize(Var->getType()->getNonOpaquePointerElementType()));
            }
            throw std::logic_error(this->_Arg3 + " is neither a type nor a variable.");
            return NULL;
        }
    }
    llvm::Value* SizeOf::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Sizeof() only returns right-values.");
        return NULL;
    }

    //Function call
    llvm::Value* FunctionCall::codegen(CodeGenerator& Gen) {
        //Get the function. Throw exception if the function doesn't exist.
        llvm::Function* Func = Gen.FindFunction(this->_FuncName);
        if (Func == NULL) {
            throw std::domain_error(this->_FuncName + " is not a defined function.");
            return NULL;
        }
        //Check the number of args. If Func took a different number of args, reject.
        if (Func->isVarArg() && this->_ArgList->size() < Func->arg_size() ||
            !Func->isVarArg() && this->_ArgList->size() != Func->arg_size()) {
            throw std::invalid_argument("Args doesn't match when calling function " + this->_FuncName + ". Expected " + std::to_string(Func->arg_size()) + ", got " + std::to_string(this->_ArgList->size()));
            return NULL;
        }
        //Check arg types. If Func took different different arg types, reject.
        std::vector<llvm::Value*> ArgList;
        size_t Index = 0;
        for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
            llvm::Value* Arg = this->_ArgList->at(Index)->codegen(Gen);
            Arg = TypeCasting(Arg, ArgIter->getType());
            if (Arg == NULL) {
                throw std::invalid_argument(std::to_string(Index) + "-th arg type doesn't match when calling function " + this->_FuncName + ".");
                return NULL;
            }
            ArgList.push_back(Arg);
        }
        //Continue to push arguments if this function takes a variable number of arguments
        //According to the C standard, bool/char/short should be extended to int, and float should be extended to double
        if (Func->isVarArg())
            for (; Index < this->_ArgList->size(); Index++) {
                llvm::Value* Arg = this->_ArgList->at(Index)->codegen(Gen);
                if (Arg->getType()->isIntegerTy())
                    Arg = TypeUpgrading(Arg, IRBuilder.getInt32Ty());
                else if (Arg->getType()->isFloatingPointTy())
                    Arg = TypeUpgrading(Arg, IRBuilder.getDoubleTy());
                ArgList.push_back(Arg);
            }
        //Create function call.
        return IRBuilder.CreateCall(Func, ArgList);
    }
    llvm::Value* FunctionCall::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Function call only returns right-values.");
        return NULL;
    }

    //Structure reference, e.g. a.x, a.y
    llvm::Value* StructReference::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* StructReference::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* StructPtr = this->_Struct->codegenPtr(Gen);
        if (!StructPtr->getType()->isPointerTy() || !StructPtr->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Reference operator \".\" must be apply to structs.");
            return NULL;
        }
        //Since C language uses name instead of index to fetch the element inside a struct,
        //we need to fetch the ast::StructType* instance according to the llvm::StructType* instance.
        //And it is the same with union types.
        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
                return NULL;
            }
            std::vector<llvm::Value*> Indices;
            Indices.push_back(IRBuilder.getInt32(0));
            Indices.push_back(IRBuilder.getInt32(MemIndex));
            return IRBuilder.CreateGEP(StructPtr->getType()->getNonOpaquePointerElementType(), StructPtr, Indices);
        }
        return NULL;
    }

    //Structure dereference, e.g. a->x, a->y
    llvm::Value* StructDereference::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* StructDereference::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* StructPtr = this->_StructPtr->codegen(Gen);
        if (!StructPtr->getType()->isPointerTy() || !StructPtr->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Dereference operator \"->\" must be apply to struct pointers.");
            return NULL;
        }
        //Since C language uses name instead of index to fetch the element inside a struct,
        //we need to fetch the ast::StructType* instance according to the llvm::StructType* instance.
        //And it is the same with union types.
        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
                return NULL;
            }
            std::vector<llvm::Value*> Indices;
            Indices.push_back(IRBuilder.getInt32(0));
            Indices.push_back(IRBuilder.getInt32(MemIndex));
            return IRBuilder.CreateGEP(StructPtr->getType()->getNonOpaquePointerElementType(), StructPtr, Indices);
        }
        return NULL;
    }

    //Unary plus, e.g. +i, +j, +123
    llvm::Value* UnaryPlus::codegen(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand->getType()->isIntegerTy() ||
            Operand->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary plus must be applied to integers or floating-point numbers.");
        return Operand;
    }
    llvm::Value* UnaryPlus::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Unary plus only returns right-values.");
        return NULL;
    }

    //Unary minus, e.g. -i, -j, -123
    llvm::Value* UnaryMinus::codegen(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand->getType()->isIntegerTy() ||
            Operand->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary minus must be applied to integers or floating-point numbers.");
        if (Operand->getType()->isIntegerTy())
            return IRBuilder.CreateNeg(Operand);
        else
            return IRBuilder.CreateFNeg(Operand);
    }
    llvm::Value* UnaryMinus::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Unary minus only returns right-values.");
        return NULL;
    }

    //Type cast, e.g. (float)n, (int)1.0
    llvm::Value* TypeCast::codegen(CodeGenerator& Gen) {
        llvm::Value* Ret = TypeCasting(this->_Operand->codegen(Gen), this->_VarType->GetLLVMType(Gen));
        if (Ret == NULL) {
            throw std::logic_error("Unable to do type casting.");
            return NULL;
        }
        return Ret;
    }
    llvm::Value* TypeCast::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Type casting only returns right-values.");
        return NULL;
    }

    //Prefix increment, e.g. ++i
    llvm::Value* PrefixInc::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* PrefixInc::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegenPtr(Gen);
        llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
        if (!(
            OpValue->getType()->isIntegerTy() ||
            OpValue->getType()->isFloatingPointTy() ||
            OpValue->getType()->isPointerTy())
            )
            throw std::logic_error("Prefix increment must be applied to integers, floating-point numbers or pointers.");
        llvm::Value* OpValuePlus = CreateAdd(OpValue, IRBuilder.getInt1(1), Gen);
        IRBuilder.CreateStore(OpValuePlus, Operand);
        return Operand;
    }

    //Postfix increment, e.g. i++
    llvm::Value* PostfixInc::codegen(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegenPtr(Gen);
        llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
        if (!(
            OpValue->getType()->isIntegerTy() ||
            OpValue->getType()->isFloatingPointTy() ||
            OpValue->getType()->isPointerTy())
            )
            throw std::logic_error("Postfix increment must be applied to integers, floating-point numbers or pointers.");
        llvm::Value* OpValuePlus = CreateAdd(OpValue, IRBuilder.getInt1(1), Gen);
        IRBuilder.CreateStore(OpValuePlus, Operand);
        return OpValue;
    }
    llvm::Value* PostfixInc::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Postfix increment only returns right-values.");
        return NULL;
    }

    //Prefix decrement, e.g. --i
    llvm::Value* PrefixDec::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* PrefixDec::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegenPtr(Gen);
        llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
        if (!(
            OpValue->getType()->isIntegerTy() ||
            OpValue->getType()->isFloatingPointTy() ||
            OpValue->getType()->isPointerTy())
            )
            throw std::logic_error("Prefix decrement must be applied to integers, floating-point numbers or pointers.");
        llvm::Value* OpValueMinus = CreateSub(OpValue, IRBuilder.getInt1(1), Gen);
        IRBuilder.CreateStore(OpValueMinus, Operand);
        return Operand;
    }

    //Postfix decrement, e.g. i--
    llvm::Value* PostfixDec::codegen(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegenPtr(Gen);
        llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
        if (!(
            OpValue->getType()->isIntegerTy() ||
            OpValue->getType()->isFloatingPointTy() ||
            OpValue->getType()->isPointerTy())
            )
            throw std::logic_error("Postfix decrement must be applied to integers, floating-point numbers or pointers.");
        llvm::Value* OpValueMinus = CreateSub(OpValue, IRBuilder.getInt1(1), Gen);
        IRBuilder.CreateStore(OpValueMinus, Operand);
        return OpValue;
    }
    llvm::Value* PostfixDec::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Postfix decrement only returns right-values.");
        return NULL;
    }

    //Indirection, e.g. *ptr
    llvm::Value* Indirection::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* Indirection::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* Ptr = this->_Operand->codegen(Gen);
        if (!Ptr->getType()->isPointerTy()) {
            throw std::logic_error("Indirection operator \"dptr()\" only applies on pointers or arrays.");
            return NULL;
        }
        return Ptr;
    }

    //Fetch address, e.g. &i
    llvm::Value* AddressOf::codegen(CodeGenerator& Gen) {
        return this->_Operand->codegenPtr(Gen);
    }
    llvm::Value* AddressOf::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Address operator \"addr()\" only returns right-values.");
        return NULL;
    }

    //Logic NOT, e.g. !x
    llvm::Value* LogicNot::codegen(CodeGenerator& Gen) {
        return IRBuilder.CreateICmpEQ(Cast2I1(this->_Operand->codegen(Gen)), IRBuilder.getInt1(false));
    }
    llvm::Value* LogicNot::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic NOT operator \"!\" only returns right-values.");
        return NULL;
    }

    //Bitwise NOT, e.g. ~x
    llvm::Value* BitwiseNot::codegen(CodeGenerator& Gen) {
        llvm::Value* Operand = this->_Operand->codegen(Gen);
        if (!Operand->getType()->isIntegerTy()) {
            throw std::logic_error("Bitwise NOT operator \"~\" must be applied to integers.");
            return NULL;
        }
        return IRBuilder.CreateNot(Operand);
    }
    llvm::Value* BitwiseNot::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise NOT operator \"~\" only returns right-values.");
        return NULL;
    }

    //Division, e.g. x/y
    llvm::Value* Division::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateDiv(LHS, RHS, Gen);
    }
    llvm::Value* Division::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Division operator \"/\" only returns right-values.");
        return NULL;
    }

    //Multiplication, e.g. x*y
    llvm::Value* Multiplication::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateMul(LHS, RHS, Gen);
    }
    llvm::Value* Multiplication::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Multiplication operator \"*\" only returns right-values.");
        return NULL;
    }

    //Modulo, e.g. x%y
    llvm::Value* Modulo::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateMod(LHS, RHS, Gen);
    }
    llvm::Value* Modulo::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Modulo operator \"%\" only returns right-values.");
        return NULL;
    }

    //Addition, e.g. x+y
    llvm::Value* Addition::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAdd(LHS, RHS, Gen);
    }
    llvm::Value* Addition::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Addition operator \"+\" only returns right-values.");
        return NULL;
    }

    //Subtraction, e.g. x-y
    llvm::Value* Subtraction::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateSub(LHS, RHS, Gen);
    }
    llvm::Value* Subtraction::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Subtraction operator \"-\" only returns right-values.");
        return NULL;
    }

    //LeftShift, e.g. x<<y
    llvm::Value* LeftShift::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateShl(LHS, RHS, Gen);
    }
    llvm::Value* LeftShift::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Left shifting operator \"<<\" only returns right-values.");
        return NULL;
    }

    //RightShift, e.g. x>>y
    llvm::Value* RightShift::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateShr(LHS, RHS, Gen);
    }
    llvm::Value* RightShift::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Right shifting operator \">>\" only returns right-values.");
        return NULL;
    }

    //LogicGT, e.g. x>y
    llvm::Value* LogicGT::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS, RHS)) {
            if (LHS->getType()->isIntegerTy())
                return IRBuilder.CreateICmpSGT(LHS, RHS);
            else
                return IRBuilder.CreateFCmpOGT(LHS, RHS);
        }
        //Pointer compare
        if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
            return IRBuilder.CreateICmpUGT(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
            return IRBuilder.CreateICmpUGT(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
            return IRBuilder.CreateICmpUGT(
                TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        throw std::domain_error("Comparison \">\" using unsupported type combination.");
    }
    llvm::Value* LogicGT::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \">\" only returns right-values.");
        return NULL;
    }

    //LogicGE, e.g. x>=y
    llvm::Value* LogicGE::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS, RHS)) {
            if (LHS->getType()->isIntegerTy())
                return IRBuilder.CreateICmpSGE(LHS, RHS);
            else
                return IRBuilder.CreateFCmpOGE(LHS, RHS);
        }
        //Pointer compare
        if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
            return IRBuilder.CreateICmpUGE(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
            return IRBuilder.CreateICmpUGE(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
            return IRBuilder.CreateICmpUGE(
                TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        throw std::domain_error("Comparison \">=\" using unsupported type combination.");
    }
    llvm::Value* LogicGE::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \">=\" only returns right-values.");
        return NULL;
    }

    //LogicLT, e.g. x<y
    llvm::Value* LogicLT::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS, RHS)) {
            if (LHS->getType()->isIntegerTy())
                return IRBuilder.CreateICmpSLT(LHS, RHS);
            else
                return IRBuilder.CreateFCmpOLT(LHS, RHS);
        }
        //Pointer compare
        if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
            return IRBuilder.CreateICmpULT(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
            return IRBuilder.CreateICmpULT(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
            return IRBuilder.CreateICmpULT(
                TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        throw std::domain_error("Comparison \"<\" using unsupported type combination.");
    }
    llvm::Value* LogicLT::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"<\" only returns right-values.");
        return NULL;
    }

    //LogicLE, e.g. x<=y
    llvm::Value* LogicLE::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS, RHS)) {
            if (LHS->getType()->isIntegerTy())
                return IRBuilder.CreateICmpSLE(LHS, RHS);
            else
                return IRBuilder.CreateFCmpOLE(LHS, RHS);
        }
        //Pointer compare
        if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
            return IRBuilder.CreateICmpULE(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
            return IRBuilder.CreateICmpULE(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
            return IRBuilder.CreateICmpULE(
                TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        throw std::domain_error("Comparison \"<=\" using unsupported type combination.");
    }
    llvm::Value* LogicLE::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"<=\" only returns right-values.");
        return NULL;
    }

    //LogicEQ, e.g. x==y
    llvm::Value* LogicEQ::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateCmpEQ(LHS, RHS);
    }
    llvm::Value* LogicEQ::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"==\" only returns right-values.");
        return NULL;
    }

    //LogicNEQ, e.g. x!=y
    llvm::Value* LogicNEQ::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS, RHS)) {
            if (LHS->getType()->isIntegerTy())
                return IRBuilder.CreateICmpNE(LHS, RHS);
            else
                return IRBuilder.CreateFCmpONE(LHS, RHS);
        }
        //Pointer compare
        if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
            return IRBuilder.CreateICmpNE(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
            return IRBuilder.CreateICmpNE(
                IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS, IRBuilder.getInt64Ty())
            );
        }
        else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
            return IRBuilder.CreateICmpNE(
                TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
            );
        }
        throw std::domain_error("Comparison \"!=\" using unsupported type combination.");
    }
    llvm::Value* LogicNEQ::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"!=\" only returns right-values.");
        return NULL;
    }

    //BitwiseAND, e.g. x&y
    llvm::Value* BitwiseAND::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateBitwiseAND(LHS, RHS, Gen);
    }
    llvm::Value* BitwiseAND::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise AND operator \"&\" only returns right-values.");
        return NULL;
    }

    //BitwiseXOR, e.g. x^y
    llvm::Value* BitwiseXOR::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateBitwiseXOR(LHS, RHS, Gen);
    }
    llvm::Value* BitwiseXOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise XOR operator \"^\" only returns right-values.");
        return NULL;
    }

    //BitwiseOR, e.g. x|y
    llvm::Value* BitwiseOR::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateBitwiseOR(LHS, RHS, Gen);
    }
    llvm::Value* BitwiseOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise OR operator \"|\" only returns right-values.");
        return NULL;
    }

    //LogicAND, e.g. x&&y
    llvm::Value* LogicAND::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        LHS = Cast2I1(LHS);
        if (LHS == NULL) {
            throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
            return NULL;
        }
        RHS = Cast2I1(RHS);
        if (RHS == NULL) {
            throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
            return NULL;
        }
        return IRBuilder.CreateLogicalAnd(LHS, RHS);
    }
    llvm::Value* LogicAND::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic AND operator \"&&\" only returns right-values.");
        return NULL;
    }

    //LogicOR, e.g. x||y
    llvm::Value* LogicOR::codegen(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegen(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        LHS = Cast2I1(LHS);
        if (LHS == NULL) {
            throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
            return NULL;
        }
        RHS = Cast2I1(RHS);
        if (RHS == NULL) {
            throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
            return NULL;
        }
        return IRBuilder.CreateLogicalOr(LHS, RHS);
    }
    llvm::Value* LogicOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic OR operator \"||\" only returns right-values.");
        return NULL;
    }

    //TernaryCondition, e.g. (cond)?x:y
    llvm::Value* TernaryCondition::codegen(CodeGenerator& Gen) {
        llvm::Value* Condition = Cast2I1(this->_Condition->codegen(Gen));
        if (Condition == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
            return NULL;
        }
        llvm::Value* True = this->_Then->codegen(Gen);
        llvm::Value* False = this->_Else->codegen(Gen);
        if (True->getType() == False->getType() || TypeUpgrading(True, False)) {
            return IRBuilder.CreateSelect(Condition, True, False);
        }
        else {
            throw std::domain_error("Thernary operand \" ? : \" using unsupported type combination.");
            return NULL;
        }
    }
    llvm::Value* TernaryCondition::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* Condition = Cast2I1(this->_Condition->codegen(Gen));
        if (Condition == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
            return NULL;
        }
        llvm::Value* True = this->_Then->codegenPtr(Gen);
        llvm::Value* False = this->_Else->codegenPtr(Gen);
        if (True->getType() != False->getType()) {
            throw std::domain_error("When using thernary expressions \" ? : \" as left-values, the latter two operands must be of the same type.");
            return NULL;
        }
        return IRBuilder.CreateSelect(Condition, True, False);
    }

    //DirectAssign, e.g. x=y
    llvm::Value* DirectAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* DirectAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS, RHS, Gen);
    }

    //DivAssign, e.g. x/=y
    llvm::Value* DivAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* DivAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateDiv(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //MulAssign, e.g. x*=y
    llvm::Value* MulAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* MulAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateMul(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //ModAssign, e.g. x%=y
    llvm::Value* ModAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* ModAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateMod(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //AddAssign, e.g. x+=y
    llvm::Value* AddAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* AddAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateAdd(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //SubAssign, e.g. x-=y
    llvm::Value* SubAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* SubAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateSub(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //SHLAssign, e.g. x<<=y
    llvm::Value* SHLAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* SHLAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateShl(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //SHRAssign, e.g. x>>=y
    llvm::Value* SHRAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* SHRAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateShr(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //BitwiseANDAssign, e.g. x&=y
    llvm::Value* BitwiseANDAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* BitwiseANDAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateBitwiseAND(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //BitwiseXORAssign, e.g. x^=y
    llvm::Value* BitwiseXORAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* BitwiseXORAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateBitwiseXOR(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //BitwiseORAssign, e.g. x|=y
    llvm::Value* BitwiseORAssign::codegen(CodeGenerator& Gen) {
        return CreateLoad(this->codegenPtr(Gen), Gen);
    }
    llvm::Value* BitwiseORAssign::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* LHS = this->_LHS->codegenPtr(Gen);
        llvm::Value* RHS = this->_RHS->codegen(Gen);
        return CreateAssignment(LHS,
            CreateBitwiseOR(
                IRBuilder.CreateLoad(
                    LHS->getType()->getNonOpaquePointerElementType(),
                    LHS),
                RHS,
                Gen),
            Gen
        );
    }

    //Comma expression, e.g. a,b,c,1
    llvm::Value* CommaExpr::codegen(CodeGenerator& Gen) {
        this->_LHS->codegen(Gen);
        return this->_RHS->codegen(Gen);
    }
    llvm::Value* CommaExpr::codegenPtr(CodeGenerator& Gen) {
        this->_LHS->codegen(Gen);
        return this->_RHS->codegenPtr(Gen);
    }

    //Variable, e.g. x
    llvm::Value* Variable::codegen(CodeGenerator& Gen) {
        llvm::Value* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) return CreateLoad(VarPtr, Gen);
        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a variable.");
        return NULL;
    }
    llvm::Value* Variable::codegenPtr(CodeGenerator& Gen) {
        llvm::Value* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) return VarPtr;
        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a variable");
        return NULL;
    }

    //Constant, e.g. 1, 1.0, 'c', true, false
    llvm::Value* Constant::codegen(CodeGenerator& Gen) {
        switch (this->_Type) {
        case BuiltInType::TypeID::_Bool:
            return IRBuilder.getInt1(this->_Bool);
        case BuiltInType::TypeID::_Char:
            return IRBuilder.getInt8(this->_Character);
        case BuiltInType::TypeID::_Int:
            return IRBuilder.getInt32(this->_Integer);
        case BuiltInType::TypeID::_Double:
            return llvm::ConstantFP::get(IRBuilder.getDoubleTy(), this->_Real);
        }
    }
    llvm::Value* Constant::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Constant is a right-value.");
        return NULL;
    }

    //Global string, e.g. "123", "3\"124\t\n"
    llvm::Value* GlobalString::codegen(CodeGenerator& Gen) {
        return IRBuilder.CreateGlobalStringPtr(this->_Content.c_str());
    }
    llvm::Value* GlobalString::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Global string (constant) is a right-value.");
        return NULL;
    }
}