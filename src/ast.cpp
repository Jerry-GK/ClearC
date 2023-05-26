#include "generator.hpp"
#include "llvm_util.hpp"

//namespace of ast
namespace ast {
    //Program
    MyValue Program::codegen(Generator& Gen) {
        for (auto Decl : *(this->_Decls))
            if (Decl)	//We allow empty-declaration which is represented by NULL pointer.
                Decl->codegen(Gen);
        return MyValue();
    }

    //Function declaration
    MyValue FuncDecl::codegen(Generator& Gen) {
        std::vector<llvm::Type*> ArgTypes;
        bool HasBaseType = !this->_TypeName.empty();

        //push a const ptr<Type> into ArgTypes if the function's TypeName is not empty
        if (HasBaseType) {
            MyType* MyTy = Gen.FindType(this->_TypeName);
            if (!MyTy) {
                throw std::logic_error("Declare function \"" + this->_FuncName + "\" belongs to unknown base type " + this->_TypeName);
                return MyValue();
            }

            auto LLVMType = MyTy->LLVMType;
            if (!LLVMType->isStructTy()) {
                throw std::logic_error("Declare function \"" + this->_FuncName + "\" whose base type " + this->_TypeName + " is not a known struct type");
                return MyValue();
            }
            auto LLVMPointerType = llvm::PointerType::get(LLVMType, 0U);
            ArgTypes.push_back(LLVMPointerType);
        }

        bool isVoidArgs = false;
        for (auto ArgType : *(this->_ArgList)) {
            if (ArgType->_VarType->_VarTy == VARTYPE_AUTO) {
                throw std::logic_error("Declare function \"" + this->_FuncName + "\" using auto type argument, which is not allowed");
                return MyValue();
            }
            llvm::Type* LLVMType = ArgType->_VarType->GetLLVMType(Gen);
            if (!LLVMType) {
                throw std::logic_error("Declare function \"" + this->_FuncName + "\" using unknown arg type(s)");
                return MyValue();
            }
            if (LLVMType->isVoidTy())
                isVoidArgs = true;
            if (LLVMType->isArrayTy())
            {
                throw std::logic_error("Declare function \"" + this->_FuncName + "\" with array type argument, which is not allowed");
                return MyValue();
            }
            ArgTypes.push_back(LLVMType);
        }

        if (this->_ArgList->size() != 1 && isVoidArgs) {
            throw std::logic_error("Declare function \"" + this->_FuncName + "\" which has invalid number of arguments with type \"void\"");
            return MyValue();
        }

        if (isVoidArgs)
            ArgTypes.pop_back();

        //Get return type
        if (this->_RetType->_VarTy == VARTYPE_AUTO)
        {
            throw std::logic_error("Declare function \"" + this->_FuncName + "\" using auto type return value, which is not allowed");
            return MyValue();
        }
        llvm::Type* RetType = this->_RetType->GetLLVMType(Gen);
        if (RetType->isArrayTy()) {
            throw std::logic_error("Declare function \"" + this->_FuncName + "\" which returns array type, which is not allowed");
            return MyValue();
        }
        //Get function type
        llvm::FunctionType* FuncType = llvm::FunctionType::get(RetType, ArgTypes, this->_ArgList->_DotsArg);

        //Create LLVM function
        llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::ExternalLinkage, this->_FuncName, Gen.Module);

        //function has been declared or defined
        if (Func->getName() != this->_FuncName) {
            //Delete the old one
            Func->eraseFromParent();
            Func = Gen.Module->getFunction(this->_FuncName);

            if (!this->_FuncBody)
            {
                throw std::logic_error("Redeclare function \"" + this->_FuncName + "\" which has been declared or defined");
                return MyValue();
            }
            //redefine
            if (!Func->empty()) {
                throw std::logic_error("Redefine function \"" + this->_FuncName + "\" which has been defined");
                return MyValue();
            }
            //define the function with conflict arg types with the previous declaration
            auto MyFunc = Gen.FindFunction(this->_FuncName);
            if (!MyFunc)
            {
                throw std::logic_error("Unexpected error when getting function \"" + this->_FuncName + "\"");
                return MyValue();
            }
            for (int i = 0;i < this->_ArgList->size();i++) {
                auto prevArgType = MyFunc->Args->at(i)->_VarType;
                auto curArgType = this->_ArgList->at(i)->_VarType;
                bool TypeSame = prevArgType->_LLVMType->getTypeID() == curArgType->_LLVMType->getTypeID();
                bool IsConstSame = prevArgType->_isConst == curArgType->_isConst;
                bool prevIsInnerConst = false;
                if (prevArgType->_VarTy == VARTYPE_POINTER) {
                    prevIsInnerConst = ((PointerType*)prevArgType)->isInnerConst();
                }
                bool curIsInnerConst = false;
                if (curArgType->_VarTy == VARTYPE_POINTER) {
                    curIsInnerConst = ((PointerType*)curArgType)->isInnerConst();
                }
                bool IsInnerConstSame = prevIsInnerConst == curIsInnerConst;

                if (!(TypeSame && IsConstSame && IsInnerConstSame)) {
                    throw std::logic_error("Define function \"" + this->_FuncName + "\" which has conflict arg type of the "
                        + std::to_string(i + 1) + "'th arg with the previous declaration");
                    return MyValue();
                }
            }
            //this "redundant" check is necessary for check more detailed type conflict
            if (Func->getFunctionType() != FuncType) {
                throw std::logic_error("Define function \"" + this->_FuncName + "\" which has conflict arg types with the previous declaration");
                return MyValue();
            }
            //(declare and) define a function which has been declared is allowed, ignore the second declaration
        }


        ast::MyFunction* MyFunc = new ast::MyFunction(Func, this->_ArgList, this->_RetType, this->_TypeName);
        auto ret = Gen.AddFunction(this->_FuncName, MyFunc); //add function into symbol table
        if (ret == ADDFUNC_NAMECONFLICT) {
            throw std::logic_error("Naming conflict for function \"" + this->_FuncName + "\"");
            return MyValue();
        }
        else if (ret == ADDFUNC_ERROR) {
            //unexpected error
            throw std::logic_error("Unexpected error while adding function \"" + this->_FuncName + "\"");
            return MyValue();
        }
        // ret == ADDFUNC_DECLARED is normal, ignore this redeclaration

        //generate body code if exists
        if (this->_FuncBody) {
            llvm::BasicBlock* FuncBlock = llvm::BasicBlock::Create(GlobalContext, "entry", Func);
            GlobalBuilder.SetInsertPoint(FuncBlock);
            Gen.PushSymbolTable();
            size_t Index = 0;
            auto ArgIter = Func->arg_begin();
            if (HasBaseType)
            {
                auto Alloca = CreateEntryBlockAlloca(Func, "0this", ArgIter->getType());
                auto MyVal = new MyValue(Alloca, "", true, false);
                GlobalBuilder.CreateStore(ArgIter, Alloca);
                //Add to the symbol table
                if (!Gen.AddVariable("0this", MyVal))
                {
                    throw std::logic_error("Cannot add \"this\" pointer in function \"" + this->_FuncName + "\"");
                    delete MyVal;
                    return MyValue();
                }
                ArgIter++;
            }
            for (; ArgIter < Func->arg_end(); ArgIter++, Index++) {
                auto ArgVarType = this->_ArgList->at(Index)->_VarType;
                bool isInnerConst = false;
                if (ArgVarType->_VarTy == VARTYPE_POINTER) {
                    isInnerConst = ((PointerType*)ArgVarType)->isInnerConst();
                }
                auto Alloca = CreateEntryBlockAlloca(Func, this->_ArgList->at(Index)->_Name, ArgIter->getType());
                auto MyVal = new MyValue(Alloca, "", ArgVarType->_isConst || ArgVarType->_VarTy == VARTYPE_ARRAY, isInnerConst);
                GlobalBuilder.CreateStore(ArgIter, Alloca);
                if (!Gen.AddVariable(this->_ArgList->at(Index)->_Name, MyVal))
                {
                    throw std::logic_error("Naming conflict or redefinition for local variable \""
                        + this->_ArgList->at(Index)->_Name + "\" in the args list of function \"" + this->_FuncName + "\"");
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
            Gen.PopSymbolTable();	//this is necessary

            return MyValue();
        }
        return MyValue();
    }

    //Function body
    MyValue FuncBody::codegen(Generator& Gen) {
        for (auto& Stmt : *(this->_Content))
            if (GlobalBuilder.GetInsertBlock()->getTerminator())
                break;
            else
                Stmt->codegen(Gen);
        //return statement is needed for all functions, including void functions.
        if (!GlobalBuilder.GetInsertBlock()->getTerminator()) {
            throw std::logic_error("A function does not have a return statement (void function needs return as well)");
            return MyValue();
        }
        return MyValue();
    }

    //Variable declaration
    MyValue VarDecl::codegen(Generator& Gen) {
        //Get the llvm type
        llvm::Type* VarType = this->_VarType->GetLLVMType(Gen);
        if (this->_VarType->_VarTy != VARTYPE_AUTO && VarType == NULL) {
            throw std::logic_error("Define variable with unknown type");
            return MyValue();
        }
        if (VarType != NULL && VarType->isVoidTy()) {
            throw std::logic_error("Cannot define \"void\" variable");
            return MyValue();
        }

        bool autoDetermined = false;
        //Create variables one by one.
        for (auto& NewVar : *(this->_VarList)) {
            MyValue InitialMyVal;
            bool isInnerConst = false;
            if (NewVar->_InitialExpr) {
                InitialMyVal = NewVar->_InitialExpr->codegen(Gen);
            }
            if (this->_VarType->_VarTy == VARTYPE_AUTO) {
                if (!NewVar->_InitialExpr) {
                    throw std::logic_error("Auto type variable \"" + NewVar->_Name + "\" must be initialized");
                    return MyValue();
                }
                if (!autoDetermined) {
                    //this->_VarType->_isConst = InitialMyVal.IsConst; //not inherit const
                    isInnerConst = InitialMyVal.IsInnerConstPointer; //but inherit inner-const
                    VarType = InitialMyVal.Value->getType();
                    autoDetermined = true;
                }
            }
            if (Gen.GetCurFunction()) {
                //Create an alloca.
                if (this->_VarType->_VarTy == VARTYPE_POINTER) {
                    isInnerConst = ((PointerType*)this->_VarType)->isInnerConst();
                }
                auto Alloca = CreateEntryBlockAlloca(Gen.GetCurFunction()->LLVMFunc, NewVar->_Name, VarType);
                auto MyVal = new MyValue(Alloca, "", this->_VarType->_isConst || this->_VarType->_VarTy == VARTYPE_ARRAY, isInnerConst);
                if (!Gen.AddVariable(NewVar->_Name, MyVal)) {
                    throw std::logic_error("Naming conflict or redefinition for local variable \"" + NewVar->_Name + "\"");
                    Alloca->eraseFromParent();
                    delete MyVal;
                    return MyValue();
                }

                if (NewVar->_InitialExpr) {
                    if (VarType->isArrayTy()) {
                        throw std::logic_error("Initialize array variable \"" + NewVar->_Name + "\", which is not allowed");
                        return MyValue();
                    }
                    llvm::Value* Initializer = NULL;
                    if (InitialMyVal.IsInnerConstPointer && !isInnerConst) {
                        throw std::logic_error("Inner-const pointer cannot initialize a non-inner-const pointer \"" + NewVar->_Name + "\"");
                        return MyValue();
                    }
                    Initializer = TypeCasting(InitialMyVal, VarType, Gen);
                    if (Initializer == NULL) {
                        throw std::logic_error("Initialize variable \"" + NewVar->_Name + "\" with value of conflict type");
                        return MyValue();
                    }
                    GlobalBuilder.CreateStore(Initializer, Alloca);
                }
            }
            else {
                //create a global variable.
                if (this->_VarType->_VarTy == VARTYPE_POINTER) {
                    isInnerConst = ((PointerType*)this->_VarType)->isInnerConst();
                }
                llvm::Constant* Initializer = NULL;
                if (NewVar->_InitialExpr) {
                    if (VarType->isArrayTy()) {
                        throw std::logic_error("Initialize array variable \"" + NewVar->_Name + "\", which is not allowed");
                        return MyValue();
                    }
                    //Global variable must be initialized by a constant.

                    if (InitialMyVal.IsInnerConstPointer && !isInnerConst) {
                        throw std::logic_error("Inner-const pointer cannot initialize a non-inner-const pointer \"" + NewVar->_Name + "\"");
                        return MyValue();
                    }

                    //save the insertion point
                    llvm::BasicBlock* SaveBB = GlobalBuilder.GetInsertBlock();
                    GlobalBuilder.SetInsertPoint(Gen.GetEmptyBB());

                    llvm::Value* InitialExpr = TypeCasting(NewVar->_InitialExpr->codegen(Gen), VarType, Gen);
                    //EmptyBB is used for detect whether the initial value is a constant!
                    if (GlobalBuilder.GetInsertBlock()->size() != 0) {
                        throw std::logic_error("Initialize global variable \"" + NewVar->_Name + "\" with non-constant value");
                        return MyValue();
                    }
                    if (InitialExpr == NULL) {
                        throw std::logic_error("Initialize variable \"" + NewVar->_Name + "\" with value of conflict type");
                        return MyValue();
                    }

                    //retore the insertion point
                    GlobalBuilder.SetInsertPoint(SaveBB);
                    Initializer = (llvm::Constant*)InitialExpr;
                }
                else {
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
                auto MyVal = new MyValue(Alloca, "", this->_VarType->_isConst || this->_VarType->_VarTy == VARTYPE_ARRAY, isInnerConst);
                if (!Gen.AddVariable(NewVar->_Name, MyVal)) {
                    throw std::logic_error("Naming conflict or redefinition for global variable \"" + NewVar->_Name + "\"");
                    Alloca->eraseFromParent();
                    delete MyVal;
                    return MyValue();
                }
            }
        }
        return MyValue();
    }

    //Type declaration
    MyValue TypeDecl::codegen(Generator& Gen) {
        llvm::Type* LLVMType;
        if (this->_VarType->_VarTy == VARTYPE_STRUCT)
            LLVMType = ((ast::StructType*)this->_VarType)->GenerateStructTypeHead(Gen, this->_Alias);
        else
            LLVMType = this->_VarType->GetLLVMType(Gen);
        if (!LLVMType) {
            throw std::logic_error("Typedef \"" + this->_Alias + "\" using undefined types");
            return MyValue();
        }

        FieldDecls* Fields = NULL;
        if (this->_VarType->_VarTy == VARTYPE_STRUCT)
            Fields = ((ast::StructType*)this->_VarType)->_StructBody;
        MyType* MyTy = new MyType(LLVMType, Fields);
        if (!Gen.AddType(this->_Alias, MyTy)) {
            throw std::logic_error("Naming conflict or redefinition for type \"" + this->_Alias + "\"");
            return MyValue();
        }
        if (this->_VarType->_VarTy == VARTYPE_STRUCT)
            ((ast::StructType*)this->_VarType)->GenerateStructTypeBody(Gen);
        return MyValue();
    }

    //Builtin type
    llvm::Type* BuiltInType::GetLLVMType(Generator& Gen) {
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

    //Defined type
    llvm::Type* DefinedType::GetLLVMType(Generator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        this->_LLVMType = Gen.FindType(this->_Name)->LLVMType;
        if (this->_LLVMType == NULL) {
            throw std::logic_error("Undefined type \"" + this->_Name + "\"");
            return NULL;
        }
        else return this->_LLVMType;
    }

    //Pointer type.
    llvm::Type* PointerType::GetLLVMType(Generator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        llvm::Type* BaseType = this->_BaseType->GetLLVMType(Gen);
        return this->_LLVMType = llvm::PointerType::get(BaseType, 0U);
    }

    //Array type.
    llvm::Type* ArrayType::GetLLVMType(Generator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        if (this->_BaseType->_isConst) {
            throw std::logic_error("The base type of array cannot be const");
            return NULL;
        }
        llvm::Type* BaseType = this->_BaseType->GetLLVMType(Gen);
        if (BaseType->isVoidTy()) {
            throw std::logic_error("The base type of array cannot be void");
            return NULL;
        }
        return this->_LLVMType = llvm::ArrayType::get(BaseType, this->_Length);
    }

    //Struct type.
    llvm::Type* StructType::GetLLVMType(Generator& Gen) {
        if (this->_LLVMType)
            return this->_LLVMType;
        //Create an anonymous identified struct type
        this->GenerateStructTypeHead(Gen);
        return this->GenerateStructTypeBody(Gen);
    }
    llvm::Type* StructType::GenerateStructTypeHead(Generator& Gen, const std::string& __Name) {
        auto LLVMType = llvm::StructType::create(GlobalContext, "struct" + __Name);
        Gen.AddStructType(LLVMType, this);
        return this->_LLVMType = LLVMType;
    }
    llvm::Type* StructType::GenerateStructTypeBody(Generator& Gen) {
        std::vector<llvm::Type*> Members;
        for (auto FDecl : *(this->_StructBody))
            if (FDecl->_VarType->GetLLVMType(Gen)->isVoidTy()) {
                throw std::logic_error("The member type of struct cannot be void");
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

    //Block
    MyValue Block::codegen(Generator& Gen) {
        Gen.PushSymbolTable();
        for (auto& Stmt : *(this->_Content))
            if (GlobalBuilder.GetInsertBlock()->getTerminator())
                break;
            else if (Stmt)
                Stmt->codegen(Gen);
        Gen.PopSymbolTable();
        return MyValue();
    }

    //If
    MyValue IfStmt::codegen(Generator& Gen) {
        MyValue Condition = this->_Condition->codegen(Gen);
        if (!(Condition.Value = CastToBool(Condition.Value))) {
            throw std::logic_error("The condition expression of if-statement must be either integer, floating-point, or pointer type");
            return MyValue();
        }
        //Create "Then", "Else" and "Merge" block
        llvm::Function* CurrentFunc = Gen.GetCurFunction()->LLVMFunc;
        llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(GlobalContext, "Then");
        llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(GlobalContext, "Else");
        llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(GlobalContext, "Merge");
        GlobalBuilder.CreateCondBr(Condition.Value, ThenBB, ElseBB);
        CurrentFunc->getBasicBlockList().push_back(ThenBB);

        GlobalBuilder.SetInsertPoint(ThenBB);
        if (this->_Then) {
            this->_Then->codegen(Gen);
        }
        TerminateBlockByBr(MergeBB);
        CurrentFunc->getBasicBlockList().push_back(ElseBB);
        GlobalBuilder.SetInsertPoint(ElseBB);
        if (this->_Else) {
            this->_Else->codegen(Gen);
        }
        TerminateBlockByBr(MergeBB);
        if (MergeBB->hasNPredecessorsOrMore(1)) {
            CurrentFunc->getBasicBlockList().push_back(MergeBB);
            GlobalBuilder.SetInsertPoint(MergeBB);
        }
        return MyValue();
    }

    //For
    MyValue ForStmt::codegen(Generator& Gen) {
        //Create blocks "ForCond", "ForLoop", "ForTail" and "ForEnd"
        llvm::Function* CurrentFunc = Gen.GetCurFunction()->LLVMFunc;
        llvm::BasicBlock* ForCondBB = llvm::BasicBlock::Create(GlobalContext, "ForCond");
        llvm::BasicBlock* ForLoopBB = llvm::BasicBlock::Create(GlobalContext, "ForLoop");
        llvm::BasicBlock* ForTailBB = llvm::BasicBlock::Create(GlobalContext, "ForTail");
        llvm::BasicBlock* ForEndBB = llvm::BasicBlock::Create(GlobalContext, "ForEnd");
        if (this->_Initial) {
            Gen.PushSymbolTable();
            this->_Initial->codegen(Gen);
        }
        TerminateBlockByBr(ForCondBB);

        CurrentFunc->getBasicBlockList().push_back(ForCondBB);
        GlobalBuilder.SetInsertPoint(ForCondBB);
        if (this->_Condition) {
            MyValue Condition = this->_Condition->codegen(Gen);
            if (!(Condition.Value = CastToBool(Condition.Value))) {
                throw std::logic_error("The condition expression of for-statement must be either integer, floating-point, or pointer type");
                return MyValue();
            }
            GlobalBuilder.CreateCondBr(Condition.Value, ForLoopBB, ForEndBB);
        }
        else {
            GlobalBuilder.CreateBr(ForLoopBB);
        }
        CurrentFunc->getBasicBlockList().push_back(ForLoopBB);
        GlobalBuilder.SetInsertPoint(ForLoopBB);
        if (this->_LoopBody) {
            Gen.EnterLoop(ForTailBB, ForEndBB);
            this->_LoopBody->codegen(Gen);
            Gen.LeaveLoop();
        }

        TerminateBlockByBr(ForTailBB);
        CurrentFunc->getBasicBlockList().push_back(ForTailBB);
        GlobalBuilder.SetInsertPoint(ForTailBB);
        if (this->_Tail)
            this->_Tail->codegen(Gen);
        GlobalBuilder.CreateBr(ForCondBB);
        CurrentFunc->getBasicBlockList().push_back(ForEndBB);
        GlobalBuilder.SetInsertPoint(ForEndBB);
        if (this->_Initial) {
            Gen.PopSymbolTable();
        }
        return MyValue();
    }

    //Switch
    MyValue SwitchStmt::codegen(Generator& Gen) {
        llvm::Function* CurrentFunc = Gen.GetCurFunction()->LLVMFunc;
        MyValue Matcher = this->_Matcher->codegen(Gen);
        std::vector<llvm::BasicBlock*> CaseBB;
        for (int i = 0; i < this->_CaseList->size(); i++)
            CaseBB.push_back(llvm::BasicBlock::Create(GlobalContext, "Case" + std::to_string(i)));
        CaseBB.push_back(llvm::BasicBlock::Create(GlobalContext, "SwitchEnd"));
        std::vector<llvm::BasicBlock*> ComparisonBB;
        ComparisonBB.push_back(GlobalBuilder.GetInsertBlock());
        for (int i = 1; i < this->_CaseList->size(); i++)
            ComparisonBB.push_back(llvm::BasicBlock::Create(GlobalContext, "Comparison" + std::to_string(i)));
        ComparisonBB.push_back(CaseBB.back());
        //Branches
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
        Gen.PushSymbolTable();
        for (int i = 0; i < this->_CaseList->size(); i++) {
            CurrentFunc->getBasicBlockList().push_back(CaseBB[i]);
            GlobalBuilder.SetInsertPoint(CaseBB[i]);
            Gen.EnterLoop(CaseBB[i + 1], CaseBB.back());
            this->_CaseList->at(i)->codegen(Gen);
            Gen.LeaveLoop();
        }
        Gen.PopSymbolTable();

        if (CaseBB.back()->hasNPredecessorsOrMore(1)) {
            CurrentFunc->getBasicBlockList().push_back(CaseBB.back());
            GlobalBuilder.SetInsertPoint(CaseBB.back());
        }
        return MyValue();
    }

    //Case
    MyValue CaseStmt::codegen(Generator& Gen) {
        for (auto& Stmt : *(this->_Content))
            if (GlobalBuilder.GetInsertBlock()->getTerminator())
                break;
            else if (Stmt)
                Stmt->codegen(Gen);
        TerminateBlockByBr(Gen.GetContinueBlock());
        return MyValue();
    }

    //Continue
    MyValue ContinueStmt::codegen(Generator& Gen) {
        llvm::BasicBlock* ContinueTarget = Gen.GetContinueBlock();
        if (ContinueTarget)
            GlobalBuilder.CreateBr(ContinueTarget);
        else
            throw std::logic_error("Using \"continue\" not in a loop or switch block");
        return MyValue();
    }

    //Break
    MyValue BreakStmt::codegen(Generator& Gen) {
        llvm::BasicBlock* BreakTarget = Gen.GetBreakBlock();
        if (BreakTarget)
            GlobalBuilder.CreateBr(BreakTarget);
        else
            throw std::logic_error("Using \"break\" not a in a loop or switch block");
        return MyValue();
    }

    //Return
    MyValue ReturnStmt::codegen(Generator& Gen) {
        llvm::Function* Func = Gen.GetCurFunction()->LLVMFunc;
        if (!Func) {
            throw std::logic_error("Using \"return\" not in a function body");
            return MyValue();
        }
        if (this->_RetVal == NULL) {
            if (Func->getReturnType()->isVoidTy())
                GlobalBuilder.CreateRetVoid();
            else {
                throw std::logic_error("Expected an expression after return statement");
                return MyValue();
            }
        }
        else {
            auto MyVal = this->_RetVal->codegen(Gen);
            auto RetType = Gen.GetCurFunction()->RetType;
            auto RetLLVMType = Func->getReturnType();

            if (MyVal.IsInnerConstPointer &&
                !(RetType->_VarTy == VARTYPE_POINTER && ((PointerType*)(RetType))->isInnerConst())) {
                throw std::logic_error("Cannot return an inner-const pointer as non-inner-const pointer type");
                return MyValue();
            }

            MyValue RetVal = MyValue(TypeCasting(MyVal, RetLLVMType, Gen));
            if (!RetVal.Value) {
                throw std::logic_error("The return value cannot be cast to the return type");
                return MyValue();
            }
            GlobalBuilder.CreateRet(RetVal.Value);
        }
        return MyValue();
    }

    //Subscript, like a[1]
    MyValue Subscript::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue Subscript::codegenPtr(Generator& Gen) {
        MyValue ArrayPtr = this->_Array->codegen(Gen);
        if (!ArrayPtr.Value->getType()->isPointerTy()) {
            throw std::logic_error("Subscript operator \"[]\" must be applied to pointers or arrays");
            return MyValue();
        }
        MyValue Subspt = this->_Index->codegen(Gen);
        if (!(Subspt.Value->getType()->isIntegerTy())) {
            throw std::logic_error("Value in \"[]\" should be an integer");
            return MyValue();
        }
        return MyValue(CreateAdd(ArrayPtr, Subspt, Gen), ArrayPtr.Name, ArrayPtr.IsConst,
            ArrayPtr.IsInnerConstPointer, ArrayPtr.IsPointingToInnerConst);
    }

    //sizeof()
    MyValue SizeOf::codegen(Generator& Gen) {
        if (this->_Arg1)//Expr
            return MyValue(GlobalBuilder.getInt64(Gen.GetTypeSize(this->_Arg1->codegen(Gen).Value->getType())));
        else if (this->_Arg2)//Type
            return MyValue(GlobalBuilder.getInt64(Gen.GetTypeSize(this->_Arg2->GetLLVMType(Gen))));
        else {//identifier
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
            throw std::logic_error(this->_Arg3 + " is neither a type nor a variable");
            return MyValue();
        }
    }
    MyValue SizeOf::codegenPtr(Generator& Gen) {
        throw std::logic_error("Sizeof() returns a right value");
        return MyValue();
    }

    //Function call
    MyValue FunctionCall::codegen(Generator& Gen) {
        ast::MyFunction* MyFunc = Gen.FindFunction(this->_FuncName);
        auto curMyFunc = Gen.GetCurFunction();
        if (MyFunc == NULL) {
            throw std::logic_error(this->_FuncName + " is not a defined function");
            return MyValue();
        }
        llvm::Function* Func = MyFunc->LLVMFunc;
        bool HasBaseType = !MyFunc->TypeName.empty();
        auto Args = MyFunc->Args;

        //Check the arg number
        if (Func->isVarArg() && this->_ArgList->size() + HasBaseType < Func->arg_size() ||
            !Func->isVarArg() && this->_ArgList->size() + HasBaseType != Func->arg_size()) {
            throw std::invalid_argument("Args number doesn't match while calling function " + this->_FuncName +
                ". Expected " + std::to_string(Func->arg_size() - HasBaseType) + ", got " + std::to_string(this->_ArgList->size()));
            return MyValue();
        }

        std::vector<llvm::Value*> ArgList;

        if (HasBaseType) {
            auto BaseType = Func->arg_begin()->getType();
            if (this->_StructRef) {
                MyValue StructPtr = this->_StructRef->codegenPtr(Gen);
                if (!StructPtr.Value->getType()->isPointerTy()) {
                    throw std::invalid_argument("Expected a struct reference when calling method \"" + this->_FuncName + "\"");
                    return MyValue();
                }
                if (StructPtr.Value->getType() != BaseType) {
                    throw std::invalid_argument("Expected a struct reference of type \"" + MyFunc->TypeName + "\" when calling method \"" + this->_FuncName + "\"");
                    return MyValue();
                }
                StructPtr.Value = TypeCasting(StructPtr, BaseType, Gen);
                ArgList.push_back(StructPtr.Value);
            }
            else if (this->_StructPtr) {
                MyValue StructPtr = this->_StructPtr->codegen(Gen);
                if (!StructPtr.Value->getType()->isPointerTy()) {
                    throw std::invalid_argument("Expected a struct pointer when calling method \"" + this->_FuncName + "\"");
                    return MyValue();
                }
                if (StructPtr.Value->getType() != BaseType) {
                    throw std::invalid_argument("Expected a struct pointer of type \"" + MyFunc->TypeName + "\" when calling method \"" + this->_FuncName + "\"");
                    return MyValue();
                }
                StructPtr.Value = TypeCasting(StructPtr, BaseType, Gen);
                ArgList.push_back(StructPtr.Value);
            }
            else {
                throw std::invalid_argument("Expected a struct reference or pointer when calling method \"" + this->_FuncName + "\"");
                return MyValue();
            }
        }

        //check private / public
        if (!MyFunc->IsPublic()) {
            auto MyFuncLLVMType = Gen.FindType(MyFunc->TypeName)->LLVMType;
            auto curMyBaseType = Gen.FindType(curMyFunc->TypeName);
            if (!curMyBaseType || curMyBaseType->LLVMType != MyFuncLLVMType) {
                throw std::logic_error("Cannot call private member function \"" + this->_FuncName + "\" from outside its class");
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

            if (Arg.IsInnerConstPointer && ArgType->_VarTy == VARTYPE_POINTER && !((PointerType*)ArgType)->isInnerConst()) {
                throw std::invalid_argument("Cannot pass an inner-const pointer to a non-inner-const pointer while calling function \"" + this->_FuncName + "\"");
                return MyValue();
            }
            Arg.Value = TypeCasting(Arg, ArgIter->getType(), Gen);
            if (Arg.Value == NULL) {
                throw std::invalid_argument(std::to_string(Index) + "-th arg type doesn't match while calling function \"" + this->_FuncName + "\"");
                return MyValue();
            }
            ArgList.push_back(Arg.Value);
        }

        if (Func->isVarArg())
            for (; Index < this->_ArgList->size(); Index++) {
                MyValue Arg = this->_ArgList->at(Index)->codegen(Gen);
                if (Arg.Value->getType()->isIntegerTy())
                    Arg.Value = TypeUpgrading(Arg.Value, GlobalBuilder.getInt32Ty());
                else if (Arg.Value->getType()->isFloatingPointTy())
                    Arg.Value = TypeUpgrading(Arg.Value, GlobalBuilder.getDoubleTy());
                ArgList.push_back(Arg.Value);
            }
        //Create function call
        auto RetType = MyFunc->RetType;
        bool isInnerConst = false;
        if (RetType->_VarTy == VARTYPE_POINTER) {
            isInnerConst = ((PointerType*)RetType)->isInnerConst();
        }
        return MyValue(GlobalBuilder.CreateCall(Func, ArgList), "", RetType->_isConst, isInnerConst);
    }
    MyValue FunctionCall::codegenPtr(Generator& Gen) {
        throw std::logic_error("Function call returns a right value");
        return MyValue();
    }

    //Structure reference, like a.x
    MyValue StructReference::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue StructReference::codegenPtr(Generator& Gen) {
        MyValue StructPtr = this->_Struct->codegenPtr(Gen);
        if (!StructPtr.Value->getType()->isPointerTy() || !StructPtr.Value->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Reference operator \".\" must be applied to structs");
            return MyValue();
        }

        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr.Value->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\"");
                return MyValue();
            }

            //check private / public
            auto curMyFunc = Gen.GetCurFunction();
            auto curMyBaseType = Gen.FindType(curMyFunc->TypeName);
            auto exprLLVMType = StructPtr.Value->getType()->getNonOpaquePointerElementType();
            bool IsPublic = IsCapital(this->_MemName[0]);
            if (!IsPublic && (!curMyBaseType || curMyBaseType->LLVMType != exprLLVMType)) {
                throw std::logic_error("Cannot access private member variable \"" + this->_MemName + "\" from outside its class");
                return MyValue();
            }

            std::vector<llvm::Value*> Indices;
            Indices.push_back(GlobalBuilder.getInt32(0));
            Indices.push_back(GlobalBuilder.getInt32(MemIndex));

            auto memType = StructType->_StructBody->at(MemIndex)->_VarType;
            bool isConst = memType->_isConst || StructPtr.IsInnerConstPointer;
            bool isInnerConst = false;
            if (memType->_VarTy == VARTYPE_POINTER) {
                isInnerConst = ((PointerType*)memType)->isInnerConst();
            }
            return MyValue(GlobalBuilder.CreateGEP(StructPtr.Value->getType()->getNonOpaquePointerElementType(), StructPtr.Value, Indices)
                , "", true, isConst, isInnerConst);
        }
        return MyValue();
    }

    //Structure dereference, like a->x
    MyValue StructDereference::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue StructDereference::codegenPtr(Generator& Gen) {
        MyValue StructPtr = this->_StructPtr->codegen(Gen);
        if (!StructPtr.Value->getType()->isPointerTy() || !StructPtr.Value->getType()->getNonOpaquePointerElementType()->isStructTy()) {
            throw std::logic_error("Dereference operator \"->\" must be applied to structs");
            return MyValue();
        }

        ast::StructType* StructType = Gen.FindStructType((llvm::StructType*)StructPtr.Value->getType()->getNonOpaquePointerElementType());
        if (StructType) {
            int MemIndex = StructType->GetElementIndex(this->_MemName);
            if (MemIndex == -1) {
                throw std::logic_error("The struct doesn't have member variable \"" + this->_MemName + "\"");
                return MyValue();
            }

            //check private / public
            auto curMyFunc = Gen.GetCurFunction();
            auto curMyBaseType = Gen.FindType(curMyFunc->TypeName);
            auto exprLLVMType = StructPtr.Value->getType()->getNonOpaquePointerElementType();
            bool IsPublic = IsCapital(this->_MemName[0]);
            if (!IsPublic && (!curMyBaseType || curMyBaseType->LLVMType != exprLLVMType)) {
                throw std::logic_error("Cannot access private member variable \"" + this->_MemName + "\" from outside its class");
                return MyValue();
            }
            std::vector<llvm::Value*> Indices;
            Indices.push_back(GlobalBuilder.getInt32(0));
            Indices.push_back(GlobalBuilder.getInt32(MemIndex));

            auto memType = StructType->_StructBody->at(MemIndex)->_VarType;
            bool isConst = memType->_isConst || StructPtr.IsInnerConstPointer;
            bool isInnerConst = false;
            if (memType->_VarTy == VARTYPE_POINTER) {
                isInnerConst = ((PointerType*)memType)->isInnerConst();
            }
            return MyValue(GlobalBuilder.CreateGEP(StructPtr.Value->getType()->getNonOpaquePointerElementType(), StructPtr.Value, Indices)
                , "", true, isConst, isInnerConst);
        }
        return MyValue();
    }

    //Unary plus
    MyValue UnaryPlus::codegen(Generator& Gen) {
        MyValue Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand.Value->getType()->isIntegerTy() ||
            Operand.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary plus must be applied to integers or floating-point numbers");
        return MyValue(Operand);
    }
    MyValue UnaryPlus::codegenPtr(Generator& Gen) {
        throw std::logic_error("Unary plus returns a right value");
        return MyValue();
    }

    //Unary minus
    MyValue UnaryMinus::codegen(Generator& Gen) {
        MyValue Operand = this->_Operand->codegen(Gen);
        if (!(
            Operand.Value->getType()->isIntegerTy() ||
            Operand.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Unary minus must be applied to integers or floating-point numbers");
        if (Operand.Value->getType()->isIntegerTy())
            return MyValue(GlobalBuilder.CreateNeg(Operand.Value), Operand.Name, Operand.IsConst, Operand.IsInnerConstPointer);
        else
            return MyValue(GlobalBuilder.CreateFNeg(Operand.Value), Operand.Name, Operand.IsConst, Operand.IsInnerConstPointer);
    }
    MyValue UnaryMinus::codegenPtr(Generator& Gen) {
        throw std::logic_error("Unary minus returns a right value");
        return MyValue();
    }

    //typecast
    MyValue TypeCast::codegen(Generator& Gen) {
        MyValue MyVal = this->_Operand->codegen(Gen);
        if (MyVal.IsInnerConstPointer
            && !(this->_VarType->_VarTy == VARTYPE_POINTER && ((PointerType*)(this->_VarType))->isInnerConst())) {
            throw std::logic_error("Cannot type cast an inner-const pointer to a non-inner-const pointer");
            return MyValue();
        }

        MyValue Ret = MyValue(TypeCasting(MyVal, this->_VarType->GetLLVMType(Gen), Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
        if (Ret.Value == NULL) {
            throw std::logic_error("Unable to do type casting");
            return MyValue();
        }
        return Ret;
    }
    MyValue TypeCast::codegenPtr(Generator& Gen) {
        throw std::logic_error("Type casting returns a right value");
        return MyValue();
    }

    //Prefix-increment
    MyValue PrefixInc::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue PrefixInc::codegenPtr(Generator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do prefix increment");
            return MyValue();
        }
        MyValue OpValue = MyValue(GlobalBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Prefix increment must be applied to integers or floating-point numbers");
        MyValue OpValuePlus = MyValue(CreateAdd(OpValue, MyValue(GlobalBuilder.getInt1(1)), Gen));
        GlobalBuilder.CreateStore(OpValuePlus.Value, Operand.Value);
        return MyValue(Operand);
    }

    //Postfix-increment
    MyValue PostfixInc::codegen(Generator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do postfix increment");
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
    MyValue PostfixInc::codegenPtr(Generator& Gen) {
        throw std::logic_error("Postfix increment returns a right value");
        return MyValue();
    }

    //Prefix-decrement
    MyValue PrefixDec::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue PrefixDec::codegenPtr(Generator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do prefix decrement");
            return MyValue();
        }
        MyValue OpValue = MyValue(GlobalBuilder.CreateLoad(Operand.Value->getType()->getNonOpaquePointerElementType(), Operand.Value));
        if (!(
            OpValue.Value->getType()->isIntegerTy() ||
            OpValue.Value->getType()->isFloatingPointTy())
            )
            throw std::logic_error("Prefix decrement must be applied to integers or floating-point numbers");
        MyValue OpValueMinus = MyValue(CreateSub(OpValue, MyValue(GlobalBuilder.getInt1(1)), Gen));
        GlobalBuilder.CreateStore(OpValueMinus.Value, Operand.Value);
        return MyValue(Operand);
    }

    //Postfix-decrement
    MyValue PostfixDec::codegen(Generator& Gen) {
        MyValue Operand = this->_Operand->codegenPtr(Gen);
        if (Operand.IsInnerConstPointer) {
            throw std::logic_error("Const variable cannot do postfix decrement");
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
    MyValue PostfixDec::codegenPtr(Generator& Gen) {
        throw std::logic_error("Postfix decrement returns a right value");
        return MyValue();
    }

    //Indirection
    MyValue Indirection::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue Indirection::codegenPtr(Generator& Gen) {
        MyValue Ptr = this->_Operand->codegen(Gen);
        if (!Ptr.Value->getType()->isPointerTy()) {
            throw std::logic_error("Indirection operator \"dptr()\" only applies on pointers or arrays");
            return MyValue();
        }
        return MyValue(Ptr);
    }

    //Fetch addres
    MyValue AddressOf::codegen(Generator& Gen) {
        return MyValue(this->_Operand->codegenPtr(Gen));
    }
    MyValue AddressOf::codegenPtr(Generator& Gen) {
        throw std::logic_error("Address operator \"addr()\" returns a right value");
        return MyValue();
    }

    //Logic NOT
    MyValue LogicNot::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(GlobalBuilder.CreateICmpEQ(CastToBool(MyVal.Value), GlobalBuilder.getInt1(false)), MyVal.Name, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue LogicNot::codegenPtr(Generator& Gen) {
        throw std::logic_error("Logic NOT operator \"!\" returns a right value");
        return MyValue();
    }

    //Bitwise NOT
    MyValue BitwiseNot::codegen(Generator& Gen) {
        MyValue Operand = this->_Operand->codegen(Gen);
        if (!Operand.Value->getType()->isIntegerTy()) {
            throw std::logic_error("Bitwise NOT operator \"~\" must be applied to integers");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateNot(Operand.Value), Operand.Name, Operand.IsConst, Operand.IsInnerConstPointer, Operand.IsPointingToInnerConst);
    }
    MyValue BitwiseNot::codegenPtr(Generator& Gen) {
        throw std::logic_error("Bitwise NOT operator \"~\" returns a right value");
        return MyValue();
    }

    //Division
    MyValue Division::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateDiv(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Division::codegenPtr(Generator& Gen) {
        throw std::logic_error("Division operator \"/\" returns a right value");
        return MyValue();
    }

    //Multiplication
    MyValue Multiplication::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateMul(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Multiplication::codegenPtr(Generator& Gen) {
        throw std::logic_error("Multiplication operator \"*\" returns a right value");
        return MyValue();
    }

    //Modulo
    MyValue Modulo::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateMod(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Modulo::codegenPtr(Generator& Gen) {
        throw std::logic_error("Modulo operator \"%\" returns a right value");
        return MyValue();
    }

    //Addition
    MyValue Addition::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAdd(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Addition::codegenPtr(Generator& Gen) {
        throw std::logic_error("Addition operator \"+\" returns a right value");
        return MyValue();
    }

    //Subtraction
    MyValue Subtraction::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateSub(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue Subtraction::codegenPtr(Generator& Gen) {
        throw std::logic_error("Subtraction operator \"-\" returns a right value");
        return MyValue();
    }

    //LeftShift
    MyValue LeftShift::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateShl(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue LeftShift::codegenPtr(Generator& Gen) {
        throw std::logic_error("Left shifting operator \"<<\" returns a right value");
        return MyValue();
    }

    //RightShift
    MyValue RightShift::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateShr(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue RightShift::codegenPtr(Generator& Gen) {
        throw std::logic_error("Right shifting operator \">>\" returns a right value");
        return MyValue();
    }

    //LogicGT
    MyValue LogicGT::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);

        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSGT(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOGT(LeftOp.Value, RightOp.Value));
        }
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
        throw std::logic_error("Comparison \">\" between uncomparable types");
    }
    MyValue LogicGT::codegenPtr(Generator& Gen) {
        throw std::logic_error("Comparison operator \">\" returns a right value");
        return MyValue();
    }

    //LogicGE
    MyValue LogicGE::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSGE(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOGE(LeftOp.Value, RightOp.Value));
        }
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
        throw std::logic_error("Comparison \">=\" between uncomparable types");
    }
    MyValue LogicGE::codegenPtr(Generator& Gen) {
        throw std::logic_error("Comparison operator \">=\" returns a right value");
        return MyValue();
    }

    //LogicLT
    MyValue LogicLT::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSLT(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOLT(LeftOp.Value, RightOp.Value));
        }
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
        throw std::logic_error("Comparison \"<\" between uncomparable types");
    }
    MyValue LogicLT::codegenPtr(Generator& Gen) {
        throw std::logic_error("Comparison operator \"<\" returns a right value");
        return MyValue();
    }

    //LogicL
    MyValue LogicLE::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpSLE(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpOLE(LeftOp.Value, RightOp.Value));
        }
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
        throw std::logic_error("Comparison \"<=\" between uncomparable types");
    }
    MyValue LogicLE::codegenPtr(Generator& Gen) {
        throw std::logic_error("Comparison operator \"<=\" returns a right value");
        return MyValue();
    }

    //LogicEQ
    MyValue LogicEQ::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateCmpEQ(LeftOp.Value, RightOp.Value));
    }
    MyValue LogicEQ::codegenPtr(Generator& Gen) {
        throw std::logic_error("Comparison operator \"==\" returns a right value");
        return MyValue();
    }

    //LogicNEQ
    MyValue LogicNEQ::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
            if (LeftOp.Value->getType()->isIntegerTy())
                return MyValue(GlobalBuilder.CreateICmpNE(LeftOp.Value, RightOp.Value));
            else
                return MyValue(GlobalBuilder.CreateFCmpONE(LeftOp.Value, RightOp.Value));
        }
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
        throw std::logic_error("Comparison \"!=\" between uncomparable types");
    }
    MyValue LogicNEQ::codegenPtr(Generator& Gen) {
        throw std::logic_error("Comparison operator \"!=\" returns a right value");
        return MyValue();
    }

    //BitwiseAND
    MyValue BitwiseAND::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateBitwiseAND(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue BitwiseAND::codegenPtr(Generator& Gen) {
        throw std::logic_error("Bitwise AND operator \"&\" returns a right value");
        return MyValue();
    }

    //BitwiseXOR
    MyValue BitwiseXOR::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateBitwiseXOR(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue BitwiseXOR::codegenPtr(Generator& Gen) {
        throw std::logic_error("Bitwise XOR operator \"^\" returns a right value");
        return MyValue();
    }

    //BitwiseOR
    MyValue BitwiseOR::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateBitwiseOR(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }
    MyValue BitwiseOR::codegenPtr(Generator& Gen) {
        throw std::logic_error("Bitwise OR operator \"|\" returns a right value");
        return MyValue();
    }

    //LogicAND
    MyValue LogicAND::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        LeftOp = MyValue(CastToBool(LeftOp.Value));
        if (LeftOp.Value == NULL) {
            throw std::logic_error("Logic AND operator \"&&\" must be applied to 2 boolean expressions");
            return MyValue();
        }
        RightOp = MyValue(CastToBool(RightOp.Value));
        if (RightOp.Value == NULL) {
            throw std::logic_error("Logic AND operator \"&&\" must be applied to 2 boolean expressions");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateLogicalAnd(LeftOp.Value, RightOp.Value));
    }
    MyValue LogicAND::codegenPtr(Generator& Gen) {
        throw std::logic_error("Logic AND operator \"&&\" returns a right value");
        return MyValue();
    }

    //LogicOR
    MyValue LogicOR::codegen(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegen(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        LeftOp = MyValue(CastToBool(LeftOp.Value));
        if (LeftOp.Value == NULL) {
            throw std::logic_error("Logic OR operator \"||\" must be applied to 2 boolean expressions");
            return MyValue();
        }
        RightOp = MyValue(CastToBool(RightOp.Value));
        if (RightOp.Value == NULL) {
            throw std::logic_error("Logic OR operator \"||\" must be applied to 2 boolean expressions");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateLogicalOr(LeftOp.Value, RightOp.Value));
    }
    MyValue LogicOR::codegenPtr(Generator& Gen) {
        throw std::logic_error("Logic OR operator \"||\" returns a right value");
        return MyValue();
    }

    //TernaryCondition
    MyValue TernaryCondition::codegen(Generator& Gen) {
        MyValue Condition = MyValue(CastToBool(this->_Condition->codegen(Gen).Value));
        if (Condition.Value == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must a boolean expression");
            return MyValue();
        }
        MyValue True = this->_Then->codegen(Gen);
        MyValue False = this->_Else->codegen(Gen);
        if (True.Value->getType() == False.Value->getType() || TypeUpgrading(True.Value, False.Value)) {
            return MyValue(GlobalBuilder.CreateSelect(Condition.Value, True.Value, False.Value), True.Name, True.IsConst,
                True.IsInnerConstPointer || False.IsInnerConstPointer, True.IsPointingToInnerConst);
        }
        else {
            throw std::logic_error("Thernary operand \" ? : \" among uncomparable types");
            return MyValue();
        }
    }
    MyValue TernaryCondition::codegenPtr(Generator& Gen) {
        MyValue Condition = MyValue(CastToBool(this->_Condition->codegen(Gen).Value));
        if (Condition.Value == NULL) {
            throw std::logic_error("The first operand of thernary operand \" ? : \" must a boolean expression");
            return MyValue();
        }
        MyValue True = this->_Then->codegenPtr(Gen);
        MyValue False = this->_Else->codegenPtr(Gen);
        if (True.Value->getType() != False.Value->getType()) {
            throw std::logic_error("When using thernary expressions \" ? : \" as left-values, the latter two operands must be of the same type");
            return MyValue();
        }
        return MyValue(GlobalBuilder.CreateSelect(Condition.Value, True.Value, False.Value));
    }

    //DirectAssign
    MyValue DirectAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue DirectAssign::codegenPtr(Generator& Gen) {
        MyValue LeftOp = this->_LeftOp->codegenPtr(Gen);
        MyValue RightOp = this->_RightOp->codegen(Gen);
        return MyValue(CreateAssignment(LeftOp, RightOp, Gen), LeftOp.Name, LeftOp.IsConst, LeftOp.IsInnerConstPointer || RightOp.IsInnerConstPointer, LeftOp.IsPointingToInnerConst);
    }

    //DivAssign
    MyValue DivAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue DivAssign::codegenPtr(Generator& Gen) {
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

    //MulAssign
    MyValue MulAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue MulAssign::codegenPtr(Generator& Gen) {
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

    //ModAssign
    MyValue ModAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue ModAssign::codegenPtr(Generator& Gen) {
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

    //AddAssign
    MyValue AddAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue AddAssign::codegenPtr(Generator& Gen) {
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

    //SubAssign
    MyValue SubAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue SubAssign::codegenPtr(Generator& Gen) {
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

    //SHLAssign
    MyValue SHLAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue SHLAssign::codegenPtr(Generator& Gen) {
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

    //SHRAssign
    MyValue SHRAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue SHRAssign::codegenPtr(Generator& Gen) {
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

    //BitwiseANDAssign
    MyValue BitwiseANDAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue BitwiseANDAssign::codegenPtr(Generator& Gen) {
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

    //BitwiseXORAssign
    MyValue BitwiseXORAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue BitwiseXORAssign::codegenPtr(Generator& Gen) {
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

    //BitwiseORAssign
    MyValue BitwiseORAssign::codegen(Generator& Gen) {
        MyValue MyVal = this->codegenPtr(Gen);
        return MyValue(CreateLoad(MyVal.Value, Gen), MyVal.Name, MyVal.IsConst, MyVal.IsInnerConstPointer, MyVal.IsPointingToInnerConst);
    }
    MyValue BitwiseORAssign::codegenPtr(Generator& Gen) {
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

    //Comma expression
    MyValue CommaExpr::codegen(Generator& Gen) {
        this->_LeftOp->codegen(Gen);
        return MyValue(this->_RightOp->codegen(Gen));
    }
    MyValue CommaExpr::codegenPtr(Generator& Gen) {
        this->_LeftOp->codegen(Gen);
        return MyValue(this->_RightOp->codegenPtr(Gen));
    }

    //Variable
    MyValue Variable::codegen(Generator& Gen) {
        const MyValue* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) {
            return MyValue(CreateLoad(VarPtr->Value, Gen), VarPtr->Name, VarPtr->IsConst, VarPtr->IsInnerConstPointer);
        }
        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a declared variable");
        return MyValue();
    }
    MyValue Variable::codegenPtr(Generator& Gen) {
        const MyValue* VarPtr = Gen.FindVariable(this->_Name);
        if (VarPtr) return MyValue(VarPtr->Value, VarPtr->Name, true,
            VarPtr->IsConst, VarPtr->IsInnerConstPointer);

        throw std::logic_error("Identifier \"" + this->_Name + "\" is not a declared variable");
        return MyValue();
    }

    //Constant
    MyValue Constant::codegen(Generator& Gen) {
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
            throw std::logic_error("Unknown constant type");
            return MyValue();
        }
    }
    MyValue Constant::codegenPtr(Generator& Gen) {
        throw std::logic_error("Constant is a right-value");
        return MyValue();
    }

    //Global string
    MyValue GlobalString::codegen(Generator& Gen) {
        return MyValue(GlobalBuilder.CreateGlobalStringPtr(this->_Content.c_str()), "", true, false, false);
    }
    MyValue GlobalString::codegenPtr(Generator& Gen) {
        throw std::logic_error("Global string is a right value");
        return MyValue();
    }

    MyValue This::codegen(Generator& Gen) {
        const MyValue* ThisPtr = Gen.FindVariable("0this");
        if (ThisPtr) {
            return MyValue(CreateLoad(ThisPtr->Value, Gen), ThisPtr->Name, true, false);
        }
        throw std::logic_error("Using \"this\" but not in a member function");
        return MyValue();
    }

    MyValue This::codegenPtr(Generator& Gen) {
        const MyValue* ThisPtr = Gen.FindVariable("0this");
        if (ThisPtr) {
            return MyValue(ThisPtr->Value, ThisPtr->Name, true, true);
        }
        throw std::logic_error("Using \"this\" but not in a member function");
        return MyValue();
    }
}