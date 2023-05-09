#include "codegen.hpp"

//The global llvm context.
llvm::LLVMContext Context;

//A helper object that makes it easy to generate LLVM instructions.
//It keeps track of the current place to insert instructions and
//has methods to create new instructions.
llvm::IRBuilder<> IRBuilder(Context);

//Constructor
CodeGenerator::CodeGenerator(void) :
    Module(new llvm::Module("main", Context)),
    DL(new llvm::DataLayout(Module)),
    CurFunction(NULL),
    StructTyTable(NULL),
    SymbolTableStack(),
    ContinueBlockStack(),
    BreakBlockStack(),
    EmptyBB(NULL),
    EmptyFunc(NULL)
{}

//Sizeof()
llvm::TypeSize CodeGenerator::GetTypeSize(llvm::Type* Type) {
    return this->DL->getTypeAllocSize(Type);
}

//Create and push an empty symbol table
void CodeGenerator::PushSymbolTable(void) {
    this->SymbolTableStack.push_back(new SymbolTable);
}

//Remove the last symbol table
void CodeGenerator::PopSymbolTable(void) {
    if (this->SymbolTableStack.size() == 0) return;
    delete this->SymbolTableStack.back();
    this->SymbolTableStack.pop_back();
}

//Find the llvm::Function* instance for the given name
llvm::Function* CodeGenerator::FindFunction(std::string Name) {
    if (this->SymbolTableStack.size() == 0) return NULL;
    for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
        auto mapIter = (*TableIter)->find(Name);
        if (mapIter != (*TableIter)->end())
            return mapIter->second.GetFunction();
    }
    return NULL;
}

//Add a function to the current symbol table
//If an old value exists (i.e., conflict), return false
AddFunctionReseult CodeGenerator::AddFunction(std::string Name, llvm::Function* Function) {
    if (this->SymbolTableStack.size() == 0) return ADDFUNC_ERROR;
    auto TopTable = this->SymbolTableStack.back();
    auto mapIter = TopTable->find(Name);
    if (mapIter != TopTable->end()) {
        if (mapIter->second.GetFunction())
            return ADDFUNC_DECLARED;
        else
            return ADDFUNC_NAMECONFLICT;
    }
    TopTable->insert({ Name, Symbol(Function) });
    return ADDFUNC_SUCCESS;
}

//Find the llvm::Type* instance for the given name
llvm::Type* CodeGenerator::FindType(std::string Name) {
    if (this->SymbolTableStack.size() == 0) return NULL;
    for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
        auto mapIter = (*TableIter)->find(Name);
        if (mapIter != (*TableIter)->end())
            return mapIter->second.GetType();
    }
    return NULL;
}

//Add a type to the current symbol table
//If an old value exists (i.e., conflict), return false
bool CodeGenerator::AddType(std::string Name, llvm::Type* Type) {
    if (this->SymbolTableStack.size() == 0) return false;
    auto TopTable = this->SymbolTableStack.back();
    auto mapIter = TopTable->find(Name);
    if (mapIter != TopTable->end())
        return false;
    TopTable->insert({ Name, Symbol(Type) });
    return true;
}

//Find variable
ExprValue* CodeGenerator::FindVariable(std::string Name) {
    if (this->SymbolTableStack.size() == 0) return NULL;
    for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
        auto mapIter = (*TableIter)->find(Name);
        if (mapIter != (*TableIter)->end())
            return mapIter->second.GetVariable();
    }
    return NULL;
}

//Add a variable to the current symbol table
//If an old value exists (i.e., conflict), return false
bool CodeGenerator::AddVariable(std::string Name, ExprValue Variable) {
    if (this->SymbolTableStack.size() == 0) return false;
    auto TopTable = this->SymbolTableStack.back();
    auto mapIter = TopTable->find(Name);
    if (mapIter != TopTable->end())
        return false;
    TopTable->insert({ Name, Symbol(&Variable) });
    return true;
}

//Find the ast::StructType* instance according to the llvm::StructType* instance
ast::StructType* CodeGenerator::FindStructType(llvm::StructType* Ty1) {
    auto mapIter = this->StructTyTable->find(Ty1);
    if (mapIter != this->StructTyTable->end())
        return mapIter->second;
    return NULL;
}

//Add a <llvm::StructType*, ast::StructType*> mapping
bool CodeGenerator::AddStructType(llvm::StructType* Ty1, ast::StructType* Ty2) {
    auto mapIter = this->StructTyTable->find(Ty1);
    if (mapIter != this->StructTyTable->end())
        return false;
    (*this->StructTyTable)[Ty1] = Ty2;
    return true;
}

//Set current function
void CodeGenerator::SetCurFunction(llvm::Function* Func) {
    this->CurFunction = Func;
}

//Get the current function
llvm::Function* CodeGenerator::GetCurrentFunction(void) {
    return this->CurFunction;
}

//Called whenever entering a loop
void CodeGenerator::EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB) {
    this->ContinueBlockStack.push_back(ContinueBB);
    this->BreakBlockStack.push_back(BreakBB);
}

//Called whenever leaving a loop
void CodeGenerator::LeaveLoop(void) {
    if (this->ContinueBlockStack.size() == 0 || this->BreakBlockStack.size() == 0) return;
    this->ContinueBlockStack.pop_back();
    this->BreakBlockStack.pop_back();
}

//Get the destination block for "continue" statements
llvm::BasicBlock* CodeGenerator::GetContinueBlock(void) {
    if (this->ContinueBlockStack.size() > 0)
        return this->ContinueBlockStack.back();
    else
        return NULL;
}

//Get the destination block for "break" statements
llvm::BasicBlock* CodeGenerator::GetBreakBlock(void) {
    if (this->BreakBlockStack.size() > 0)
        return this->BreakBlockStack.back();
    else
        return NULL;
}

llvm::BasicBlock* CodeGenerator::GetEmptyBB(void) {
    return this->EmptyBB;
}
void CodeGenerator::GenerateIRCode(ast::Program& Root, const std::string& OptimizeLevel) {
    //Initialize symbol table
    this->StructTyTable = new StructTypeTable;
    this->PushSymbolTable();

    //Create a temp function and a temp block for global instruction code generation
    this->EmptyFunc = llvm::Function::Create(llvm::FunctionType::get(IRBuilder.getVoidTy(), false), llvm::GlobalValue::InternalLinkage, "__EmptyFunc__", this->Module);
    this->EmptyBB = llvm::BasicBlock::Create(Context, "__EmptyBB__", this->EmptyFunc);

    //Generate code
    Root.codegen(*this);

    //add a terminater for the temp block
    IRBuilder.SetInsertPoint(this->EmptyBB);
    //IRBuilder.CreateRetVoid();
    //Delete
    this->EmptyBB->eraseFromParent();
    this->EmptyFunc->eraseFromParent();

    //Delete symbol table
    this->PopSymbolTable();
    delete this->StructTyTable; this->StructTyTable = NULL;
}

bool CodeGenerator::OutputIR(std::string FileName) {
    std::error_code EC;
    llvm::raw_fd_ostream Out(FileName, EC);
    Out << "*******************  IR Code  ********************\n";
    this->Module->print(Out, NULL);
    Out << "\n*****************  Verification  *****************\n";
    if (llvm::verifyModule(*this->Module, &Out) == 0) {
        Out << "No IR error.\n";
        return true;
    }
    else {
        Out << "IR Error occurs.\n";
        return false;
    }
}

void CodeGenerator::GenObjectCode(std::string FileName) {
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    if (!Target) {
        throw std::runtime_error(Error);
        return;
    }
    auto CPU = "generic";
    auto Features = "";
    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    Module->setDataLayout(TargetMachine->createDataLayout());
    Module->setTargetTriple(TargetTriple);
    std::error_code EC;
    llvm::raw_fd_ostream Dest(FileName, EC, llvm::sys::fs::OF_None);
    if (EC) {
        throw std::runtime_error("Could not open file: " + EC.message());
        return;
    }
    auto FileType = llvm::CGFT_ObjectFile;
    llvm::legacy::PassManager PM;
    if (TargetMachine->addPassesToEmitFile(PM, Dest, nullptr, FileType)) {
        throw std::runtime_error("TargetMachine can't emit a file of this type");
        return;
    }
    PM.run(*Module);
    Dest.flush();
}