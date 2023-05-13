#pragma once
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include "ast.hpp"

//The global context.
extern llvm::LLVMContext GlobalContext;

//A helper object that makes it easy to generate LLVM instructions.
//It keeps track of the current place to insert instructions and
//has methods to create new instructions.
extern llvm::IRBuilder<> GlobalBuilder;

//return type for AddFunction()
enum AddFunctionReseult {
    ADDFUNC_SUCCESS, //Success
    ADDFUNC_DECLARED, //Already declared
    ADDFUNC_NAMECONFLICT, //Name conflict with variable or type
    ADDFUNC_ERROR //Other errors
};

class CodeGenerator {
public:
    llvm::Module* Module;
public:
    CodeGenerator(void);

    //Sizeof()
    llvm::TypeSize GetTypeSize(llvm::Type* Type);

    //Create and push an empty symbol table
    void PushSymbolTable(void);

    //Remove the last symbol table
    void PopSymbolTable(void);

    //Find the llvm::Function* instance for the given name
    ast::MyFunction* FindFunction(std::string Name);

    //Add a function to the current symbol table
    AddFunctionReseult AddFunction(std::string Name, ast::MyFunction* Function);

    //Find the llvm::Type* instance for the given name
    ast::MyType* FindType(std::string Name);

    //Add a type to the current symbol table
    //If an old value exists (i.e., conflict), return false
    bool AddType(std::string Name, ast::MyType* Type);

    //Find variable
    ast::MyValue* FindVariable(std::string Name);

    //Add a variable to the current symbol table
    //If an old value exists (i.e., conflict), return false
    bool AddVariable(std::string Name, ast::MyValue* Variable);

    //Find the ast::StructType* instance according to the llvm::StructType* instance
    ast::StructType* FindStructType(llvm::StructType* Ty1);

    //Add a <llvm::StructType*, ast::StructType*> mapping
    bool AddStructType(llvm::StructType* Ty1, ast::StructType* Ty2);

    //Set current function
    void SetCurFunction(ast::MyFunction* Func);

    //Get the current function
    ast::MyFunction* GetCurFunction(void);

    //Called whenever entering a loop
    void EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB);

    //Called whenever leaving a loop
    void LeaveLoop(void);

    //Get the destination block for "continue" statements
    llvm::BasicBlock* GetContinueBlock(void);

    //Get the destination block for "break" statements
    llvm::BasicBlock* GetBreakBlock(void);

    llvm::BasicBlock* GetEmptyBB(void);

    void GenerateIRCode(ast::Program& Root);

    void OptimizeIRCode(const std::string& OptimizationLevel = "");

    bool OutputIR(std::string FileName);

    void GenObjectCode(std::string FileName);


private:
    class Symbol {
    public:
        Symbol(void) : Val(NULL), Type(UNDEFINED) {}
        Symbol(ast::MyFunction* Func) : Val(Func), Type(FUNCTION) {}
        Symbol(ast::MyType* Ty) : Val(Ty), Type(TYPE) {}
        Symbol(ast::MyValue* MyVal) : Val(MyVal), Type(VARIABLE) {}
        ast::MyFunction* GetFunction(void) { return this->Type == FUNCTION ? (ast::MyFunction*)Val : NULL; }
        ast::MyType* GetType(void) { return this->Type == TYPE ? (ast::MyType*)Val : NULL; }
        ast::MyValue* GetVariable(void) { return this->Type == VARIABLE ? (ast::MyValue*)Val : NULL; }

    private:
        void* Val;
        enum {
            FUNCTION,
            TYPE,
            VARIABLE,
            UNDEFINED
        } Type;
    };
    using StructTable = std::map<llvm::StructType*, ast::StructType*>;
    using SymbolTable = std::map<std::string, Symbol>;

private:
    llvm::DataLayout* DataLayout;								//Data layout
    ast::MyFunction* CurFunction;						//Current function
    StructTable* StructTyTable;						//Struct type table
    std::vector<SymbolTable*> SymbolTableStack;			//Symbol table
    std::vector<llvm::BasicBlock*> ContinueBlockStack;	//Store blocks for "continue" statement
    std::vector<llvm::BasicBlock*> BreakBlockStack;		//Store blocks for "break" statement
    llvm::BasicBlock* EmptyBB;                          //Empty block for global variables
    llvm::Function* EmptyFunc;                          //Empty function for EmptyBB
};