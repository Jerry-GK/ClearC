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

extern llvm::LLVMContext GlobalContext;

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

    //Remove last symbol table
    void PopSymbolTable(void);

    //Get function by name
    ast::MyFunction* FindFunction(std::string Name);

    //Add a function to the current symbol table
    AddFunctionReseult AddFunction(std::string Name, ast::MyFunction* Function);

    //Find type
    ast::MyType* FindType(std::string Name);

    //Add a type to the current symbol table
    //return false if an old type exists
    bool AddType(std::string Name, ast::MyType* Type);

    //Find variable
    ast::MyValue* FindVariable(std::string Name);

    //Add a variable to the current symbol table
    bool AddVariable(std::string Name, ast::MyValue* Variable);

    //Find struct type
    ast::StructType* FindStructType(llvm::StructType* Ty1);

    //Add a struct type to the current symbol table
    bool AddStructType(llvm::StructType* Ty1, ast::StructType* Ty2);

    //Set current function
    void SetCurFunction(ast::MyFunction* Func);

    //Get the current function
    ast::MyFunction* GetCurFunction(void);

    //Enter a loop
    void EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB);

    //Leave the current loop
    void LeaveLoop(void);

    //Get the destination block for "continue" statements
    llvm::BasicBlock* GetContinueBlock(void);

    //Get the destination block for "break" statements
    llvm::BasicBlock* GetBreakBlock(void);

    //Get the empty block(global variable declaration, always empty)
    llvm::BasicBlock* GetEmptyBB(void);

    //Generate IR code(in Module)
    void GenerateIRCode(ast::Program& Root);

    //Optimize IR code
    void OptimizeIRCode(const std::string& OptimizationLevel = "");

    //Output IR code to file
    bool OutputIR(std::string FileName);

    //Output object code(to file)
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
    ast::MyFunction* CurFunction;						        //Current function
    StructTable* StructTyTable;						            //Struct table
    std::vector<SymbolTable*> SymbolTableStack;			        //Symbol table
    std::vector<llvm::BasicBlock*> ContinueBlockStack;	        //Blocks for "continue" statement
    std::vector<llvm::BasicBlock*> BreakBlockStack;		        //Blocks for "break" statement
    llvm::BasicBlock* EmptyBB;                                  //Empty block for global variable declaration
    llvm::Function* EmptyFunc;                                   //Empty function for EmptyBB
};