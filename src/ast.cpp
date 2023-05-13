#include "codegen.hpp"
#include "llvm_util.hpp"

//Namespace containing all classes involved in the construction of Abstract Syntax Tree (ast)
namespace ast {
    //A program is composed of several declarations
    MyValue Program::codegen(CodeGenerator& Gen) {
        for (auto Decl : *(this->_Decls))
            if (Decl)	//We allow empty-declaration which is represented by NULL pointer.
                Decl->codegen(Gen);
        return MyValue();
    }

    //Function declaration
    MyValue FuncDecl::codegen(CodeGenerator& Gen) {
        //Set the argument type list. We need to call "GetLLVMType"
        //to change ast::VarType* type to llvm::Type* type
        std::vector<llvm::Type*> ArgTypes;
        bool HasBaseType = !this->_TypeName.empty();

        //push a const ptr<Type> into ArgTypes if the function's TypeName is not empty
        if (HasBaseType) {
            MyType* MyTy = Gen.FindType(this->_TypeName);
            if (!MyTy) {
                throw std::logic_error("Define a function " + this->_FuncName + " belongs to unknown base type " + this->_TypeName);
                return MyValue();
            }

            auto LLVMType = MyTy->LLVMType;
            if (!LLVMType->isStructTy()) {
                throw std::logic_error("Define a function " + this->_FuncName + " whose base type " + this->_TypeName + " is not a struct type.");
                return MyValue();
            }
            auto LLVMPointerType = llvm::PointerType::get(LLVMType, 0U);
            ArgTypes.push_back(LLVMPointerType);
        }

        bool isVoidArgs = false;
        for (auto ArgType : *(this->_ArgList)) {
            llvm::Type* LLVMType = ArgType->_VarType->GetLLVMType(Gen);
            if (!LLVMType) {
                throw std::logic_error("Define a function " + this->_FuncName + " using unknown type(s).");
                return MyValue();
            }
            //Check if it is a "void" type
            if (LLVMType->isVoidTy())
                isVoidArgs = true;
            //not allow to use array type as function argument
            if (LLVMType->isArrayTy())
            {
                throw std::logic_error("Define a function " + this->_FuncName + " with array type argument, which is not allowed.");
                return MyValue();
            }
            ArgTypes.push_back(LLVMType);
        }
        //Throw an exception if #args >= 2 and the function has a "void" argument.
        if (this->_ArgList->size() != 1 && isVoidArgs) {
            throw std::logic_error("Function " + this->_FuncName + " has invalid number of arguments with type \"void\".");
            return MyValue();
        }
        //Clear the arg list of the function only has one "void" arg.
        if (isVoidArgs)
            ArgTypes.pop_back();

        //Get return type
        llvm::Type* RetType = this->_RetType->GetLLVMType(Gen);
        if (RetType->isArrayTy()) {
            throw std::logic_error("Function " + this->_FuncName + " returns array type, which is not allowed.");
            return MyValue();
        }
        //Get function type
        llvm::FunctionType* FuncType = llvm::FunctionType::get(RetType, ArgTypes, this->_ArgList->_VarArg);

        //Create function
        llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::ExternalLinkage, this->_FuncName, Gen.Module);

        //function has been declared or defined
        if (Func->getName() != this->_FuncName) {
            //Delete the one we just made and get the existing one.
            Func->eraseFromParent();
            Func = Gen.Module->getFunction(this->_FuncName);

            if (!this->_FuncBody)
            {
                throw std::logic_error("Redeclare function \"" + this->_FuncName + "\" which has been declared or defined.");
                return MyValue();
            }
            //redefine
            if (!Func->empty()) {
                throw std::logic_error("Redefine function \"" + this->_FuncName + "\" which has been defined.");
                return MyValue();
            }
            //define the function with conflict arg types with the previous declaration
            auto MyFunc = Gen.FindFunction(this->_FuncName);
            if (!MyFunc)
            {
                throw std::logic_error("Unexpected error when getting function \"" + this->_FuncName + "\".");
                return MyValue();
            }
            for (int i = 0;i < this->_ArgList->size();i++) {
                auto prevArgType = MyFunc->Args->at(i)->_VarType;
                auto curArgType = this->_ArgList->at(i)->_VarType;
                bool TypeSame = prevArgType->_LLVMType->getTypeID() == curArgType->_LLVMType->getTypeID();
                bool IsConstSame = prevArgType->_isConst == curArgType->_isConst;
                bool prevIsInnerConst = false;
                if (prevArgType->isPointerType()) {
                    prevIsInnerConst = ((PointerType*)prevArgType)->isInnerConst();
                }
                bool curIsInnerConst = false;
                if (curArgType->isPointerType()) {
                    curIsInnerConst = ((PointerType*)curArgType)->isInnerConst();
                }
                bool IsInnerConstSame = prevIsInnerConst == curIsInnerConst;

                if (!(TypeSame && IsConstSame && IsInnerConstSame)) {
                    throw std::logic_error("Define function \"" + this->_FuncName + "\" which has conflict arg type of the "
                        + std::to_string(i + 1) + "'th arg with the previous declaration.");
                    return MyValue();
                }
            }
            //this "redundant" check is necessary for check more detailed type conflict
            if (Func->getFunctionType() != FuncType) {
                throw std::logic_error("Define function \"" + this->_FuncName + "\" which has conflict arg types with the previous declaration.");
                return MyValue();
            }
            //(declare and) define a function which has been declared is allowed, ignore the second declaration
        }


        ast::MyFunction* MyFunc = new ast::MyFunction(Func, this->_ArgList, this->_RetType, this->_TypeName);
        auto ret = Gen.AddFunction(this->_FuncName, MyFunc); //add function into symbol table
        if (ret == ADDFUNC_NAMECONFLICT) {
            throw std::logic_error("Naming conflict for function: " + this->_FuncName);
            return MyValue();
        }
        else if (ret == ADDFUNC_ERROR) {
            //unexpected error
            throw std::logic_error("Unexpected error when adding function: " + this->_FuncName);
            return MyValue();
        }
        // ret == ADDFUNC_DECLARED is normal, ignore this redeclaration

        //If this function has a body, generate its body's code.
        if (this->_FuncBody) {
            //Create a new basic block to start insertion into.
            llvm::BasicBlock* FuncBlock = llvm::BasicBlock::Create(GlobalContext, "entry", Func);
            GlobalBuilder.SetInsertPoint(FuncBlock);
            //Create allocated space for arguments.
            Gen.PushSymbolTable();	//This variable table is only used to store the arguments of the function
            size_t Index = 0;
            auto ArgIter = Func->arg_begin();
            if (HasBaseType)
            {
                //add hidden this pointer
                auto Alloca = CreateEntryBlockAlloca(Func, "0this", ArgIter->getType());
                auto MyVal = new MyValue(Alloca, "", true, false);
                //Assign the value by "store" instruction
                GlobalBuilder.CreateStore(ArgIter, Alloca);
                //Add to the symbol table
                if (!Gen.AddVariable("0this", MyVal))
                {
                    throw std::logic_error("Cannot add this pointer in function " + this->_FuncName);
                    delete MyVal;
                    return MyValue();
                }
                ArgIter++;
            }
            for (; ArgIter < Func->arg_end(); ArgIter++, Index++) {
                //Create alloca
                auto ArgVarType = this->_ArgList->at(Index)->_VarType;
                bool isInnerConst = false;
                if (ArgVarType->isPointerType()) {
                    isInnerConst = ((PointerType*)ArgVarType)->isInnerConst();
                }
                auto Alloca = CreateEntryBlockAlloca(Func, this->_ArgList->at(Index)->_Name, ArgIter->getType());
                auto MyVal = new MyValue(Alloca, "", ArgVarType->_isConst || ArgVarType->isArrayType(), isInnerConst);
                //Assign the value by "store" instruction
                GlobalBuilder.CreateStore(ArgIter, Alloca);
                //Add to the symbol table
                if (!Gen.AddVariable(this->_ArgList->at(Index)->_Name, MyVal))
                {
                    throw std::logic_error("Naming conflict or redefinition for local variable: " + this->_ArgList->at(Index)->_Name + " in the args list of function " + this->_FuncName);
                    delete MyVal;
                    return MyValue();
                }
            }

            //Generate code of the function body
            Gen.SetCurFunction(MyFunc);
            Gen.PushSymbolTable();
            this->_FuncBody->codegen(Gen);
            Gen.PopSymbolTable();
            Gen.SetCurFunction(NULL);
            Gen.PopSymbolTable();	//We need to pop out an extra variable table.

            return MyValue();
        }
        return MyValue();
    }

    //Function body
    MyValue FuncBody::codegen(CodeGenerator& Gen) {
        //Generate the statements in FuncBody, one by one.
        for (auto& Stmt : *(this->_Content))
            //If the current block already has a terminator,
            //i.e. a "return" statement is generated, stop;
            //Otherwise, continue generating.
            if (GlobalBuilder.GetInsertBlock()->getTerminator())
                break;
            else
                Stmt->codegen(Gen);
        //return statement is needed for all functions, including void functions.
        if (!GlobalBuilder.GetInsertBlock()->getTerminator()) {
            throw std::logic_error("Function does not have a return statement (void function needs return as well).");
            return MyValue();
        }
        return MyValue();
    }

    //Variable declaration
    MyValue VarDecl::codegen(CodeGenerator& Gen) {
        //Get the llvm type
        llvm::Type* VarType = this->_VarType->GetLLVMType(Gen);
        if (VarType == NULL) {
            throw std::logic_error("Define variables with unknown type.");
            return MyValue();
        }
        if (VarType->isVoidTy()) {
            throw std::logic_error("Cannot define \"void\" variables.");
            return MyValue();
        }

        //Create variables one by one.
        for (auto& NewVar : *(this->_VarList)) {
            //Determine whether the declaration is inside a function.
            //If so, create an alloca;
            //Otherwise, create a global variable.
            if (Gen.GetCurFunction()) {
                //Create an alloca.
                bool isInnerConst = false;
                if (this->_VarType->isPointerType()) {
                    isInnerConst = ((PointerType*)this->_VarType)->isInnerConst();
                }
                auto Alloca = CreateEntryBlockAlloca(Gen.GetCurFunction()->LLVMFunc, NewVar->_Name, VarType);
                auto MyVal = new MyValue(Alloca, "", this->_VarType->_isConst || this->_VarType->isArrayType(), isInnerConst);
                if (!Gen.AddVariable(NewVar->_Name, MyVal)) {
                    throw std::logic_error("Naming conflict or redefinition for local variable: " + NewVar->_Name);
                    Alloca->eraseFromParent();
                    delete MyVal;
                    return MyValue();
                }
                //Assign the initial value by "store" instruction.
                if (NewVar->_InitialExpr) {
                    if (VarType->isArrayTy()) {
                        throw std::logic_error("Initialize array variable \"" + NewVar->_Name + "\", which is not allowed.");
                        return MyValue();
                    }
                    llvm::Value* Initializer = NULL;
                    MyValue InitialMyVal = NewVar->_InitialExpr->codegen(Gen);
                    if (InitialMyVal.IsInnerConstPointer && !isInnerConst) {
                        throw std::logic_error("Inner-const pointer cannot initialize a non-inner-const pointer.");
                        return MyValue();
                    }
                    Initializer = TypeCasting(InitialMyVal, VarType, Gen);
                    if (Initializer == NULL) {
                        throw std::logic_error("Initialize variable \"" + NewVar->_Name + "\" with value of conflict type.");
                        return MyValue();
                    }
                    GlobalBuilder.CreateStore(Initializer, Alloca);
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
                        return MyValue();
                    }
                    //Global variable must be initialized (if any) by a constant.

                    MyValue InitialMyVal = NewVar->_InitialExpr->codegen(Gen);
                    if (InitialMyVal.IsInnerConstPointer && !isInnerConst) {
                        throw std::logic_error("Inner-const pointer cannot initialize a non-inner-const pointer.");
                        return MyValue();
                    }

                    //save the insertion point
                    llvm::BasicBlock* SaveBB = GlobalBuilder.GetInsertBlock();
                    GlobalBuilder.SetInsertPoint(Gen.GetEmptyBB());

                    llvm::Value* InitialExpr = TypeCasting(NewVar->_InitialExpr->codegen(Gen), VarType, Gen);
                    //EmptyBB is used for detect whether the initial value is a constant!
                    if (GlobalBuilder.GetInsertBlock()->size() != 0) {
                        throw std::logic_error("Initialize global variable " + NewVar->_Name + " with non-constant value.");
                        return MyValue();
                    }
                    if (InitialExpr == NULL) {
                        throw std::logic_error("Initialize variable " + NewVar->_Name + " with value of conflict type.");
                        return MyValue();
                    }

                    //retore the insertion point
                    GlobalBuilder.SetInsertPoint(SaveBB);
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
                auto MyVal = new MyValue(Alloca, "", this->_VarType->_isConst || this->_VarType->isArrayType(), isInnerConst);
                if (!Gen.AddVariable(NewVar->_Name, MyVal)) {
                    throw std::logic_error("Naming conflict or redefinition for global variable: " + NewVar->_Name);
                    Alloca->eraseFromParent();
                    delete MyVal;
                    return MyValue();
                }
            }
        }
        return MyValue();
    }

    //Type declaration
    MyValue TypeDecl::codegen(CodeGenerator& Gen) {
        //Add an item to the current typedef symbol table
        //If an old value exists (i.e., conflict), raise an error
        llvm::Type* LLVMType;
        //For struct or union types, firstly we just need to get an opaque struct type
        if (this->_VarType->isStructType())
            LLVMType = ((ast::StructType*)this->_VarType)->GenerateStructTypeHead(Gen, this->_Alias);
        else
            LLVMType = this->_VarType->GetLLVMType(Gen);
        if (!LLVMType) {
            throw std::logic_error("Typedef " + this->_Alias + " using undefined types.");
            return MyValue();
        }

        FieldDecls* Fields = NULL;
        if (this->_VarType->isStructType())
            Fields = ((ast::StructType*)this->_VarType)->_StructBody;
        MyType* MyTy = new MyType(LLVMType, Fields);
        if (!Gen.AddType(this->_Alias, MyTy)) {
            throw std::logic_error("Naming conflict or redefinition for type: " + this->_Alias);
            return MyValue();
        }
        //For struct or union types, we need to generate its body
        if (this->_VarType->isStructType())
            ((ast::StructType*)this->_VarType)->GenerateStructTypeBody(Gen);
        return MyValue();
    }

    //Built-in type
    llvm::Type* BuiltInType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        switch (this->_Type) {
        case _Bool: this->_LLVMType = GlobalBuilder.getInt1Ty(); break;
        case _Short: this->_LLVMType = GlobalBuilder.getInt16Ty(); break;
        case _Int: this->_LLVMType = GlobalBuilder.getInt32Ty(); break;
        case _Long: this->_LLVMType = GlobalBuilder.getInt64Ty(); break;
        case _Char: this->_LLVMType = GlobalBuilder.getInt8Ty(); break;
        case _Float: this->_LLVMType = GlobalBuilder.getFloatTy(); break;
        case _Double: this->_LLVMType = GlobalBuilder.getDoubleTy(); break;
        case _Void: this->_LLVMType = GlobalBuilder.getVoidTy(); break;
        default: break;
        }
        return this->_LLVMType;
    }

    //Defined type. Use this class when only an identifier is given.
    llvm::Type* DefinedType::GetLLVMType(CodeGenerator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        this->_LLVMType = Gen.FindType(this->_Name)->LLVMType;
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
        this->GenerateStructTypeHead(Gen);
        return this->GenerateStructTypeBody(Gen);
    }
    llvm::Type* StructType::GenerateStructTypeHead(CodeGenerator& Gen, const std::string& __Name) {
        //Firstly, generate an empty identified struct type
        auto LLVMType = llvm::StructType::create(GlobalContext, "struct." + __Name);
        //Add to the struct table
        Gen.AddStructType(LLVMType, this);
        return this->_LLVMType = LLVMType;
    }
    llvm::Type* StructType::GenerateStructTypeBody(CodeGenerator& Gen) {
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
    MyValue Block::codegen(CodeGenerator& Gen) {
        Gen.PushSymbolTable();
        //Generate the statements in Block, one by one.
        for (auto& Stmt : *(this->_Content))
            //If the current block already has a terminator,
            //i.e. a "break" statement is generated, stop;
            //Otherwise, continue generating.
            if (GlobalBuilder.GetInsertBlock()->getTerminator())
                break;
            else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
                Stmt->codegen(Gen);
        Gen.PopSymbolTable();
        return MyValue();
    }

    //If statement
    MyValue IfStmt::codegen(CodeGenerator& Gen) {
        //Evaluate condition
        //Since we don't allow variable declarations in if-condition (because we only allow expressions there),
        //we don't need to push a symbol table
        MyValue Condition = this->_Condition->codegen(Gen);
        //Cast the type to i1
        if (!(Condition.Value = Cast2I1(Condition.Value))) {
            throw std::logic_error("The condition value of if-statement must be either an integer, or a floating-point number, or a pointer.");
            return MyValue();
        }
        //Create "Then", "Else" and "Merge" block
        llvm::Function* CurrentFunc = Gen.GetCurFunction()->LLVMFunc;
        llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(GlobalContext, "Then");
        llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(GlobalContext, "Else");
        llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(GlobalContext, "Merge");
        //Create a branch instruction corresponding to this if statement
        GlobalBuilder.CreateCondBr(Condition.Value, ThenBB, ElseBB);
        //Generate code in the "Then" block
        CurrentFunc->getBasicBlockList().push_back(ThenBB);

        GlobalBuilder.SetInsertPoint(ThenBB);
        if (this->_Then) {
            Gen.PushSymbolTable();
            this->_Then->codegen(Gen);
            Gen.PopSymbolTable();
        }
        TerminateBlockByBr(MergeBB);
        //Generate code in the "Else" block
        CurrentFunc->getBasicBlockList().push_back(ElseBB);
        GlobalBuilder.SetInsertPoint(ElseBB);
        if (this->_Else) {
            Gen.PushSymbolTable();
            this->_Else->codegen(Gen);
            Gen.PopSymbolTable();
        }
        TerminateBlockByBr(MergeBB);
        //Finish "Merge" block
        if (MergeBB->hasNPredecessorsOrMore(1)) {
            CurrentFunc->getBasicBlockList().push_back(MergeBB);
            GlobalBuilder.SetInsertPoint(MergeBB);
        }
        return MyValue();
    }

    //For statement
    MyValue ForStmt::codegen(CodeGenerator& Gen) {
        //Create "ForCond", "ForLoop", "ForTail" and "ForEnd"
        llvm::Function* CurrentFunc = Gen.GetCurFunction()->LLVMFunc;
        llvm::BasicBlock* ForCondBB = llvm::BasicBlock::Create(GlobalContext, "ForCond");
        llvm::BasicBlock* ForLoopBB = llvm::BasicBlock::Create(GlobalContext, "ForLoop");
        llvm::BasicBlock* ForTailBB = llvm::BasicBlock::Create(GlobalContext, "ForTail");
        llvm::BasicBlock* ForEndBB = llvm::BasicBlock::Create(GlobalContext, "ForEnd");
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
        GlobalBuilder.SetInsertPoint(ForCondBB);
        if (this->_Condition) {
            //If it has a loop condition, evaluate it (cast the type to i1 if necessary).
            MyValue Condition = this->_Condition->codegen(Gen);
            if (!(Condition.Value = Cast2I1(Condition.Value))) {
                throw std::logic_error("The condition value of for-statement must be either an integer, or a floating-point number, or a pointer.");
                return MyValue();
            }
            GlobalBuilder.CreateCondBr(Condition.Value, ForLoopBB, ForEndBB);
        }
        else {
            //Otherwise, it is an unconditional loop condition (always returns true)
            GlobalBuilder.CreateBr(ForLoopBB);
        }
        //Generate code in the "ForLoop" block
        CurrentFunc->getBasicBlockList().push_back(ForLoopBB);
        GlobalBuilder.SetInsertPoint(ForLoopBB);
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
        GlobalBuilder.SetInsertPoint(ForTailBB);
        if (this->_Tail)
            this->_Tail->codegen(Gen);
        GlobalBuilder.CreateBr(ForCondBB);
        //Finish "ForEnd" block
        CurrentFunc->getBasicBlockList().push_back(ForEndBB);
        GlobalBuilder.SetInsertPoint(ForEndBB);
        if (this->_Initial) {
            Gen.PopSymbolTable();
        }
        return MyValue();
    }

    //Switch statement
    MyValue SwitchStmt::codegen(CodeGenerator& Gen) {
        llvm::Function* CurrentFunc = Gen.GetCurFunction()->LLVMFunc;
        //Evaluate condition
        //Since we don't allow variable declarations in switch-matcher (because we only allow expressions there),
        //we don't need to push a symbol table.
        MyValue Matcher = this->_Matcher->codegen(Gen);
        //Create one block for each case statement.
        std::vector<llvm::BasicBlock*> CaseBB;
        for (int i = 0; i < this->_CaseList->size(); i++)
            CaseBB.push_back(llvm::BasicBlock::Create(GlobalContext, "Case" + std::to_string(i)));
        //Create an extra block for SwitchEnd
        CaseBB.push_back(llvm::BasicBlock::Create(GlobalContext, "SwitchEnd"));
        //Create one block for each comparison.
        std::vector<llvm::BasicBlock*> ComparisonBB;
        ComparisonBB.push_back(GlobalBuilder.GetInsertBlock());
        for (int i = 1; i < this->_CaseList->size(); i++)
            ComparisonBB.push_back(llvm::BasicBlock::Create(GlobalContext, "Comparison" + std::to_string(i)));
        ComparisonBB.push_back(CaseBB.back());
        //Generate branches
        for (int i = 0; i < this->_CaseList->size(); i++) {
            if (i) {
                CurrentFunc->getBasicBlockList().push_back(ComparisonBB[i]);
                GlobalBuilder.SetInsertPoint(ComparisonBB[i]);
            }
            if (this->_CaseList->at(i)->_Condition)	//Have condition
                GlobalBuilder.CreateCondBr(
                    CreateCmpEQ(Matcher.Value, this->_CaseList->at(i)->_Condition->codegen(Gen).Value),
                    CaseBB[i],
                    ComparisonBB[i + 1]
                );
            else									//Default
                GlobalBuilder.CreateBr(CaseBB[i]);
        }
        //Generate code for each case statement
        Gen.PushSymbolTable();
        for (int i = 0; i < this->_CaseList->size(); i++) {
            CurrentFunc->getBasicBlockList().push_back(CaseBB[i]);
            GlobalBuilder.SetInsertPoint(CaseBB[i]);
            Gen.EnterLoop(CaseBB[i + 1], CaseBB.back());
            this->_CaseList->at(i)->codegen(Gen);
            Gen.LeaveLoop();
        }
        Gen.PopSymbolTable();
        //Finish "SwitchEnd" block
        if (CaseBB.back()->hasNPredecessorsOrMore(1)) {
            CurrentFunc->getBasicBlockList().push_back(CaseBB.back());
            GlobalBuilder.SetInsertPoint(CaseBB.back());
        }
        return MyValue();
    }

    //Case statement in switch statement
    MyValue CaseStmt::codegen(CodeGenerator& Gen) {
        //Generate the statements, one by one.
        for (auto& Stmt : *(this->_Content))
            //If the current block already has a terminator,
            //i.e. a "break" statement is generated, stop;
            //Otherwise, continue generating.
            if (GlobalBuilder.GetInsertBlock()->getTerminator())
                break;
            else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
                Stmt->codegen(Gen);
        //If not terminated, jump to the next case block
        TerminateBlockByBr(Gen.GetContinueBlock());
        return MyValue();
    }

    //Continue statement
    MyValue ContinueStmt::codegen(CodeGenerator& Gen) {
        llvm::BasicBlock* ContinueTarget = Gen.GetContinueBlock();
        if (ContinueTarget)
            GlobalBuilder.CreateBr(ContinueTarget);
        else
            throw std::logic_error("Continue statement should only be used in loops or switch statements.");
        return MyValue();
    }

    //Break statement
    MyValue BreakStmt::codegen(CodeGenerator& Gen) {
        llvm::BasicBlock* BreakTarget = Gen.GetBreakBlock();
        if (BreakTarget)
            GlobalBuilder.CreateBr(BreakTarget);
        else
            throw std::logic_error("Break statement should only be used in loops or switch statements.");
        return MyValue();
    }

    //Return statement
    MyValue ReturnStmt::codegen(CodeGenerator& Gen) {
        llvm::Function* Func = Gen.GetCurFunction()->LLVMFunc;
        if (!Func) {
            throw std::logic_error("Return statement should only be used in function bodies.");
            return MyValue();
        }
        if (this->_RetVal == NULL) {
            if (Func->getReturnType()->isVoidTy())
                GlobalBuilder.CreateRetVoid();
            else {
                throw std::logic_error("Expected an expression after return statement.");
                return MyValue();
            }
        }
        else {
            auto MyVal = this->_RetVal->codegen(Gen);
            auto RetType = Gen.GetCurFunction()->RetType;
            auto RetLLVMType = Func->getReturnType();

            if (MyVal.IsInnerConstPointer &&
                !(RetType->isPointerType() && ((PointerType*)(RetType))->isInnerConst())) {
                throw std::logic_error("Cannot return an inner-const pointer as non-inner-const pointer type.");
                return MyValue();
            }

            MyValue RetVal = MyValue(TypeCasting(MyVal, RetLLVMType, Gen));
            if (!RetVal.Value) {
                throw std::logic_error("The type of return value doesn't match and cannot be cast to the return type.");
                return MyValue();
            }
            GlobalBuilder.CreateRet(RetVal.Value);
        }
        return MyValue();
    }

    //Subscript, e.g. a[10], b[2][3]
    MyValue Subscript::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue Subscript::codegenPtr(CodeGenerator& Gen) {
        //Get the pointer
        MyValue ArrayPtr = this->_Array->codegen(Gen);
        if (!ArrayPtr.Value->getType()->isPointerTy()) {
            throw std::logic_error("Subscript operator \"[]\" must be applied to pointers or arrays.");
            return MyValue();
        }
        //Get the index value
        MyValue Subspt = this->_Index->codegen(Gen);
        if (!(Subspt.Value->getType()->isIntegerTy())) {
            throw std::logic_error("Subscription should be an integer.");
            return MyValue();
        }
        //Return pointer addition
        return MyValue(CreateAdd(ArrayPtr, Subspt, Gen), ArrayPtr.Name, ArrayPtr.IsConst,
            ArrayPtr.IsInnerConstPointer, ArrayPtr.IsPointingToInnerConst);
    }

    //Operator sizeof() in C
    MyValue SizeOf::codegen(CodeGenerator& Gen) {
        if (this->_Arg1)//Expression
            return MyValue(GlobalBuilder.getInt64(Gen.GetTypeSize(this->_Arg1->codegen(Gen).Value->getType())));
        else if (this->_Arg2)//VarType
            return MyValue(GlobalBuilder.getInt64(Gen.GetTypeSize(this->_Arg2->GetLLVMType(Gen))));
        else {//Single identifier
            llvm::Type* Type = Gen.FindType(this->_Arg3)->LLVMType;
            if (Type) {
                this->_Arg2 = new DefinedType(this->_Arg3);
                return MyValue(GlobalBuilder.getInt64(Gen.GetTypeSize(Type)));
            }
            const MyValue* VarPtr = Gen.FindVariable(this->_Arg3);
            if (VarPtr) {
                this->_Arg1 = new Variable(this->_Arg3);
                return MyValue(GlobalBuilder.getInt64(Gen.GetTypeSize(VarPtr->Value->getType()->getNonOpaquePointerElementType())));
            }
            throw std::logic_error(this->_Arg3 + " is neither a type nor a variable.");
            return MyValue();
        }
    }
    MyValue SizeOf::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Sizeof() only returns right-values.");
        return MyValue();
    }

    //Function call
    MyValue FunctionCall::codegen(CodeGenerator& Gen) {
        //Get the function. Throw exception if the function doesn't exist.
        ast::MyFunction* MyFunc = Gen.FindFunction(this->_FuncName);
        auto curMyFunc = Gen.GetCurFunction();
        if (MyFunc == NULL) {
            throw std::domain_error(this->_FuncName + " is not a defined function.");
            return MyValue();
        }
        llvm::Function* Func = MyFunc->LLVMFunc;
        bool HasBaseType = !MyFunc->TypeName.empty();
        auto Args = MyFunc->Args;

        //Check the number of args. If Func took a different number of args, reject.
        if (Func->isVarArg() && this->_ArgList->size() + HasBaseType < Func->arg_size() ||
            !Func->isVarArg() && this->_ArgList->size() + HasBaseType != Func->arg_size()) {
            throw std::invalid_argument("Args doesn't match when calling function " + this->_FuncName +
                ". Expected " + std::to_string(Func->arg_size() - HasBaseType) + ", got " + std::to_string(this->_ArgList->size()));
            return MyValue();
        }

        std::vector<llvm::Value*> ArgList;

        if (HasBaseType) {
            auto BaseType = Func->arg_begin()->getType();
            if (this->_StructRef) {
                MyValue StructPtr = this->_StructRef->codegenPtr(Gen);
                if (!StructPtr.Value->getType()->isPointerTy()) {
                    throw std::invalid_argument("Expected a struct reference when calling method " + this->_FuncName + ".");
                    return MyValue();
                }
                if (StructPtr.Value->getType() != BaseType) {
                    throw std::invalid_argument("Expected a struct reference of type " + MyFunc->TypeName + " when calling method " + this->_FuncName + ".");
                    return MyValue();
                }
                StructPtr.Value = TypeCasting(StructPtr, BaseType, Gen);
                ArgList.push_back(StructPtr.Value);
            }
            else if (this->_StructPtr) {
                MyValue StructPtr = this->_StructPtr->codegen(Gen);
                if (!StructPtr.Value->getType()->isPointerTy()) {
                    throw std::invalid_argument("Expected a struct pointer when calling method " + this->_FuncName + ".");
                    return MyValue();
                }
                if (StructPtr.Value->getType() != BaseType) {
                    throw std::invalid_argument("Expected a struct pointer of type " + MyFunc->TypeName + " when calling method " + this->_FuncName + ".");
                    return MyValue();
                }
                StructPtr.Value = TypeCasting(StructPtr, BaseType, Gen);
                ArgList.push_back(StructPtr.Value);
            }
            else {
                throw std::invalid_argument("Expected a struct reference or pointer when calling method " + this->_FuncName + ".");
                return MyValue();
            }
        }

        size_t Index = 0;
        auto ArgIter = Func->arg_begin();
        if (HasBaseType) {
            ArgIter++;
        }
        for (; ArgIter < Func->arg_end(); ArgIter++, Index++) {
            MyValue Arg = this->_ArgList->at(Index)->codegen(Gen);
            auto ArgType = Args->at(Index)->_VarType;

            if (Arg.IsInnerConstPointer && ArgType->isPointerType() && !((PointerType*)ArgType)->isInnerConst()) {
                throw std::invalid_argument("Cannot pass an inner-const pointer to a non-inner-const pointer argument.");
                return MyValue();
            }
            Arg.Value = TypeCasting(Arg, ArgIter->getType(), Gen);
            if (Arg.Value == NULL) {
                throw std::invalid_argument(std::to_string(Index) + "-th arg type doesn't match when calling function " + this->_FuncName + ".");
                return MyValue();
            }
            ArgList.push_back(Arg.Value);
        }
        //Continue to push arguments if this function takes a variable number of arguments
        //According to the C standard, bool/char/short should be extended to int, and float should be extended to double
        if (Func->isVarArg())
            for (; Index < this->_ArgList->size(); Index++) {
                MyValue Arg = this->_ArgList->at(Index)->codegen(Gen);
                if (Arg.Value->getType()->isIntegerTy())
                    Arg.Value = TypeUpgrading(Arg.Value, GlobalBuilder.getInt32Ty());
                else if (Arg.Value->getType()->isFloatingPointTy())
                    Arg.Value = TypeUpgrading(Arg.Value, GlobalBuilder.getDoubleTy());
                ArgList.push_back(Arg.Value);
            }
        //Create function call.
        auto RetType = MyFunc->RetType;
        bool isInnerConst = false;
        if (RetType->isPointerType()) {
            isInnerConst = ((PointerType*)RetType)->isInnerConst();
        }
        return MyValue(GlobalBuilder.CreateCall(Func, ArgList), "", RetType->_isConst, isInnerConst);
    }
    MyValue FunctionCall::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Function call only returns right-values.");
        return MyValue();
    }

    //Structure reference, e.g. a.x, a.y
    MyValue StructReference::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue StructReference::codegenPtr(CodeGenerator& Gen) {
        MyValue StructPtr = this->_Struct->codegenPtr(Gen);
        if (!StructPtr.Value->getType()->isPointerTy() || !StructPtr.Value->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Reference operator \".\" must be apply to structs.");
            return MyValue();
        }
        //Since C language uses name instead of index to fetch the element inside a struct,
        //we need to fetch the ast::StructType* instance according to the llvm::StructType* instance.
        //And it is the same with union types.
        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr.Value->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
                return MyValue();
            }
            std::vector<llvm::Value*> Indices;
            Indices.push_back(GlobalBuilder.getInt32(0));
            Indices.push_back(GlobalBuilder.getInt32(MemIndex));

            auto memType = StructType->_StructBody->at(MemIndex)->_VarType;
            bool isConst = memType->_isConst || StructPtr.IsInnerConstPointer;
            bool isInnerConst = false;
            if (memType->isPointerType()) {
                isInnerConst = ((PointerType*)memType)->isInnerConst();
            }
            return MyValue(GlobalBuilder.CreateGEP(StructPtr.Value->getType()->getNonOpaquePointerElementType(), StructPtr.Value, Indices)
                , "", true, isConst, isInnerConst);
        }
        return MyValue();
    }

    //Structure dereference, e.g. a->x, a->y
    MyValue StructDereference::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue StructDereference::codegenPtr(CodeGenerator& Gen) {
        MyValue StructPtr = this->_StructPtr->codegen(Gen);
        if (!StructPtr.Value->getType()->isPointerTy() || !StructPtr.Value->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Dereference operator \"->\" must be apply to structs.");
            return MyValue();
        }
        //Since C language uses name instead of index to fetch the element inside a struct,
        //we need to fetch the ast::StructType* instance according to the llvm::StructType* instance.
        //And it is the same with union types.
        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr.Value->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
                return MyValue();
            }

            std::vector<llvm::Value*> Indices;
            Indices.push_back(GlobalBuilder.getInt32(0));
            Indices.push_back(GlobalBuilder.getInt32(MemIndex));

            auto memType = StructType->_StructBody->at(MemIndex)->_VarType;
            bool isConst = memType->_isConst || StructPtr.IsInnerConstPointer;
            bool isInnerConst = false;
            if (memType->isPointerType()) {
                isInnerConst = ((PointerType*)memType)->isInnerConst();
            }
            return MyValue(GlobalBuilder.CreateGEP(StructPtr.Value->getType()->getNonOpaquePointerElementType(), StructPtr.Value, Indices)
                , "", true, isConst, isInnerConst);
        }
        return MyValue();
    }

    //Unary plus, e.g. +i, +j, +123
    MyValue UnaryPlus::codegen(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand.Value->getType()->isIntegerTy() ||
            Operand.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary plus must be applied to integers or floating-point numbers.");
        return MyValue(Operand);
    }
    MyValue UnaryPlus::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Unary plus only returns right-values.");
        return MyValue();
    }

    //Unary minus, e.g. -i, -j, -123
    MyValue UnaryMinus::codegen(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand.Value->getType()->isIntegerTy() ||
            Operand.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary minus must be applied to integers or floating-point numbers.");
        if (Operand.Value->getType()->isIntegerTy())
            return MyValue(GlobalBuilder.CreateNeg(Operand.Value), Operand.Name, Operand.IsConst, Operand.IsInnerConstPointer);
        else
            return MyValue(GlobalBuilder.CreateFNeg(Operand.Value), Operand.Name, Operand.IsConst, Operand.IsInnerConstPointer);
    }
    MyValue UnaryMinus::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Unary minus only returns right-values.");
        return MyValue();
    }

    //Type cast, e.g. (float)n, (int)1.0
    MyValue TypeCast::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->_Operand->codegen(Gen);
        if (MyVal.IsInnerConstPointer
            && !(this->_VarType->isPointerType() && ((PointerType*)(this->_VarType))->isInnerConst())) {
            throw std::logic_error("Cannot type cast an inner-const pointer to a non-inner-const pointer.");
            return MyValue();
        }

        MyValue Ret = MyValue(TypeCasting(MyVal, this->_VarType->GetLLVMType(Gen), Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
        if (Ret.Value == NULL) {
            throw std::logic_error("Unable to do type casting.");
            return MyValue();
        }
        return Ret;
    }
    MyValue TypeCast::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Type casting only returns right-values.");
        return MyValue();
    }

    //Prefix increment, e.g. ++i
    MyValue PrefixInc::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue PrefixInc::codegenPtr(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do prefix increment.");
            return MyValue();
        }
        MyValue OpValue = MyValue(GlobalBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Prefix increment must be applied to integers or floating-point numbers.");
        MyValue OpValuePlus = MyValue(CreateAdd(OpValue, MyValue(GlobalBuilder.getInt1(1)), Gen));
        GlobalBuilder.CreateStore(OpValuePlus.Value, Operand.Value);
        return MyValue(Operand);
    }

    //Postfix increment, e.g. i++
    MyValue PostfixInc::codegen(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do postfix increment.");
            return MyValue();
        }
        MyValue OpValue = MyValue(GlobalBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Postfix increment must be applied to integers or floating-point numbers");
        MyValue OpValuePlus = MyValue(CreateAdd(OpValue, MyValue(GlobalBuilder.getInt1(1)), Gen));
        GlobalBuilder.CreateStore(OpValuePlus.Value, Operand.Value);
        return MyValue(OpValue.Value, Operand.Name, Operand.IsInnerConstPointer, Operand.IsPointingToInnerConst);
    }
    MyValue PostfixInc::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Postfix increment only returns right-values.");
        return MyValue();
    }

    //Prefix decrement, e.g. --i
    MyValue PrefixDec::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue PrefixDec::codegenPtr(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do prefix decrement.");
            return MyValue();
        }
        MyValue OpValue = MyValue(GlobalBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Prefix decrement must be applied to integers or floating-point numbers.");
        MyValue OpValueMinus = MyValue(CreateSub(OpValue, MyValue(GlobalBuilder.getInt1(1)), Gen));
        GlobalBuilder.CreateStore(OpValueMinus.Value, Operand.Value);
        return MyValue(Operand);
    }

    //Postfix decrement, e.g. i--
    MyValue PostfixDec::codegen(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do postfix decrement.");
            return MyValue();
        }
        MyValue OpValue = MyValue(GlobalBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Postfix decrement must be applied to integers or floating-point numbers");
        MyValue OpValueMinus = MyValue(CreateSub(OpValue, MyValue(GlobalBuilder.getInt1(1)), Gen));
        GlobalBuilder.CreateStore(OpValueMinus.Value, Operand.Value);
        return MyValue(OpValue.Value, Operand.Name, Operand.IsInnerConstPointer, Operand.IsPointingToInnerConst);
    }
    MyValue PostfixDec::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Postfix decrement only returns right-values.");
        return MyValue();
    }

    //Indirection, e.g. dptr(x)
    MyValue Indirection::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue Indirection::codegenPtr(CodeGenerator& Gen) {
        MyValue Ptr = this->_Operand->codegen(Gen);
        if (!Ptr.Value->getType()->isPointerTy()) {
            throw std::logic_error("Indirection operator \"dptr()\" only applies on pointers or arrays.");
            return MyValue();
        }
        return MyValue(Ptr);
    }

    //Fetch address, e.g. addr()
    MyValue AddressOf::codegen(CodeGenerator& Gen) {
        return MyValue(this->_Operand->codegenPtr(Gen));
    }
    MyValue AddressOf::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Address operator \"addr()\" only returns right-values.");
        return MyValue();
    }

    //Logic NOT, e.g. !x
    MyValue LogicNot::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(GlobalBuilder.CreateICmpEQ(Cast2I1(MyVal.Value), GlobalBuilder.getInt1(false)), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue LogicNot::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic NOT operator \"!\" only returns right-values.");
        return MyValue();
    }

    //Bitwise NOT, e.g. ~x
    MyValue BitwiseNot::codegen(CodeGenerator& Gen) {
        MyValue Operand = this->_Operand->codegen(Gen);
        if (!Operand.Value->getType()->isIntegerTy()) {
            throw std::logic_error("Bitwise NOT operator \"~\" must be applied to integers.");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateNot(Operand.Value), Operand.Name, Operand.IsConst, Operand.IsInnerConstPointer, Operand.IsPointingToInnerConst);
    }
    MyValue BitwiseNot::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise NOT operator \"~\" only returns right-values.");
        return MyValue();
    }

    //Division, e.g. x/y
    MyValue Division::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateDiv(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Division::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Division operator \"/\" only returns right-values.");
        return MyValue();
    }

    //Multiplication, e.g. x*y
    MyValue Multiplication::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateMul(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Multiplication::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Multiplication operator \"*\" only returns right-values.");
        return MyValue();
    }

    //Modulo, e.g. x%y
    MyValue Modulo::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateMod(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Modulo::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Modulo operator \"%\" only returns right-values.");
        return MyValue();
    }

    //Addition, e.g. x+y
    MyValue Addition::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAdd(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Addition::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Addition operator \"+\" only returns right-values.");
        return MyValue();
    }

    //Subtraction, e.g. x-y
    MyValue Subtraction::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateSub(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Subtraction::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Subtraction operator \"-\" only returns right-values.");
        return MyValue();
    }

    //LeftShift, e.g. x<<y
    MyValue LeftShift::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateShl(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue LeftShift::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Left shifting operator \"<<\" only returns right-values.");
        return MyValue();
    }

    //RightShift, e.g. x>>y
    MyValue RightShift::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateShr(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue RightShift::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Right shifting operator \">>\" only returns right-values.");
        return MyValue();
    }

    //LogicGT, e.g. x>y
    MyValue LogicGT::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSGT(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOGT(LeftOp.Value, RightOp.Value));
        }
        //Pointer compare
        if (LeftOp.Value->getType()->isPointerTy() && LeftOp.Value->getType() == RightOp.Value->getType()) {
            return MyValue(GlobalBuilder.CreateICmpUGT(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
            return MyValue(GlobalBuilder.CreateICmpUGT(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                TypeUpgrading(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isPointerTy()) {
            return MyValue(GlobalBuilder.CreateICmpUGT(
                TypeUpgrading(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \">\" using unsupported type combination.");
    }
    MyValue LogicGT::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \">\" only returns right-values.");
        return MyValue();
    }

    //LogicGE, e.g. x>=y
    MyValue LogicGE::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSGE(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOGE(LeftOp.Value, RightOp.Value));
        }
        //Pointer compare
        if (LeftOp.Value->getType()->isPointerTy() && LeftOp.Value->getType() == RightOp.Value->getType()) {
            return MyValue(GlobalBuilder.CreateICmpUGE(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
            return MyValue(GlobalBuilder.CreateICmpUGE(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                TypeUpgrading(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isPointerTy()) {
            return MyValue(GlobalBuilder.CreateICmpUGE(
                TypeUpgrading(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \">=\" using unsupported type combination.");
    }
    MyValue LogicGE::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \">=\" only returns right-values.");
        return MyValue();
    }

    //LogicLT, e.g. x<y
    MyValue LogicLT::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSLT(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOLT(LeftOp.Value, RightOp.Value));
        }
        //Pointer compare
        if (LeftOp.Value->getType()->isPointerTy() && LeftOp.Value->getType() == RightOp.Value->getType()) {
            return MyValue(GlobalBuilder.CreateICmpULT(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
            return MyValue(GlobalBuilder.CreateICmpULT(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                TypeUpgrading(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isPointerTy()) {
            return MyValue(GlobalBuilder.CreateICmpULT(
                TypeUpgrading(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \"<\" using unsupported type combination.");
    }
    MyValue LogicLT::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"<\" only returns right-values.");
        return MyValue();
    }

    //LogicLE, e.g. x<=y
    MyValue LogicLE::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSLE(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOLE(LeftOp.Value, RightOp.Value));
        }
        //Pointer compare
        if (LeftOp.Value->getType()->isPointerTy() && LeftOp.Value->getType() == RightOp.Value->getType()) {
            return MyValue(GlobalBuilder.CreateICmpULE(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
            return MyValue(GlobalBuilder.CreateICmpULE(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                TypeUpgrading(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isPointerTy()) {
            return MyValue(GlobalBuilder.CreateICmpULE(
                TypeUpgrading(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \"<=\" using unsupported type combination.");
    }
    MyValue LogicLE::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"<=\" only returns right-values.");
        return MyValue();
    }

    //LogicEQ, e.g. x==y
    MyValue LogicEQ::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateCmpEQ(LeftOp.Value, RightOp.Value));
    }
    MyValue LogicEQ::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"==\" only returns right-values.");
        return MyValue();
    }

    //LogicNEQ, e.g. x!=y
    MyValue LogicNEQ::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        //Arithmatic compare
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpNE(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpONE(LeftOp.Value, RightOp.Value));
        }
        //Pointer compare
        if (LeftOp.Value->getType()->isPointerTy() && LeftOp.Value->getType() == RightOp.Value->getType()) {
            return MyValue(GlobalBuilder.CreateICmpNE(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
            return MyValue(GlobalBuilder.CreateICmpNE(
                GlobalBuilder.CreatePtrToInt(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                TypeUpgrading(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        else if (LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isPointerTy()) {
            return MyValue(GlobalBuilder.CreateICmpNE(
                TypeUpgrading(LeftOp.Value, GlobalBuilder.getInt64Ty()),
                GlobalBuilder.CreatePtrToInt(RightOp.Value, GlobalBuilder.getInt64Ty())
            ));
        }
        throw std::domain_error("Comparison \"!=\" using unsupported type combination.");
    }
    MyValue LogicNEQ::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Comparison operator \"!=\" only returns right-values.");
        return MyValue();
    }

    //BitwiseAND, e.g. x&y
    MyValue BitwiseAND::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateBitwiseAND(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue BitwiseAND::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise AND operator \"&\" only returns right-values.");
        return MyValue();
    }

    //BitwiseXOR, e.g. x^y
    MyValue BitwiseXOR::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateBitwiseXOR(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue BitwiseXOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise XOR operator \"^\" only returns right-values.");
        return MyValue();
    }

    //BitwiseOR, e.g. x|y
    MyValue BitwiseOR::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateBitwiseOR(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue BitwiseOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Bitwise OR operator \"|\" only returns right-values.");
        return MyValue();
    }

    //LogicAND, e.g. x&&y
    MyValue LogicAND::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        LeftOp = MyValue(Cast2I1(LeftOp.Value));
        if (LeftOp.Value == NULL) {
            throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
            return MyValue();
        }
        RightOp = MyValue(Cast2I1(RightOp.Value));
        if (RightOp.Value == NULL) {
            throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateLogicalAnd(LeftOp.Value, RightOp.Value));
    }
    MyValue LogicAND::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic AND operator \"&&\" only returns right-values.");
        return MyValue();
    }

    //LogicOR, e.g. x||y
    MyValue LogicOR::codegen(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        LeftOp = MyValue(Cast2I1(LeftOp.Value));
        if (LeftOp.Value == NULL) {
            throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
            return MyValue();
        }
        RightOp = MyValue(Cast2I1(RightOp.Value));
        if (RightOp.Value == NULL) {
            throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateLogicalOr(LeftOp.Value, RightOp.Value));
    }
    MyValue LogicOR::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Logic OR operator \"||\" only returns right-values.");
        return MyValue();
    }

    //TernaryCondition, e.g. (cond)?x:y
    MyValue TernaryCondition::codegen(CodeGenerator& Gen) {
        MyValue Condition = MyValue(Cast2I1(this->_Condition->codegen(Gen).Value));
        if (Condition.Value == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
            return MyValue();
        }
        MyValue True = this->_Then->codegen(Gen);
        MyValue False = this->_Else->codegen(Gen);
        if (True.Value->getType() == False.Value->getType() || TypeUpgrading(True.Value, False.Value)) {
            return MyValue(GlobalBuilder.CreateSelect(Condition.Value, True.Value, False.Value), True.Name, True.IsConst,
                True.IsInnerConstPointer || False.IsInnerConstPointer, True.IsPointingToInnerConst);
        }
        else {
            throw std::domain_error("Thernary operand \" ? : \" using unsupported type combination.");
            return MyValue();
        }
    }
    MyValue TernaryCondition::codegenPtr(CodeGenerator& Gen) {
        MyValue Condition = MyValue(Cast2I1(this->_Condition->codegen(Gen).Value));
        if (Condition.Value == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
            return MyValue();
        }
        MyValue True = this->_Then->codegenPtr(Gen);
        MyValue False = this->_Else->codegenPtr(Gen);
        if (True.Value->getType() != False.Value->getType()) {
            throw std::domain_error("When using thernary expressions \" ? : \" as left-values, the latter two operands must be of the same type.");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateSelect(Condition.Value, True.Value, False.Value));
    }

    //DirectAssign, e.g. x=y
    MyValue DirectAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue DirectAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //DivAssign, e.g. x/=y
    MyValue DivAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue DivAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);

        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateDiv(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //MulAssign, e.g. x*=y
    MyValue MulAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue MulAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateMul(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //ModAssign, e.g. x%=y
    MyValue ModAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue ModAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateMod(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //AddAssign, e.g. x+=y
    MyValue AddAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue AddAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateAdd(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //SubAssign, e.g. x-=y
    MyValue SubAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue SubAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateSub(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //SHLAssign, e.g. x<<=y
    MyValue SHLAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue SHLAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateShl(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //SHRAssign, e.g. x>>=y
    MyValue SHRAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue SHRAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateShr(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //BitwiseANDAssign, e.g. x&=y
    MyValue BitwiseANDAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue BitwiseANDAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateBitwiseAND(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //BitwiseXORAssign, e.g. x^=y
    MyValue BitwiseXORAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue BitwiseXORAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateBitwiseXOR(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //BitwiseORAssign, e.g. x|=y
    MyValue BitwiseORAssign::codegen(CodeGenerator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue BitwiseORAssign::codegenPtr(CodeGenerator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp,
            MyValue(CreateBitwiseOR(
                MyValue(GlobalBuilder.CreateLoad(
                    LeftOp.Value->getType()->getNonOpaquePointerElementType(),
                    LeftOp.Value)),
                RightOp,
                Gen)),
            Gen
        ), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //Comma expression, e.g. a,b,c,1
    MyValue CommaExpr::codegen(CodeGenerator& Gen) {
        this->_LeftOp->codegen(Gen);
        return MyValue(this->_RightOp->codegen(Gen));
    }
    MyValue CommaExpr::codegenPtr(CodeGenerator& Gen) {
        this->_LeftOp->codegen(Gen);
        return MyValue(this->_RightOp->codegenPtr(Gen));
    }

    //Variable, e.g. x
    MyValue Variable::codegen(CodeGenerator& Gen) {
        const MyValue* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) {
            return MyValue(CreateLoad(VarPtr->Value, Gen), VarPtr->Name, VarPtr->IsConst, VarPtr->IsInnerConstPointer);
        }
        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a variable.");
        return MyValue();
    }
    MyValue Variable::codegenPtr(CodeGenerator& Gen) {
        const MyValue* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) return MyValue(VarPtr->Value, VarPtr->Name, true,
            VarPtr->IsConst, VarPtr->IsInnerConstPointer);

        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a variable");
        return MyValue();
    }

    //Constant, e.g. 1, 1.0, 'c', true, false
    MyValue Constant::codegen(CodeGenerator& Gen) {
        switch (this->_Type) {
        case BuiltInType::TypeID::_Bool:
            return MyValue(GlobalBuilder.getInt1(this->_Bool), "", true, false, false);
        case BuiltInType::TypeID::_Char:
            return MyValue(GlobalBuilder.getInt8(this->_Character), "", true, false, false);
        case BuiltInType::TypeID::_Int:
        {
            auto ret = MyValue(GlobalBuilder.getInt32(this->_Integer), "", true, false, false);
            if (this->_Integer == 0) {
                ret.SetIsZeroConstant();
            }
            return ret;
        }
        case BuiltInType::TypeID::_Double:
            return MyValue(llvm::ConstantFP::get(GlobalBuilder.getDoubleTy(), this->_Real), "", true, false, false);
        default:
            throw std::logic_error("Unknown constant type.");
            return MyValue();
        }
    }
    MyValue Constant::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Constant is a right-value.");
        return MyValue();
    }

    //Global string, e.g. "123", "3\"124\t\n"
    MyValue GlobalString::codegen(CodeGenerator& Gen) {
        return MyValue(GlobalBuilder.CreateGlobalStringPtr(this->_Content.c_str()), "", true, false, false);
    }
    MyValue GlobalString::codegenPtr(CodeGenerator& Gen) {
        throw std::logic_error("Global string (constant) is a right-value.");
        return MyValue();
    }

    MyValue This::codegen(CodeGenerator& Gen) {
        const MyValue* ThisPtr = Gen.FindVariable("0this");
        if (ThisPtr) {
            return MyValue(CreateLoad(ThisPtr->Value, Gen), ThisPtr->Name, true, false);
        }
        throw std::logic_error("Using \"this\" but not in a member function.");
        return MyValue();
    }

    MyValue This::codegenPtr(CodeGenerator& Gen) {
        const MyValue* ThisPtr = Gen.FindVariable("0this");
        if (ThisPtr) {
            return MyValue(ThisPtr->Value, ThisPtr->Name, true, true);
        }
        throw std::logic_error("Using \"this\" but not in a member function.");
        return MyValue();
    }
}