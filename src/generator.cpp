#include "generator.hpp"
//global llvm context.
llvm::LLVMContext GlobalContext;

//global llvm builder.
llvm::IRBuilder<> GlobalBuilder(GlobalContext);

Generator::Generator(void) :
    Module(new llvm::Module("main", GlobalContext)),
    DataLayout(new llvm::DataLayout(Module)),
    CurFunction(NULL),
    StructTyTable(NULL),
    SymbolTableStack(),
    ContinueBlockStack(),
    BreakBlockStack(),
    EmptyBB(NULL),
    EmptyFunc(NULL)
{}

llvm::TypeSize Generator::GetTypeSize(llvm::Type* Type) {
    return this->DataLayout->getTypeAllocSize(Type);
}

void Generator::PushSymbolTable(void) {
    this->SymbolTableStack.push_back(new SymbolStackFrame());
}

void Generator::PopSymbolTable(void) {
    if (this->SymbolTableStack.size() == 0) return;
    delete this->SymbolTableStack.back();
    this->SymbolTableStack.pop_back();
}

ast::MyFunction* Generator::FindFunction(std::string Name) {
    if (this->SymbolTableStack.size() == 0) return NULL;
    for (auto FrameIter = this->SymbolTableStack.end() - 1; FrameIter >= this->SymbolTableStack.begin(); FrameIter--) {
        auto FuncTable = (*FrameIter)->FuncSymbolTable;
        auto mapIter = FuncTable->find(Name);
        if (mapIter != FuncTable->end())
            return mapIter->second.GetFunction();
    }
    return NULL;
}

AddFunctionReseult Generator::AddFunction(std::string Name, ast::MyFunction* Function) {
    if (this->SymbolTableStack.size() == 0) return ADDFUNC_ERROR;
    auto TopFrame = this->SymbolTableStack.back();
    auto FuncTable = TopFrame->FuncSymbolTable;
    auto mapIter = FuncTable->find(Name);
    if (mapIter != FuncTable->end()) {
        if (mapIter->second.GetFunction())
            return ADDFUNC_DECLARED;
        else
            return ADDFUNC_NAMECONFLICT;
    }
    FuncTable->insert({ Name, Symbol(Function) });
    return ADDFUNC_SUCCESS;
}

ast::MyType* Generator::FindType(std::string Name) {
    if (this->SymbolTableStack.size() == 0) return NULL;
    for (auto FrameIter = this->SymbolTableStack.end() - 1; FrameIter >= this->SymbolTableStack.begin(); FrameIter--) {
        auto TypeTable = (*FrameIter)->TypeSymbolTable;
        auto mapIter = TypeTable->find(Name);
        if (mapIter != TypeTable->end())
            return mapIter->second.GetType();
    }
    return NULL;
}

bool Generator::AddType(std::string Name, ast::MyType* Type) {
    if (this->SymbolTableStack.size() == 0) return false;
    auto TopFrame = this->SymbolTableStack.back();
    auto TypeTable = TopFrame->TypeSymbolTable;
    auto mapIter = TypeTable->find(Name);
    if (mapIter != TypeTable->end())
        return false;
    TypeTable->insert({ Name, Symbol(Type) });
    return true;
}

ast::MyValue* Generator::FindVariable(std::string Name) {
    if (this->SymbolTableStack.size() == 0) return NULL;
    for (auto FrameIter = this->SymbolTableStack.end() - 1; FrameIter >= this->SymbolTableStack.begin(); FrameIter--) {
        auto VarTable = (*FrameIter)->VarSymbolTable;
        auto mapIter = VarTable->find(Name);
        if (mapIter != VarTable->end())
            return mapIter->second.GetVariable();
    }
    return NULL;
}

bool Generator::AddVariable(std::string Name, ast::MyValue* Variable) {
    if (this->SymbolTableStack.size() == 0) return false;
    auto TopFrame = this->SymbolTableStack.back();
    auto VarTable = TopFrame->VarSymbolTable;
    auto mapIter = VarTable->find(Name);
    if (mapIter != VarTable->end())
        return false;
    VarTable->insert({ Name, Symbol(Variable) });
    return true;
}

ast::StructType* Generator::FindStructType(llvm::StructType* Ty1) {
    auto mapIter = this->StructTyTable->find(Ty1);
    if (mapIter != this->StructTyTable->end())
        return mapIter->second;
    return NULL;
}

bool Generator::AddStructType(llvm::StructType* Ty1, ast::StructType* Ty2) {
    auto mapIter = this->StructTyTable->find(Ty1);
    if (mapIter != this->StructTyTable->end())
        return false;
    (*this->StructTyTable)[Ty1] = Ty2;
    return true;
}

void Generator::SetCurFunction(ast::MyFunction* Func) {
    this->CurFunction = Func;
}

ast::MyFunction* Generator::GetCurFunction(void) {
    return this->CurFunction;
}

void Generator::EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB) {
    this->ContinueBlockStack.push_back(ContinueBB);
    this->BreakBlockStack.push_back(BreakBB);
}

void Generator::LeaveLoop(void) {
    if (this->ContinueBlockStack.size() == 0 || this->BreakBlockStack.size() == 0) return;
    this->ContinueBlockStack.pop_back();
    this->BreakBlockStack.pop_back();
}

llvm::BasicBlock* Generator::GetContinueBlock(void) {
    if (this->ContinueBlockStack.size() > 0)
        return this->ContinueBlockStack.back();
    else
        return NULL;
}

llvm::BasicBlock* Generator::GetBreakBlock(void) {
    if (this->BreakBlockStack.size() > 0)
        return this->BreakBlockStack.back();
    else
        return NULL;
}

llvm::BasicBlock* Generator::GetEmptyBB(void) {
    return this->EmptyBB;
}

void Generator::GenerateIRCode(ast::Program& Root) {
    //Initialize symbol table
    this->StructTyTable = new StructTable;
    this->PushSymbolTable();

    this->EmptyFunc = llvm::Function::Create(llvm::FunctionType::get(GlobalBuilder.getVoidTy(), false), llvm::GlobalValue::InternalLinkage, "__EmptyFunc__", this->Module);
    this->EmptyBB = llvm::BasicBlock::Create(GlobalContext, "__EmptyBB__", this->EmptyFunc);

    //Generate code
    Root.codegen(*this);

    GlobalBuilder.SetInsertPoint(this->EmptyBB);

    this->EmptyBB->eraseFromParent();
    this->EmptyFunc->eraseFromParent();

    //Delete symbol table
    this->PopSymbolTable();
    delete this->StructTyTable; this->StructTyTable = NULL;
}

void Generator::OptimizeIRCode(const std::string& OptimizationLevel) {
    //Run optimization
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGSCCAM;
    llvm::ModuleAnalysisManager MAM;
    llvm::PassBuilder PB;

    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGSCCAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGSCCAM, MAM);

    const llvm::OptimizationLevel* OptLevel;
    if (OptimizationLevel == "O0")
        OptLevel = &llvm::OptimizationLevel::O0;
    else if (OptimizationLevel == "O1")
        OptLevel = &llvm::OptimizationLevel::O1;
    else if (OptimizationLevel == "O2")
        OptLevel = &llvm::OptimizationLevel::O2;
    else if (OptimizationLevel == "O3")
        OptLevel = &llvm::OptimizationLevel::O3;
    else if (OptimizationLevel == "Os")
        OptLevel = &llvm::OptimizationLevel::Os;
    else if (OptimizationLevel == "Oz")
        OptLevel = &llvm::OptimizationLevel::Oz;
    else {
        throw std::logic_error("Invalid optimization level: " + OptimizationLevel);
        return;
    }
    llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(*OptLevel);
    //Optimize the IR code
    MPM.run(*this->Module, MAM);
}

bool Generator::OutputIR(std::string FileName) {
    std::error_code err;
    llvm::raw_fd_ostream Out(FileName, err);
    Out << "===================  IR Code  ===================\n";
    this->Module->print(Out, NULL);
    Out << "\n===================  Errors  ====================\n";
    if (llvm::verifyModule(*this->Module, &Out) == 0) {
        Out << "No IR error.\n";
        return true;
    }
    else {
        Out << "IR Error occurs.\n";
        return false;
    }
}

void Generator::GenObjectCode(std::string FileName) {
    auto target = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(target, Error);
    if (!Target) {
        throw std::runtime_error(Error);
        return;
    }

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(target, "generic", "", opt, RM);
    Module->setDataLayout(TargetMachine->createDataLayout());
    Module->setTargetTriple(target);
    std::error_code err;
    llvm::raw_fd_ostream ostr(FileName, err, llvm::sys::fs::OF_None);
    if (err) {
        throw std::runtime_error("Can not open file: " + err.message());
        return;
    }

    llvm::legacy::PassManager pm;
    if (TargetMachine->addPassesToEmitFile(pm, ostr, nullptr, llvm::CGFT_ObjectFile)) {
        throw std::runtime_error("TargetMachine emit file failed");
        return;
    }
    pm.run(*Module);
    ostr.flush();
}