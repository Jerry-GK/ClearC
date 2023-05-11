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
#include <llvm/IR/LegacyPassManager.h>
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

class CodeGenerator;

class ExprValue {
public:
    explicit ExprValue(void) :
        Value(nullptr),
        Name(""),
        IsConst(false),
        IsInnerConstPointer(false),
        IsPointingToInnerConst(false) {}
    explicit ExprValue(llvm::Value* _v) :
        Value(_v),
        Name(""),
        IsConst(false),
        IsInnerConstPointer(false),
        IsPointingToInnerConst(false) {}
    explicit ExprValue(llvm::Value* _v, std::string _n, bool _isc, bool _iicp, bool _ipic = false) :
        Value(_v),
        Name(_n),
        IsConst(_isc),
        IsInnerConstPointer(_iicp),
        IsPointingToInnerConst(_ipic) {}

    ExprValue(const ExprValue& other) :
        Value(other.Value),
        Name(other.Name),
        IsConst(other.IsConst),
        IsInnerConstPointer(other.IsInnerConstPointer),
        IsPointingToInnerConst(other.IsPointingToInnerConst) {}

    ~ExprValue(void) {}

    llvm::Value* Value;
    std::string Name;
    bool IsConst;
    bool IsInnerConstPointer;
    bool IsPointingToInnerConst;
};


namespace ast {

    class Node;

    /*** Root ***/
    class Program;

    /*** Declarations ***/
    class Decl;
    using Decls = std::vector<Decl*>;
    class FuncDecl;
    class Arg;
    class ArgList;
    class FuncBody;
    class VarDecl;
    class VarInit;
    using VarList = std::vector<VarInit*>;
    class TypeDecl;

    /*** Variable Types ***/
    class VarType;
    class DefinedType;
    class PointerType;
    class ArrayType;
    class StructType;
    class FieldDecl;
    using FieldDecls = std::vector<FieldDecl*>;
    using MemList = std::vector<std::string>;

    /*** Statements ***/
    class Stmt;
    using Stmts = std::vector<Stmt*>;
    class IfStmt;
    class ForStmt;
    class SwitchStmt;
    class CaseStmt;
    using CaseList = std::vector<CaseStmt*>;
    class BreakStmt;
    class ContinueStmt;
    class ReturnStmt;
    class Block;

    /*** Expressions ***/
    class Expr;
    class Subscript;
    class SizeOf;
    class FunctionCall;
    using ExprList = std::vector<Expr*>;
    class StructReference;
    class StructDereference;
    class UnaryPlus;
    class UnaryMinus;
    class TypeCast;
    class PrefixInc;
    class PostfixInc;
    class PrefixDec;
    class PostfixDec;
    class Indirection;
    class AddressOf;
    class LogicNot;
    class BitwiseNot;
    class Division;
    class Multiplication;
    class Modulo;
    class Addition;
    class Subtraction;
    class LeftShift;
    class RightShift;
    class LogicGT;
    class LogicGE;
    class LogicLT;
    class LogicLE;
    class LogicEQ;
    class LogicNEQ;
    class BitwiseAND;
    class BitwiseXOR;
    class BitwiseOR;
    class LogicAND;
    class LogicOR;
    class TernaryCondition;
    class DirectAssign;
    class DivAssign;
    class MulAssign;
    class ModAssign;
    class AddAssign;
    class SubAssign;
    class SHLAssign;
    class SHRAssign;
    class BitwiseANDAssign;
    class BitwiseXORAssign;
    class BitwiseORAssign;
    class CommaExpr;
    class Variable;
    class Constant;
    class GlobalString;

    class MyFunction;
}

//Class definitions
namespace ast {
    //Pure virtual class for ast node
    class Node {
    public:
        Node(void) {}
        ~Node(void) {}
        virtual ExprValue codegen(CodeGenerator& Gen) = 0;
        virtual std::string astJson() = 0;
    };

    //The root node of ast
    class Program : public Node {
    public:
        Decls* _Decls;

        Program(Decls* _Decls) :_Decls(_Decls) {}
        ~Program(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
        void genHTML(std::string FileName);
    };

    //Pure virtual class for statement
    class Stmt : public Node {
    public:
        Stmt(void) {}
        ~Stmt(void) {}
        virtual ExprValue codegen(CodeGenerator& Gen) = 0;
        virtual std::string astJson() = 0;
    };

    //Pure virtual class for Declarations
    class Decl : public Stmt {
    public:
        Decl(void) {}
        ~Decl(void) {}
        virtual ExprValue codegen(CodeGenerator& Gen) = 0;
        virtual std::string astJson() = 0;
    };

    //Function declaration
    class FuncDecl : public Decl {
    public:
        VarType* _RetType;
        std::string _Name;

        ArgList* _ArgList;

        //Funcbody is NULL for declaration
        FuncBody* _FuncBody;

        FuncDecl(VarType* __RetType, const std::string& __Name, ArgList* __ArgList, FuncBody* __FuncBody = NULL) :
            _RetType(__RetType), _Name(__Name), _ArgList(__ArgList), _FuncBody(__FuncBody) {}
        ~FuncDecl(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Function body
    class FuncBody : public Node {
    public:
        Stmts* _Content;

        FuncBody(Stmts* __Content) :_Content(__Content) {}
        ~FuncBody(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Variable declaration
    class VarDecl : public Decl {
    public:
        VarType* _VarType;
        VarList* _VarList;

        VarDecl(VarType* __VarType, VarList* __VarList) :
            _VarType(__VarType), _VarList(__VarList) {}
        ~VarDecl(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //A variable in one variable declaration
    class VarInit : public Node {
    public:
        //The name of the variable
        std::string _Name;
        //The initial value (if any) of this variable
        Expr* _InitialExpr;

        VarInit(const std::string& __Name) :
            _Name(__Name), _InitialExpr(NULL) {}
        VarInit(const std::string& __Name, Expr* __InitialExpr) :
            _Name(__Name), _InitialExpr(__InitialExpr) {}
        ~VarInit(void) {}
        ExprValue codegen(CodeGenerator& Gen) { return ExprValue(); }
        std::string astJson();
    };

    //Type declaration
    class TypeDecl : public Decl {
    public:
        //Its alias
        std::string _Alias;
        //Variable type
        VarType* _VarType;

        TypeDecl(const std::string& __Alias, VarType* __VarType) :
            _Alias(__Alias), _VarType(__VarType) {}
        ~TypeDecl(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Base class for variable type
    class VarType : public Node {
    public:
        //Whether this type is const
        bool _isConst;
        //Its LLVM type. It is initialized as NULL, and generated by function GetLLVMType.
        llvm::Type* _LLVMType;

        VarType(void) : _isConst(false), _LLVMType(NULL) {}
        ~VarType(void) {}
        //Set this variable type to be constant.
        void SetConst(void) {
            this->_isConst = true;
        }
        //Return the corresponding instance of llvm::Type*.
        //Meanwhile, it will update _LLVMType.
        virtual llvm::Type* GetLLVMType(CodeGenerator& Gen) = 0;
        //VarType class don't need an actual codegen function
        ExprValue codegen(CodeGenerator& Gen) { return ExprValue(); }
        //Determine class type
        virtual bool isBuiltInType(void) = 0;
        virtual bool isDefinedType(void) = 0;
        virtual bool isPointerType(void) = 0;
        virtual bool isArrayType(void) = 0;
        virtual bool isStructType(void) = 0;
        virtual std::string astJson() = 0;
    };

    //Built-in type
    class BuiltInType : public VarType {
    public:
        //Enum of built-in type
        enum TypeID {
            _Bool,
            _Short,
            _Int,
            _Long,
            _Char,
            _Float,
            _Double,
            _Void
        };
        //Type ID
        TypeID _Type;

        BuiltInType(TypeID __Type) : _Type(__Type) {}
        ~BuiltInType(void) {}
        //Return the corresponding instance of llvm::Type*.
        //Meanwhile, it will update _LLVMType.
        llvm::Type* GetLLVMType(CodeGenerator& Gen);
        //Determine class type
        bool isBuiltInType(void) { return true; }
        bool isDefinedType(void) { return false; }
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return false; }
        std::string astJson();
    };

    //Defined type. Use this class when only an identifier is given.
    class DefinedType : public VarType {
    public:
        //Its name.
        std::string _Name;

        DefinedType(const std::string& __Name) : _Name(__Name) {}
        ~DefinedType(void) {}
        //Return the corresponding instance of llvm::Type*.
        //Meanwhile, it will update _LLVMType.
        llvm::Type* GetLLVMType(CodeGenerator& Gen);
        //Determine class type
        bool isBuiltInType(void) { return false; }
        bool isDefinedType(void) { return true; }
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return false; }
        std::string astJson();
    };

    //Pointer type.
    class PointerType : public VarType {
    public:
        //Base type.
        //For example, the base type for "int ptr" is "int"
        VarType* _BaseType;

        PointerType(VarType* __BaseType) : _BaseType(__BaseType) {}
        ~PointerType(void) {}
        //Return the corresponding instance of llvm::Type*.
        //Meanwhile, it will update _LLVMType.
        llvm::Type* GetLLVMType(CodeGenerator& Gen);
        //Determine class type
        bool isBuiltInType(void) { return false; }
        bool isDefinedType(void) { return false; }
        bool isPointerType(void) { return true; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return false; }

        bool isInnerConst(void) { return _BaseType->_isConst; }
        std::string astJson();
    };

    //Array Type
    class ArrayType : public VarType {
    public:
        //Base type and array length
        //For example, the base type and array length for "int[20]"
        //is "int" and 20, respectively.
        VarType* _BaseType;
        size_t _Length;

        ArrayType(VarType* __BaseType, size_t __Length) : _BaseType(__BaseType), _Length(__Length) {}
        ArrayType(VarType* __BaseType) : _BaseType(__BaseType), _Length(0) {}
        ~ArrayType(void) {}
        //Return the corresponding instance of llvm::Type*.
        //Meanwhile, it will update _LLVMType.
        llvm::Type* GetLLVMType(CodeGenerator& Gen);
        //Determine class type
        bool isBuiltInType(void) { return false; }
        bool isDefinedType(void) { return false; }
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return true; }
        bool isStructType(void) { return false; }
        std::string astJson();
    };

    //Struct Type
    class StructType : public VarType {
    public:
        //Struct body
        FieldDecls* _StructBody;

        StructType(FieldDecls* __StructBody) : _StructBody(__StructBody) {}
        ~StructType(void) {}
        //This is only called if the struct type is an anonymous one,
        //or its LLVM type is already generated.
        llvm::Type* GetLLVMType(CodeGenerator& Gen);
        //Return the corresponding instance of llvm::Type*, as an identified struct type
        //Meanwhile, it will update _LLVMType.
        llvm::Type* GenerateLLVMTypeHead(CodeGenerator& Gen, const std::string& __Name = "<unnamed>");
        llvm::Type* GenerateLLVMTypeBody(CodeGenerator& Gen);
        //Get the element index according to its name
        size_t GetElementIndex(const std::string& __MemName);
        //Determine class type
        bool isBuiltInType(void) { return false; }
        bool isDefinedType(void) { return false; }
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return true; }
        std::string astJson();
    };

    //Field declaration for struct type
    class FieldDecl : public Decl {
    public:
        //The variable type for this declaration
        VarType* _VarType;
        //The list of variable names for this declaration
        MemList* _MemList;

        FieldDecl(VarType* __VarType, MemList* __MemList) :_VarType(__VarType), _MemList(__MemList) {}
        ~FieldDecl(void) {}
        //FieldDecl class don't need an actual codegen function
        ExprValue codegen(CodeGenerator& Gen) { return ExprValue(); }
        std::string astJson();
    };

    //Function argument list
    class ArgList : public std::vector<Arg*>, public Node {
    public:
        //Set true if the argument list contains "..."
        bool _VarArg;
        void SetVarArg(void) { this->_VarArg = true; }

        ArgList(void) : _VarArg(false) {}
        ~ArgList(void) {}
        //ArgList class don't need an actual codegen function
        ExprValue codegen(CodeGenerator& Gen) { return ExprValue(); }
        std::string astJson();
    };

    //Function argument
    class Arg : public Node {
    public:
        //Its type
        VarType* _VarType;
        //Its name (if any)
        std::string _Name;

        Arg(VarType* __VarType, const std::string& __Name = "") :
            _VarType(__VarType), _Name(__Name) {}
        ~Arg(void) {}
        //Arg class don't need an actual codegen function
        ExprValue codegen(CodeGenerator& Gen) { return ExprValue(); }
        std::string astJson();
    };

    //Statement block
    class Block : public Stmt {
    public:
        //Its content
        Stmts* _Content;

        Block(Stmts* __Content) :_Content(__Content) {}
        ~Block(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //If statement
    class IfStmt : public Stmt {
    public:
        //Branch condition, then-clause and else-clause
        Expr* _Condition;
        Block* _Then;
        Block* _Else;

        IfStmt(Expr* __Condition, Block* __Then, Block* __Else = NULL) : _Condition(__Condition), _Then(__Then), _Else(__Else) {}
        ~IfStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //For statement
    class ForStmt : public Stmt {
    public:
        //For-initial, loop condition, for-tail and loop body
        Stmt* _Initial;
        Expr* _Condition;
        Expr* _Tail;
        Block* _LoopBody;

        ForStmt(Stmt* __Initial, Expr* __Condition, Expr* __Tail, Block* __LoopBody) :
            _Initial(__Initial), _Condition(__Condition), _Tail(__Tail), _LoopBody(__LoopBody) {}
        ~ForStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Switch statement
    class SwitchStmt : public Stmt {
    public:
        //Switch-matcher and case statement list
        Expr* _Matcher;
        CaseList* _CaseList;

        SwitchStmt(Expr* __Matcher, CaseList* __CaseList) : _Matcher(__Matcher), _CaseList(__CaseList) {}
        ~SwitchStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Case statement in switch statement
    class CaseStmt : public Stmt {
    public:
        //Case condition. Set NULL if the condition is "default".
        Expr* _Condition;
        //The statements to be excuted.
        Stmts* _Content;

        CaseStmt(Expr* __Condition, Stmts* __Content) : _Condition(__Condition), _Content(__Content) {}
        ~CaseStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Continue statement
    class ContinueStmt : public Stmt {
    public:
        ContinueStmt(void) {}
        ~ContinueStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Break statement
    class BreakStmt : public Stmt {
    public:
        BreakStmt(void) {}
        ~BreakStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Return statement
    class ReturnStmt : public Stmt {
    public:
        //The expression to be returned (if any)
        Expr* _RetVal;
        ReturnStmt(Expr* __RetVal = NULL) : _RetVal(__RetVal) {}
        ~ReturnStmt(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        std::string astJson();
    };

    //Pure virtual class for expression
    class Expr : public Stmt {
    public:
        Expr(void) {}
        ~Expr(void) {}
        //This function is used to get the "value" of the expression.
        virtual ExprValue codegen(CodeGenerator& Gen) = 0;
        //This function is used to get the "pointer" of the instance(must be Expr of left).
        //It is used to implement the "left value" in C language,
        //e.g., the LHS of the assignment.
        //right value cannot get ptr
        virtual ExprValue codegenPtr(CodeGenerator& Gen) = 0;
        virtual std::string astJson() = 0;
    };

    //Subscript, e.g. a[10]
    class Subscript : public Expr {
    public:
        Expr* _Array;
        Expr* _Index;
        Subscript(Expr* __Array, Expr* __Index) : _Array(__Array), _Index(__Index) {}
        ~Subscript(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    //Operator sizeof() in C
    class SizeOf : public Expr {
    public:
        Expr* _Arg1;
        VarType* _Arg2;
        std::string _Arg3;
        SizeOf(Expr* __Arg1) : _Arg1(__Arg1), _Arg2(NULL), _Arg3("") {}
        SizeOf(VarType* __Arg2) : _Arg1(NULL), _Arg2(__Arg2), _Arg3("") {}
        SizeOf(const std::string& __Arg3) : _Arg1(NULL), _Arg2(NULL), _Arg3(__Arg3) {}
        ~SizeOf(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    //Function call
    class FunctionCall : public Expr {
    public:
        std::string _FuncName;
        ExprList* _ArgList;
        FunctionCall(const std::string& __FuncName, ExprList* __ArgList) : _FuncName(__FuncName), _ArgList(__ArgList) {}
        ~FunctionCall(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class StructReference : public Expr {
    public:
        Expr* _Struct;
        std::string _MemName;
        StructReference(Expr* __Struct, const std::string& __MemName) : _Struct(__Struct), _MemName(__MemName) {}
        ~StructReference(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class StructDereference : public Expr {
    public:
        Expr* _StructPtr;
        std::string _MemName;
        StructDereference(Expr* __StructPtr, const std::string& __MemName) : _StructPtr(__StructPtr), _MemName(__MemName) {}
        ~StructDereference(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class UnaryPlus : public Expr {
    public:
        Expr* _Operand;
        UnaryPlus(Expr* __Operand) : _Operand(__Operand) {}
        ~UnaryPlus(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class UnaryMinus : public Expr {
    public:
        Expr* _Operand;
        UnaryMinus(Expr* __Operand) : _Operand(__Operand) {}
        ~UnaryMinus(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class TypeCast : public Expr {
    public:
        VarType* _VarType;
        Expr* _Operand;
        TypeCast(VarType* __VarType, Expr* __Operand) : _VarType(__VarType), _Operand(__Operand) {}
        ~TypeCast(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class PrefixInc : public Expr {
    public:
        Expr* _Operand;
        PrefixInc(Expr* __Operand) : _Operand(__Operand) {}
        ~PrefixInc(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class PostfixInc : public Expr {
    public:
        Expr* _Operand;
        PostfixInc(Expr* __Operand) : _Operand(__Operand) {}
        ~PostfixInc(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class PrefixDec : public Expr {
    public:
        Expr* _Operand;
        PrefixDec(Expr* __Operand) : _Operand(__Operand) {}
        ~PrefixDec(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class PostfixDec : public Expr {
    public:
        Expr* _Operand;
        PostfixDec(Expr* __Operand) : _Operand(__Operand) {}
        ~PostfixDec(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Indirection : public Expr {
    public:
        Expr* _Operand;
        Indirection(Expr* __Operand) : _Operand(__Operand) {}
        ~Indirection(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class AddressOf : public Expr {
    public:
        Expr* _Operand;
        AddressOf(Expr* __Operand) : _Operand(__Operand) {}
        ~AddressOf(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicNot : public Expr {
    public:
        Expr* _Operand;
        LogicNot(Expr* __Operand) : _Operand(__Operand) {}
        ~LogicNot(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseNot : public Expr {
    public:
        Expr* _Operand;
        BitwiseNot(Expr* __Operand) : _Operand(__Operand) {}
        ~BitwiseNot(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Division : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        Division(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~Division(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Multiplication : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        Multiplication(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~Multiplication(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Modulo : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        Modulo(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~Modulo(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Addition : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        Addition(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~Addition(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Subtraction : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        Subtraction(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~Subtraction(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LeftShift : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LeftShift(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LeftShift(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class RightShift : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        RightShift(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~RightShift(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicGT : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicGT(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicGT(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicGE : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicGE(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicGE(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicLT : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicLT(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicLT(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicLE : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicLE(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicLE(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicEQ : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicEQ(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicEQ(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicNEQ : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicNEQ(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicNEQ(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseAND : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        BitwiseAND(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~BitwiseAND(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseXOR : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        BitwiseXOR(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~BitwiseXOR(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseOR : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        BitwiseOR(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~BitwiseOR(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicAND : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicAND(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicAND(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class LogicOR : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        LogicOR(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~LogicOR(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class TernaryCondition : public Expr {
    public:
        Expr* _Condition;
        Expr* _Then;
        Expr* _Else;
        TernaryCondition(Expr* __Condition, Expr* __Then, Expr* __Else) : _Condition(__Condition), _Then(__Then), _Else(__Else) {}
        ~TernaryCondition(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class DirectAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        DirectAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~DirectAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class DivAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        DivAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~DivAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class MulAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        MulAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~MulAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class ModAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        ModAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~ModAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class AddAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        AddAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~AddAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class SubAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        SubAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~SubAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class SHLAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        SHLAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~SHLAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };
    class SHRAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        SHRAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~SHRAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseANDAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        BitwiseANDAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~BitwiseANDAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseXORAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        BitwiseXORAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~BitwiseXORAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class BitwiseORAssign : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        BitwiseORAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~BitwiseORAssign(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class CommaExpr : public Expr {
    public:
        Expr* _LHS;
        Expr* _RHS;
        CommaExpr(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
        ~CommaExpr(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Variable : public Expr {
    public:
        std::string _Name;
        Variable(const std::string& __Name) : _Name(__Name) {}
        ~Variable(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class Constant : public Expr {
    public:
        BuiltInType::TypeID _Type;
        bool _Bool;
        char _Character;
        int _Integer;
        double _Real;
        Constant(bool __Bool) :
            _Type(BuiltInType::TypeID::_Bool), _Bool(__Bool), _Character('\0'), _Integer(0), _Real(0.0) {}
        Constant(char __Character) :
            _Type(BuiltInType::TypeID::_Char), _Bool(false), _Character(__Character), _Integer(0), _Real(0.0) {}
        Constant(int __Integer) :
            _Type(BuiltInType::TypeID::_Int), _Bool(false), _Character('\0'), _Integer(__Integer), _Real(0.0) {}
        Constant(double __Real) :
            _Type(BuiltInType::TypeID::_Double), _Bool(false), _Character('\0'), _Integer(0), _Real(__Real) {}
        ~Constant(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };

    class GlobalString : public Constant {
    public:
        std::string _Content;
        GlobalString(const std::string& __Content) : Constant(0), _Content(__Content) {}
        ~GlobalString(void) {}
        ExprValue codegen(CodeGenerator& Gen);
        ExprValue codegenPtr(CodeGenerator& Gen);
        std::string astJson();
    };


    // to record ArgList
    class MyFunction {
    public:
        explicit MyFunction(llvm::Function* _f, ArgList* _a) :
            LLVMFunc(_f),
            Args(_a) {}

        llvm::Function* LLVMFunc;
        ArgList* Args;
    };

}