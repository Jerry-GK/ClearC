#include "codegen.hpp"
#include "llvm_util.hpp"

//Namespace containing all classes involved in the construction of Abstract Syntax Tree (ast)
namespace ast {
    //A program is composed of several declarations
    ExprValue Program::codegen(CodeGenerator& Gen) {
        for (auto Decl : *(this->_Decls))
            if (Decl)	//We allow empty-declaration which is represented by NULL pointer.
                Decl->codegen(Gen);
        return ExprValue();
    }

    //Function declaration
    ExprValue FuncDecl::codegen(CodeGenerator& Gen) {
        //Set the argument type list. We need to call "GetLLVMType"
        //to change ast::VarType* type to llvm::Type* type
        std::vector<llvm::Type*> ArgTypes;
        bool isVoidArgs = false;
        for (auto ArgType : *(this->_ArgList)) {
            llvm::Type* LLVMType = ArgType->_VarType->GetLLVMType(Gen);
            if (!LLVMType) {
                throw std::logic_error("Define a function " + this->_Name + " using unknown type(s).");
                return ExprValue();
            }
            //Check if it is a "void" type
            if (LLVMType->isVoidTy())
                isVoidArgs = true;
            //not allow to use array type as function argument
            if (LLVMType->isArrayTy())
            {
                throw std::logic_error("Define a function " + this->_Name + " with array type argument, which is not allowed.");
                return ExprValue();
            }
            ArgTypes.push_back(LLVMType);
        }
        //Throw an exception if #args >= 2 and the function has a "void" argument.
        if (ArgTypes.size() != 1 && isVoidArgs) {
            throw std::logic_error("Function " + this->_Name + " has invalid number of arguments with type \"void\".");
            return ExprValue();
        }
        //Clear the arg list of the function only has one "void" arg.
        if (isVoidArgs)
            ArgTypes.clear();

        //Get return type
        llvm::Type* RetType = this->_RetType->GetLLVMType(Gen);
        if (RetType->isArrayTy()) {
            throw std::logic_error("Function " + this->_Name + " returns array type, which is not allowed.");
            return ExprValue();
        }
        //Get function type
        llvm::FunctionType* FuncType = llvm::FunctionType::get(RetType, ArgTypes, this->_ArgList->_VarArg);
        //Create function
        llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::ExternalLinkage, this->_Name, Gen.Module);
        ast::MyFunction* MyFunc = new ast::MyFunction(Func, this->_ArgList);
        auto ret = Gen.AddFunction(this->_Name, MyFunc); //add function into symbol table
        if (ret == ADDFUNC_NAMECONFLICT) {
            throw std::logic_error("Naming conflict for function: " + this->_Name);
            return ExprValue();
        }
        else if (ret == ADDFUNC_ERROR) {
            //unexpected error
            throw std::logic_error("Unexpected error when adding function: " + this->_Name);
            return ExprValue();
        }

        //function has been declared or defined
        if (Func->getName() != this->_Name) {
            //Delete the one we just made and get the existing one.
            Func->eraseFromParent();
            Func = Gen.Module->getFunction(this->_Name);

            if (!this->_FuncBody)
            {
                throw std::logic_error("Redeclare function \"" + this->_Name + "\" which has been declared or defined.");
                return ExprValue();
            }
            //redefine
            if (!Func->empty()) {
                throw std::logic_error("Redefine function \"" + this->_Name + "\" which has been defined.");
                return ExprValue();
            }
            //define the function with conflict arg types with the previous declaration
            if (Func->getFunctionType() != FuncType) {
                throw std::logic_error("Define function \"" + this->_Name + "\" which has conflict arg types with the previous declaration.");
                return ExprValue();
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
                auto ArgVarType = this->_ArgList->at(Index)->_VarType;
                bool isInnerConst = false;
                if (ArgVarType->isPointerType()) {
                    isInnerConst = ((PointerType*)ArgVarType)->isInnerConst();
                }
                auto Alloca = CreateEntryBlockAlloca(Func, this->_ArgList->at(Index)->_Name, ArgTypes[Index]);
                auto ExprVal = new ExprValue(Alloca, "", ArgVarType->_isConst, isInnerConst);
                //Assign the value by "store" instruction
                IRBuilder.CreateStore(ArgIter, Alloca);
                //Add to the symbol table
                if (!Gen.AddVariable(this->_ArgList->at(Index)->_Name, ExprVal))
                {
                    throw std::logic_error("Naming conflict or redefinition for local variable: " + this->_ArgList->at(Index)->_Name + "in the args list of function " + this->_Name);
                    delete ExprVal;
                    return ExprValue();
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
        return ExprValue();
    }

    //Function body
    ExprValue FuncBody::codegen(CodeGenerator& Gen) {
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
            return ExprValue();
        }
        return ExprValue();
    }

    //Variable declaration
    ExprValue VarDecl::codegen(CodeGenerator& Gen) {
        //Get the llvm type
        llvm::Type* VarType = this->_VarType->GetLLVMType(Gen);
        if (VarType == NULL) {
            throw std::logic_error("Define variables with unknown type.");
            return ExprValue();
        }
        if (VarType->isVoidTy()) {
            throw std::logic_error("Cannot define \"void\" variables.");
            return ExprValue();
        }

        //Create variables one by one.
        for (auto& NewVar : *(this->_VarList)) {
            //Determine whether the declaration is inside a function.
            //If so, create an alloca;
            //Otherwise, create a global variable.
            if (Gen.GetCurrentFunction()) {
                //Create an alloca.
                bool isInnerConst = false;
                if (this->_VarType->isPointerType()) {
                    isInnerConst = ((PointerType*)this->_VarType)->isInnerConst();
                }
                auto Alloca = CreateEntryBlockAlloca(Gen.GetCurrentFunction(), NewVar->_Name, VarType);
                auto ExprVal = new ExprValue(Alloca, "", this->_VarType->_isConst, isInnerConst);
                if (!Gen.AddVariable(NewVar->_Name, ExprVal)) {
                    throw std::logic_error("Naming conflict or redefinition for local variable: " + NewVar->_Name);
                    Alloca->eraseFromParent();
                    delete ExprVal;
                    return ExprValue();
                }
                //Assign the initial value by "store" instruction.
                if (NewVar->_InitialExpr) {
                    if (VarType->isArrayTy()) {
                        throw std::logic_error("Initialize array variable \"" + NewVar->_Name + "\", which is not allowed.");
                        return ExprValue();
                    }
                    llvm::Value* Initializer = NULL;
                    ExprValue InitialExprVal = NewVar->_InitialExpr->codegen(Gen);
                    if (InitialExprVal.IsInnerConstPointer && !isInnerConst) {
                        throw std::logic_error("Inner-const pointer cannot initialize a non-inner-const pointer.");
                        return ExprValue();
                    }
                    Initializer = TypeCasting(InitialExprVal, VarType, Gen);
                    if (Initializer == NULL) {
                        throw std::logic_error("Initialize variable \"" + NewVar->_Name + "\" with value of conflict type.");
                        return ExprValue();
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
                bool isInnerConst = false;
                if (this->_VarType->isPointerType()) {
                    isInnerConst = ((PointerType*)this->_VarType)->isInnerConst();
                }
                llvm::Constant* Initializer = NULL;
                if (NewVar->_InitialExpr) {
                    if (VarType->isArrayTy()) {
                        throw std::logic_error("Initialize array variable \"" + NewVar->_Name + "\", which is not allowed.");
                        return ExprValue();
                    }
                    //Global variable must be initialized (if any) by a constant.

                    ExprValue InitialExprVal = NewVar->_InitialExpr->codegen(Gen);
                    if (InitialExprVal.IsInnerConstPointer && !isInnerConst) {
                        throw std::logic_error("Inner-const pointer cannot initialize a non-inner-const pointer.");
                        return ExprValue();
                    }

                    //save the insertion point
                    llvm::BasicBlock* SaveBB = IRBuilder.GetInsertBlock();
                    IRBuilder.SetInsertPoint(Gen.GetEmptyBB());

                    llvm::Value* InitialExpr = TypeCasting(NewVar->_InitialExpr->codegen(Gen), VarType, Gen);
                    //EmptyBB is used for detect whether the initial value is a constant!
                    if (IRBuilder.GetInsertBlock()->size() != 0) {
                        throw std::logic_error("Initialize global variable " + NewVar->_Name + " with non-constant value.");
                        return ExprValue();
                    }
                    if (InitialExpr == NULL) {
                        throw std::logic_error("Initialize variable " + NewVar->_Name + " with value of conflict type.");
                        return ExprValue();
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
                auto ExprVal = new ExprValue(Alloca, "", this->_VarType->_isConst, isInnerConst);
                if (!Gen.AddVariable(NewVar->_Name, ExprVal)) {
                    throw std::logic_error("Naming conflict or redefinition for global variable: " + NewVar->_Name);
                    Alloca->eraseFromParent();
                    delete ExprVal;
                    return ExprValue();
                }
            }
        }
        return ExprValue();
    }

    //Type declaration
    ExprValue TypeDecl::codegen(CodeGenerator& Gen) {
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
            return ExprValue();
        }
        if (!Gen.AddType(this->_Alias, LLVMType))
            throw std::logic_error("Naming conflict or redefinition for type: " + this->_Alias);
        //For struct or union types, we need to generate its body
        if (this->_VarType->isStructType())
            ((ast::StructType*)this->_VarType)->GenerateLLVMTypeBody(Gen);
        return ExprValue();
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
        if (this->_BaseType->_isConst) {
            throw std::logic_error("The base type of array cannot be const.");
            return NULL;
        }
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
    ExprValue Block::codegen(CodeGenerator& Gen) {
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
        return ExprValue();
    }

    //If statement
    ExprValue IfStmt::codegen(CodeGenerator& Gen) {
        //Evaluate condition
        //Since we don't allow variable declarations in if-condition (because we only allow expressions there),
        //we don't need to push a symbol table
        ExprValue Condition = this->_Condition->codegen(Gen);
        //Cast the type to i1
        if (!(Condition.Value = Cast2I1(Condition.Value))) {
            throw std::logic_error("The condition value of if-statement must be either an integer, or a floating-point number, or a pointer.");
            return ExprValue();
        }
        //Create "Then", "Else" and "Merge" block
        llvm::Function* CurrentFunc = Gen.GetCurrentFunction();
        llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(Context, "Then");
        llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(Context, "Else");
        llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(Context, "Merge");
        //Create a branch instruction corresponding to this if statement
        IRBuilder.CreateCondBr(Condition.Value, ThenBB, ElseBB);
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
        return ExprValue();
    }

    //For statement
    ExprValue ForStmt::codegen(CodeGenerator& Gen) {
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
            ExprValue Condition = this->_Condition->codegen(Gen);
            if (!(Condition.Value = Cast2I1(Condition.Value))) {
                throw std::logic_error("The condition value of for-statement must be either an integer, or a floating-point number, or a pointer.");
                return ExprValue();
            }
            IRBuilder.CreateCondBr(Condition.Value, ForLoopBB, ForEndBB);
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
        return ExprValue();
    }

    //Switch statement
    ExprValue SwitchStmt::codegen(CodeGenerator& Gen) {
        llvm::Function* CurrentFunc = Gen.GetCurrentFunction();
        //Evaluate condition
        //Since we don't allow variable declarations in switch-matcher (because we only allow expressions there),
        //we don't need to push a symbol table.
        ExprValue Matcher = this->_Matcher->codegen(Gen);
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
                    CreateCmpEQ(Matcher.Value, this->_CaseList->at(i)->_Condition->codegen(Gen).Value),
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
        return ExprValue();
    }

    //Case statement in switch statement
    ExprValue CaseStmt::codegen(CodeGenerator& Gen) {
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
        return ExprValue();
    }

    //Continue statement
    ExprValue ContinueStmt::codegen(CodeGenerator& Gen) {
        llvm::BasicBlock* ContinueTarget = Gen.GetContinueBlock();
        if (ContinueTarget)
            IRBuilder.CreateBr(ContinueTarget);
        else
            throw std::logic_error("Continue statement should only be used in loops or switch statements.");
        return ExprValue();
    }

    //Break statement
    ExprValue BreakStmt::codegen(CodeGenerator& Gen) {
        llvm::BasicBlock* BreakTarget = Gen.GetBreakBlock();
        if (BreakTarget)
            IRBuilder.CreateBr(BreakTarget);
        else
            throw std::logic_error("Break statement should only be used in loops or switch statements.");
        return ExprValue();
    }

    //Return statement
    ExprValue ReturnStmt::codegen(CodeGenerator& Gen) {
        llvm::Function* Func = Gen.GetCurrentFunction();
        if (!Func) {
            throw std::logic_error("Return statement should only be used in function bodies.");
            return ExprValue();
        }
        if (this->_RetVal == NULL) {
            if (Func->getReturnType()->isVoidTy())
                IRBuilder.CreateRetVoid();
            else {
                throw std::logic_error("Expected an expression after return statement.");
                return ExprValue();
            }
        }
        else {
            ExprValue RetVal = ExprValue(TypeCasting(this->_RetVal->codegen(Gen), Func->getReturnType(), Gen));
            if (!RetVal.Value) {
                throw std::logic_error("The type of return value doesn't match and cannot be cast to the return type.");
                return ExprValue();
            }
            IRBuilder.CreateRet(RetVal.Value);
        }
        return ExprValue();
    }

    //Subscript, e.g. a[10], b[2][3]
    ExprValue Subscript::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue Subscript::codegenPtr(CodeGenerator& Gen) {
        //Get the pointer
        ExprValue ArrayPtr = this->_Array->codegen(Gen);
        if (!ArrayPtr.Value->getType()->isPointerTy()) {
            throw std::logic_error("Subscript operator \"[]\" must be applied to pointers or arrays.");
            return ExprValue();
        }
        //Get the index value
        ExprValue Subspt = this->_Index->codegen(Gen);
        if (!(Subspt.Value->getType()->isIntegerTy())) {
            throw std::logic_error("Subscription should be an integer.");
            return ExprValue();
        }
        //Return pointer addition
        return ExprValue(CreateAdd(ArrayPtr, Subspt, Gen));
    }

    //Operator sizeof() in C
    ExprValue SizeOf::codegen(CodeGenerator& Gen) {
        if (this->_Arg1)//Expression
            return ExprValue(IRBuilder.getInt64(Gen.GetTypeSize(this->_Arg1->codegen(Gen).Value->getType())));
        else if (this->_Arg2)//VarType
            return ExprValue(IRBuilder.getInt64(Gen.GetTypeSize(this->_Arg2->GetLLVMType(Gen))));
        else {//Single identifier
            llvm::Type* Type = Gen.FindType(this->_Arg3);
            if (Type) {
                this->_Arg2 = new DefinedType(this->_Arg3);
                return ExprValue(IRBuilder.getInt64(Gen.GetTypeSize(Type)));
            }
            const ExprValue* VarPtr = Gen.FindVariable(this->_Arg3);
            if (VarPtr) {
                this->_Arg1 = new Variable(this->_Arg3);
                return ExprValue(IRBuilder.getInt64(Gen.GetTypeSize(VarPtr->Value->getType()->getNonOpaquePointerElementType())));
            }
            throw std::logic_error(this->_Arg3 + " is neither a type nor a variable.");
            return ExprValue();
        }
    }
    ExprValue SizeOf::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Sizeof() only returns right-values.");
        return ExprValue();
    }

    //Function call
    ExprValue FunctionCall::codegen(CodeGenerator& Gen) {
        //Get the function. Throw exception if the function doesn't exist.
        ast::MyFunction* MyFunc = Gen.FindFunction(this->_FuncName);
        if (MyFunc == NULL) {
            throw std::domain_error(this->_FuncName + " is not a defined function.");
            return ExprValue();
        }
        llvm::Function* Func = MyFunc->LLVMFunc;
        auto Args = MyFunc->Args;
        //Check the number of args. If Func took a different number of args, reject.
        if (Func->isVarArg() && this->_ArgList->size() < Func->arg_size() ||
            !Func->isVarArg() && this->_ArgList->size() != Func->arg_size()) {
            throw std::invalid_argument("Args doesn't match when calling function " + this->_FuncName + ". Expected " + std::to_string(Func->arg_size()) + ", got " + std::to_string(this->_ArgList->size()));
            return ExprValue();
        }
        //Check arg types. If Func took different different arg types, reject.
        std::vector<llvm::Value*> ArgList;
        size_t Index = 0;
        for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
            ExprValue Arg = this->_ArgList->at(Index)->codegen(Gen);
            auto ArgType = Args->at(Index)->_VarType;

            if (Arg.IsInnerConstPointer && ArgType->isPointerType() && !((PointerType*)ArgType)->isInnerConst()) {
                throw std::invalid_argument("Cannot pass an inner-const pointer to a non-inner-const pointer argument.");
                return ExprValue();
            }
            Arg.Value = TypeCasting(Arg, ArgIter->getType(), Gen);
            if (Arg.Value == NULL) {
                throw std::invalid_argument(std::to_string(Index) + "-th arg type doesn't match when calling function " + this->_FuncName + ".");
                return ExprValue();
            }
            ArgList.push_back(Arg.Value);
        }
        //Continue to push arguments if this function takes a variable number of arguments
        //According to the C standard, bool/char/short should be extended to int, and float should be extended to double
        if (Func->isVarArg())
            for (; Index < this->_ArgList->size(); Index++) {
                ExprValue Arg = this->_ArgList->at(Index)->codegen(Gen);
                if (Arg.Value->getType()->isIntegerTy())
                    Arg.Value = TypeUpgrading(Arg.Value, IRBuilder.getInt32Ty());
                else if (Arg.Value->getType()->isFloatingPointTy())
                    Arg.Value = TypeUpgrading(Arg.Value, IRBuilder.getDoubleTy());
                ArgList.push_back(Arg.Value);
            }
        //Create function call.
        return ExprValue(IRBuilder.CreateCall(Func, ArgList));
    }
    ExprValue FunctionCall::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Function call only returns right-values.");
        return ExprValue();
    }

    //Structure reference, e.g. a.x, a.y
    ExprValue StructReference::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue StructReference::codegenPtr(CodeGenerator& Gen) {
        ExprValue StructPtr = this->_Struct->codegenPtr(Gen);
        if (!StructPtr.Value->getType()->isPointerTy() || !StructPtr.Value->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Reference operator \".\" must be apply to structs.");
            return ExprValue();
        }
        //Since C language uses name instead of index to fetch the element inside a struct,
        //we need to fetch the ast::StructType* instance according to the llvm::StructType* instance.
        //And it is the same with union types.
        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr.Value->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
                return ExprValue();
            }
            std::vector<llvm::Value*> Indices;
            Indices.push_back(IRBuilder.getInt32(0));
            Indices.push_back(IRBuilder.getInt32(MemIndex));
            return ExprValue(IRBuilder.CreateGEP(StructPtr.Value->getType()->getNonOpaquePointerElementType(), StructPtr.Value, Indices));
        }
        return ExprValue();
    }

    //Structure dereference, e.g. a->x, a->y
    ExprValue StructDereference::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue StructDereference::codegenPtr(CodeGenerator& Gen) {
        ExprValue StructPtr = this->_StructPtr->codegen(Gen);
        if (!StructPtr.Value->getType()->isPointerTy() || !StructPtr.Value->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Dereference operator \"->\" must be apply to struct pointers.");
            return ExprValue();
        }
        //Since C language uses name instead of index to fetch the element inside a struct,
        //we need to fetch the ast::StructType* instance according to the llvm::StructType* instance.
        //And it is the same with union types.
        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr.Value->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
                return ExprValue();
            }
            std::vector<llvm::Value*> Indices;
            Indices.push_back(IRBuilder.getInt32(0));
            Indices.push_back(IRBuilder.getInt32(MemIndex));
            return ExprValue(IRBuilder.CreateGEP(StructPtr.Value->getType()->getNonOpaquePointerElementType(), StructPtr.Value, Indices));
        }
        return ExprValue();
    }

    //Unary plus, e.g. +i, +j, +123
    ExprValue UnaryPlus::codegen(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand.Value->getType()->isIntegerTy() ||
            Operand.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary plus must be applied to integers or floating-point numbers.");
        return ExprValue(Operand);
    }
    ExprValue UnaryPlus::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Unary plus only returns right-values.");
        return ExprValue();
    }

    //Unary minus, e.g. -i, -j, -123
    ExprValue UnaryMinus::codegen(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand.Value->getType()->isIntegerTy() ||
            Operand.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary minus must be applied to integers or floating-point numbers.");
        if (Operand.Value->getType()->isIntegerTy())
            return ExprValue(IRBuilder.CreateNeg(Operand.Value));
        else
            return ExprValue(IRBuilder.CreateFNeg(Operand.Value));
    }
    ExprValue UnaryMinus::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Unary minus only returns right-values.");
        return ExprValue();
    }

    //Type cast, e.g. (float)n, (int)1.0
    ExprValue TypeCast::codegen(CodeGenerator& Gen) {
        ExprValue ExprVal = this->_Operand->codegen(Gen);
        if (ExprVal.IsInnerConstPointer
            && !(this->_VarType->isPointerType() && ((PointerType*)(this->_VarType))->isInnerConst())) {
            throw std::logic_error("Cannot type cast an inner-const pointer to a non-inner-const pointer.");
            return ExprValue();
        }

        ExprValue Ret = ExprValue(TypeCasting(ExprVal, this->_VarType->GetLLVMType(Gen), Gen), ExprVal.Name, ExprVal.IsConst, ExprVal.IsInnerConstPointer, ExprVal.IsPointingToInnerConst);
        if (Ret.Value == NULL) {
            throw std::logic_error("Unable to do type casting.");
            return ExprValue();
        }
        return Ret;
    }
    ExprValue TypeCast::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Type casting only returns right-values.");
        return ExprValue();
    }

    //Prefix increment, e.g. ++i
    ExprValue PrefixInc::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue PrefixInc::codegenPtr(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do prefix increment.");
            return ExprValue();
        }
        ExprValue OpValue = ExprValue(IRBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy() ||
            OpValue.Value->getType()->isPointerTy())
            )
            throw std::logic_error("Prefix increment must be applied to integers, floating-point numbers or pointers.");
        ExprValue OpValuePlus = ExprValue(CreateAdd(OpValue, ExprValue(IRBuilder.getInt1(1)), Gen));
        IRBuilder.CreateStore(OpValuePlus.Value, Operand.Value);
        return ExprValue(Operand);
    }

    //Postfix increment, e.g. i++
    ExprValue PostfixInc::codegen(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do postfix increment.");
            return ExprValue();
        }
        ExprValue OpValue = ExprValue(IRBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy() ||
            OpValue.Value->getType()->isPointerTy())
            )
            throw std::logic_error("Postfix increment must be applied to integers, floating-point numbers or pointers.");
        ExprValue OpValuePlus = ExprValue(CreateAdd(OpValue, ExprValue(IRBuilder.getInt1(1)), Gen));
        IRBuilder.CreateStore(OpValuePlus.Value, Operand.Value);
        return ExprValue(OpValue);
    }
    ExprValue PostfixInc::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Postfix increment only returns right-values.");
        return ExprValue();
    }

    //Prefix decrement, e.g. --i
    ExprValue PrefixDec::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue PrefixDec::codegenPtr(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do prefix decrement.");
            return ExprValue();
        }
        ExprValue OpValue = ExprValue(IRBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy() ||
            OpValue.Value->getType()->isPointerTy())
            )
            throw std::logic_error("Prefix decrement must be applied to integers, floating-point numbers or pointers.");
        ExprValue OpValueMinus = ExprValue(CreateSub(OpValue, ExprValue(IRBuilder.getInt1(1)), Gen));
        IRBuilder.CreateStore(OpValueMinus.Value, Operand.Value);
        return ExprValue(Operand);
    }

    //Postfix decrement, e.g. i--
    ExprValue PostfixDec::codegen(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do postfix decrement.");
            return ExprValue();
        }
        ExprValue OpValue = ExprValue(IRBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy() ||
            OpValue.Value->getType()->isPointerTy())
            )
            throw std::logic_error("Postfix decrement must be applied to integers, floating-point numbers or pointers.");
        ExprValue OpValueMinus = ExprValue(CreateSub(OpValue, ExprValue(IRBuilder.getInt1(1)), Gen));
        IRBuilder.CreateStore(OpValueMinus.Value, Operand.Value);
        return ExprValue(OpValue);
    }
    ExprValue PostfixDec::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Postfix decrement only returns right-values.");
        return ExprValue();
    }

    //Indirection, e.g. *ptr
    ExprValue Indirection::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue Indirection::codegenPtr(CodeGenerator& Gen) {
        ExprValue Ptr = this->_Operand->codegen(Gen);
        if (!Ptr.Value->getType()->isPointerTy()) {
            throw std::logic_error("Indirection operator \"dptr()\" only applies on pointers or arrays.");
            return ExprValue();
        }
        return ExprValue(Ptr);
    }

    //Fetch address, e.g. &i
    ExprValue AddressOf::codegen(CodeGenerator& Gen) {
        return ExprValue(this->_Operand->codegenPtr(Gen));
    }
    ExprValue AddressOf::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Address operator \"addr()\" only returns right-values.");
        return ExprValue();
    }

    //Logic NOT, e.g. !x
    ExprValue LogicNot::codegen(CodeGenerator& Gen) {
        return ExprValue(IRBuilder.CreateICmpEQ(Cast2I1(this->_Operand->codegen(Gen).Value), IRBuilder.getInt1(false)));
    }
    ExprValue LogicNot::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic NOT operator \"!\" only returns right-values.");
        return ExprValue();
    }

    //Bitwise NOT, e.g. ~x
    ExprValue BitwiseNot::codegen(CodeGenerator& Gen) {
        ExprValue Operand = this->_Operand->codegen(Gen);
        if (!Operand.Value->getType()->isIntegerTy()) {
            throw std::logic_error("Bitwise NOT operator \"~\" must be applied to integers.");
            return ExprValue();
        }
        return ExprValue(IRBuilder.CreateNot(Operand.Value));
    }
    ExprValue BitwiseNot::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise NOT operator \"~\" only returns right-values.");
        return ExprValue();
    }

    //Division, e.g. x/y
    ExprValue Division::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateDiv(LHS, RHS, Gen));
    }
    ExprValue Division::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Division operator \"/\" only returns right-values.");
        return ExprValue();
    }

    //Multiplication, e.g. x*y
    ExprValue Multiplication::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateMul(LHS, RHS, Gen));
    }
    ExprValue Multiplication::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Multiplication operator \"*\" only returns right-values.");
        return ExprValue();
    }

    //Modulo, e.g. x%y
    ExprValue Modulo::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateMod(LHS, RHS, Gen));
    }
    ExprValue Modulo::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Modulo operator \"%\" only returns right-values.");
        return ExprValue();
    }

    //Addition, e.g. x+y
    ExprValue Addition::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAdd(LHS, RHS, Gen));
    }
    ExprValue Addition::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Addition operator \"+\" only returns right-values.");
        return ExprValue();
    }

    //Subtraction, e.g. x-y
    ExprValue Subtraction::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateSub(LHS, RHS, Gen));
    }
    ExprValue Subtraction::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Subtraction operator \"-\" only returns right-values.");
        return ExprValue();
    }

    //LeftShift, e.g. x<<y
    ExprValue LeftShift::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateShl(LHS, RHS, Gen));
    }
    ExprValue LeftShift::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Left shifting operator \"<<\" only returns right-values.");
        return ExprValue();
    }

    //RightShift, e.g. x>>y
    ExprValue RightShift::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateShr(LHS, RHS, Gen));
    }
    ExprValue RightShift::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Right shifting operator \">>\" only returns right-values.");
        return ExprValue();
    }

    //LogicGT, e.g. x>y
    ExprValue LogicGT::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS.Value, RHS.Value)) {
            if (LHS.Value->getType()->isIntegerTy())
                return ExprValue(IRBuilder.CreateICmpSGT(LHS.Value, RHS.Value));
            else
                return ExprValue(IRBuilder.CreateFCmpOGT(LHS.Value, RHS.Value));
        }
        //Pointer compare
        if (LHS.Value->getType()->isPointerTy() && LHS.Value->getType() == RHS.Value->getType()) {
            return ExprValue(IRBuilder.CreateICmpUGT(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isPointerTy() && RHS.Value->getType()->isIntegerTy()) {
            return ExprValue(IRBuilder.CreateICmpUGT(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isIntegerTy() && RHS.Value->getType()->isPointerTy()) {
            return ExprValue(IRBuilder.CreateICmpUGT(
                TypeUpgrading(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \">\" using unsupported type combination.");
    }
    ExprValue LogicGT::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \">\" only returns right-values.");
        return ExprValue();
    }

    //LogicGE, e.g. x>=y
    ExprValue LogicGE::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS.Value, RHS.Value)) {
            if (LHS.Value->getType()->isIntegerTy())
                return ExprValue(IRBuilder.CreateICmpSGE(LHS.Value, RHS.Value));
            else
                return ExprValue(IRBuilder.CreateFCmpOGE(LHS.Value, RHS.Value));
        }
        //Pointer compare
        if (LHS.Value->getType()->isPointerTy() && LHS.Value->getType() == RHS.Value->getType()) {
            return ExprValue(IRBuilder.CreateICmpUGE(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isPointerTy() && RHS.Value->getType()->isIntegerTy()) {
            return ExprValue(IRBuilder.CreateICmpUGE(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isIntegerTy() && RHS.Value->getType()->isPointerTy()) {
            return ExprValue(IRBuilder.CreateICmpUGE(
                TypeUpgrading(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \">=\" using unsupported type combination.");
    }
    ExprValue LogicGE::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \">=\" only returns right-values.");
        return ExprValue();
    }

    //LogicLT, e.g. x<y
    ExprValue LogicLT::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS.Value, RHS.Value)) {
            if (LHS.Value->getType()->isIntegerTy())
                return ExprValue(IRBuilder.CreateICmpSLT(LHS.Value, RHS.Value));
            else
                return ExprValue(IRBuilder.CreateFCmpOLT(LHS.Value, RHS.Value));
        }
        //Pointer compare
        if (LHS.Value->getType()->isPointerTy() && LHS.Value->getType() == RHS.Value->getType()) {
            return ExprValue(IRBuilder.CreateICmpULT(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isPointerTy() && RHS.Value->getType()->isIntegerTy()) {
            return ExprValue(IRBuilder.CreateICmpULT(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isIntegerTy() && RHS.Value->getType()->isPointerTy()) {
            return ExprValue(IRBuilder.CreateICmpULT(
                TypeUpgrading(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \"<\" using unsupported type combination.");
    }
    ExprValue LogicLT::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"<\" only returns right-values.");
        return ExprValue();
    }

    //LogicLE, e.g. x<=y
    ExprValue LogicLE::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS.Value, RHS.Value)) {
            if (LHS.Value->getType()->isIntegerTy())
                return ExprValue(IRBuilder.CreateICmpSLE(LHS.Value, RHS.Value));
            else
                return ExprValue(IRBuilder.CreateFCmpOLE(LHS.Value, RHS.Value));
        }
        //Pointer compare
        if (LHS.Value->getType()->isPointerTy() && LHS.Value->getType() == RHS.Value->getType()) {
            return ExprValue(IRBuilder.CreateICmpULE(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isPointerTy() && RHS.Value->getType()->isIntegerTy()) {
            return ExprValue(IRBuilder.CreateICmpULE(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isIntegerTy() && RHS.Value->getType()->isPointerTy()) {
            return ExprValue(IRBuilder.CreateICmpULE(
                TypeUpgrading(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \"<=\" using unsupported type combination.");
    }
    ExprValue LogicLE::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"<=\" only returns right-values.");
        return ExprValue();
    }

    //LogicEQ, e.g. x==y
    ExprValue LogicEQ::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateCmpEQ(LHS.Value, RHS.Value));
    }
    ExprValue LogicEQ::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"==\" only returns right-values.");
        return ExprValue();
    }

    //LogicNEQ, e.g. x!=y
    ExprValue LogicNEQ::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LHS.Value, RHS.Value)) {
            if (LHS.Value->getType()->isIntegerTy())
                return ExprValue(IRBuilder.CreateICmpNE(LHS.Value, RHS.Value));
            else
                return ExprValue(IRBuilder.CreateFCmpONE(LHS.Value, RHS.Value));
        }
        //Pointer compare
        if (LHS.Value->getType()->isPointerTy() && LHS.Value->getType() == RHS.Value->getType()) {
            return ExprValue(IRBuilder.CreateICmpNE(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isPointerTy() && RHS.Value->getType()->isIntegerTy()) {
            return ExprValue(IRBuilder.CreateICmpNE(
                IRBuilder.CreatePtrToInt(LHS.Value, IRBuilder.getInt64Ty()),
                TypeUpgrading(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        else if (LHS.Value->getType()->isIntegerTy() && RHS.Value->getType()->isPointerTy()) {
            return ExprValue(IRBuilder.CreateICmpNE(
                TypeUpgrading(LHS.Value, IRBuilder.getInt64Ty()),
                IRBuilder.CreatePtrToInt(RHS.Value, IRBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \"!=\" using unsupported type combination.");
    }
    ExprValue LogicNEQ::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"!=\" only returns right-values.");
        return ExprValue();
    }

    //BitwiseAND, e.g. x&y
    ExprValue BitwiseAND::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateBitwiseAND(LHS, RHS, Gen));
    }
    ExprValue BitwiseAND::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise AND operator \"&\" only returns right-values.");
        return ExprValue();
    }

    //BitwiseXOR, e.g. x^y
    ExprValue BitwiseXOR::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateBitwiseXOR(LHS, RHS, Gen));
    }
    ExprValue BitwiseXOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise XOR operator \"^\" only returns right-values.");
        return ExprValue();
    }

    //BitwiseOR, e.g. x|y
    ExprValue BitwiseOR::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateBitwiseOR(LHS, RHS, Gen));
    }
    ExprValue BitwiseOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise OR operator \"|\" only returns right-values.");
        return ExprValue();
    }

    //LogicAND, e.g. x&&y
    ExprValue LogicAND::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        LHS = ExprValue(Cast2I1(LHS.Value));
        if (LHS.Value == NULL) {
            throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
            return ExprValue();
        }
        RHS = ExprValue(Cast2I1(RHS.Value));
        if (RHS.Value == NULL) {
            throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
            return ExprValue();
        }
        return ExprValue(IRBuilder.CreateLogicalAnd(LHS.Value, RHS.Value));
    }
    ExprValue LogicAND::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic AND operator \"&&\" only returns right-values.");
        return ExprValue();
    }

    //LogicOR, e.g. x||y
    ExprValue LogicOR::codegen(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegen(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        LHS = ExprValue(Cast2I1(LHS.Value));
        if (LHS.Value == NULL) {
            throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
            return ExprValue();
        }
        RHS = ExprValue(Cast2I1(RHS.Value));
        if (RHS.Value == NULL) {
            throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
            return ExprValue();
        }
        return ExprValue(IRBuilder.CreateLogicalOr(LHS.Value, RHS.Value));
    }
    ExprValue LogicOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic OR operator \"||\" only returns right-values.");
        return ExprValue();
    }

    //TernaryCondition, e.g. (cond)?x:y
    ExprValue TernaryCondition::codegen(CodeGenerator& Gen) {
        ExprValue Condition = ExprValue(Cast2I1(this->_Condition->codegen(Gen).Value));
        if (Condition.Value == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
            return ExprValue();
        }
        ExprValue True = this->_Then->codegen(Gen);
        ExprValue False = this->_Else->codegen(Gen);
        if (True.Value->getType() == False.Value->getType() || TypeUpgrading(True.Value, False.Value)) {
            return ExprValue(IRBuilder.CreateSelect(Condition.Value, True.Value, False.Value));
        }
        else {
            throw std::domain_error("Thernary operand \" ? : \" using unsupported type combination.");
            return ExprValue();
        }
    }
    ExprValue TernaryCondition::codegenPtr(CodeGenerator& Gen) {
        ExprValue Condition = ExprValue(Cast2I1(this->_Condition->codegen(Gen).Value));
        if (Condition.Value == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
            return ExprValue();
        }
        ExprValue True = this->_Then->codegenPtr(Gen);
        ExprValue False = this->_Else->codegenPtr(Gen);
        if (True.Value->getType() != False.Value->getType()) {
            throw std::domain_error("When using thernary expressions \" ? : \" as left-values, the latter two operands must be of the same type.");
            return ExprValue();
        }
        return ExprValue(IRBuilder.CreateSelect(Condition.Value, True.Value, False.Value));
    }

    //DirectAssign, e.g. x=y
    ExprValue DirectAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue DirectAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS, RHS, Gen));
    }

    //DivAssign, e.g. x/=y
    ExprValue DivAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue DivAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateDiv(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //MulAssign, e.g. x*=y
    ExprValue MulAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue MulAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateMul(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //ModAssign, e.g. x%=y
    ExprValue ModAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue ModAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateMod(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //AddAssign, e.g. x+=y
    ExprValue AddAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue AddAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateAdd(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //SubAssign, e.g. x-=y
    ExprValue SubAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue SubAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateSub(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //SHLAssign, e.g. x<<=y
    ExprValue SHLAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue SHLAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateShl(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //SHRAssign, e.g. x>>=y
    ExprValue SHRAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue SHRAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateShr(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //BitwiseANDAssign, e.g. x&=y
    ExprValue BitwiseANDAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue BitwiseANDAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateBitwiseAND(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //BitwiseXORAssign, e.g. x^=y
    ExprValue BitwiseXORAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue BitwiseXORAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateBitwiseXOR(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //BitwiseORAssign, e.g. x|=y
    ExprValue BitwiseORAssign::codegen(CodeGenerator& Gen) {
        return ExprValue(CreateLoad(this->codegenPtr(Gen).Value, Gen));
    }
    ExprValue BitwiseORAssign::codegenPtr(CodeGenerator& Gen) {
        ExprValue LHS = this->_LHS->codegenPtr(Gen);
        ExprValue RHS = this->_RHS->codegen(Gen);
        return ExprValue(CreateAssignment(LHS,
            ExprValue(CreateBitwiseOR(
                ExprValue(IRBuilder.CreateLoad(
                    LHS.Value->getType()->getNonOpaquePointerElementType(),
                    LHS.Value)),
                RHS,
                Gen)),
            Gen
        ));
    }

    //Comma expression, e.g. a,b,c,1
    ExprValue CommaExpr::codegen(CodeGenerator& Gen) {
        this->_LHS->codegen(Gen);
        return ExprValue(this->_RHS->codegen(Gen));
    }
    ExprValue CommaExpr::codegenPtr(CodeGenerator& Gen) {
        this->_LHS->codegen(Gen);
        return ExprValue(this->_RHS->codegenPtr(Gen));
    }

    //Variable, e.g. x
    ExprValue Variable::codegen(CodeGenerator& Gen) {
        const ExprValue* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) {
            return ExprValue(CreateLoad(VarPtr->Value, Gen), VarPtr->Name, VarPtr->IsConst, VarPtr->IsInnerConstPointer);
        }
        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a variable.");
        return ExprValue();
    }
    ExprValue Variable::codegenPtr(CodeGenerator& Gen) {
        const ExprValue* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) return ExprValue(VarPtr->Value, VarPtr->Name, false,
            VarPtr->IsConst, VarPtr->IsInnerConstPointer);

        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a variable");
        return ExprValue();
    }

    //Constant, e.g. 1, 1.0, 'c', true, false
    ExprValue Constant::codegen(CodeGenerator& Gen) {
        switch (this->_Type) {
        case BuiltInType::TypeID::_Bool:
            return ExprValue(IRBuilder.getInt1(this->_Bool));
        case BuiltInType::TypeID::_Char:
            return ExprValue(IRBuilder.getInt8(this->_Character));
        case BuiltInType::TypeID::_Int:
            return ExprValue(IRBuilder.getInt32(this->_Integer));
        case BuiltInType::TypeID::_Double:
            return ExprValue(llvm::ConstantFP::get(IRBuilder.getDoubleTy(), this->_Real));
        default:
            throw std::logic_error("Unknown constant type.");
            return ExprValue();
        }
    }
    ExprValue Constant::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Constant is a right-value.");
        return ExprValue();
    }

    //Global string, e.g. "123", "3\"124\t\n"
    ExprValue GlobalString::codegen(CodeGenerator& Gen) {
        return ExprValue(IRBuilder.CreateGlobalStringPtr(this->_Content.c_str()));
    }
    ExprValue GlobalString::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Global string (constant) is a right-value.");
        return ExprValue();
    }
}