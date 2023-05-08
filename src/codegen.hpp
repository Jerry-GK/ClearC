#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <exception>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Verifier.h>
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
extern llvm::LLVMContext Context;

//A helper object that makes it easy to generate LLVM instructions.
//It keeps track of the current place to insert instructions and
//has methods to create new instructions.
extern llvm::IRBuilder<> IRBuilder;


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
    llvm::Function* FindFunction(std::string Name);

    //Add a function to the current symbol table
    //If an old value exists (i.e., conflict), return false
    bool AddFunction(std::string Name, llvm::Function* Function);

    //Find the llvm::Type* instance for the given name
    llvm::Type* FindType(std::string Name);

    //Add a type to the current symbol table
    //If an old value exists (i.e., conflict), return false
    bool AddType(std::string Name, llvm::Type* Type);

    //Find variable
    llvm::Value* FindVariable(std::string Name);

    //Add a variable to the current symbol table
    //If an old value exists (i.e., conflict), return false
    bool AddVariable(std::string Name, llvm::Value* Variable);

    //Find constant
    llvm::Value* FindConstant(std::string Name);

    //Add a constant to the current symbol table
    //If an old value exists (i.e., conflict), return false
    bool AddConstant(std::string Name, llvm::Value* Constant);

    //Find the ast::StructType* instance according to the llvm::StructType* instance
    ast::StructType* FindStructType(llvm::StructType* Ty1);

    //Add a <llvm::StructType*, ast::StructType*> mapping
    bool AddStructType(llvm::StructType* Ty1, ast::StructType* Ty2);

    //Set current function
    void SetCurFunction(llvm::Function* Func);

    //Get the current function
    llvm::Function* GetCurrentFunction(void);

    //Called whenever entering a loop
    void EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB);

    //Called whenever leaving a loop
    void LeaveLoop(void);

    //Get the destination block for "continue" statements
    llvm::BasicBlock* GetContinueBlock(void);

    //Get the destination block for "break" statements
    llvm::BasicBlock* GetBreakBlock(void);

    //Exchange the current insert point with TmpBB
    void ExchangeInsertPointWithTmpBB(void);

    void GenerateIRCode(ast::Program& Root, const std::string& OptimizeLevel = "");

    void OutputIR(std::string FileName);

    void GenObjectCode(std::string FileName);

private:
    class Symbol {
    public:
        Symbol(void) : Content(NULL), Type(UNDEFINED) {}
        Symbol(llvm::Function* Func) : Content(Func), Type(FUNCTION) {}
        Symbol(llvm::Type* Ty) : Content(Ty), Type(TYPE) {}
        Symbol(llvm::Value* Value, bool isConst) : Content(Value), Type(isConst ? CONSTANT : VARIABLE) {}
        llvm::Function* GetFunction(void) { return this->Type == FUNCTION ? (llvm::Function*)Content : NULL; }
        llvm::Type* GetType(void) { return this->Type == TYPE ? (llvm::Type*)Content : NULL; }
        llvm::Value* GetVariable(void) { return this->Type == VARIABLE ? (llvm::Value*)Content : NULL; }
        llvm::Value* GetConstant(void) { return this->Type == CONSTANT ? (llvm::Value*)Content : NULL; }
    private:
        void* Content;
        enum {
            FUNCTION,
            TYPE,
            VARIABLE,
            CONSTANT,
            UNDEFINED
        } Type;
    };
    using StructTypeTable = std::map<llvm::StructType*, ast::StructType*>;
    using SymbolTable = std::map<std::string, Symbol>;

private:
    llvm::DataLayout* DL;								//Data layout
    llvm::Function* CurFunction;						//Current function
    StructTypeTable* StructTyTable;						//Struct type table
    std::vector<SymbolTable*> SymbolTableStack;			//Symbol table
    std::vector<llvm::BasicBlock*> ContinueBlockStack;	//Store blocks for "continue" statement
    std::vector<llvm::BasicBlock*> BreakBlockStack;		//Store blocks for "break" statement
    llvm::BasicBlock* TmpBB;							//Temp block for global instruction code generation
    llvm::Function* TmpFunc;							//Temp function for global instruction code generation
};