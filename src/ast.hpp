#pragma once
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
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

class CodeGenerator;

namespace ast {
    //wrapped class for llvm types
    class MyValue;
    class MyFunction;
    class MyType;

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
    class This;
}

//Class definitions
namespace ast {
    //wrapped llvm::Value
    class MyValue {
    public:
        explicit MyValue(void) :
            Value(nullptr),
            Name(""),
            IsConst(false),
            IsInnerConstPointer(false),
            IsPointingToInnerConst(false),
            IsZeroConstant(false) {}
        explicit MyValue(llvm::Value* _v) :
            Value(_v),
            Name(""),
            IsConst(false),
            IsInnerConstPointer(false),
            IsPointingToInnerConst(false),
            IsZeroConstant(false) {}
        explicit MyValue(llvm::Value* _v, std::string _n, bool _isc, bool _iicp, bool _ipic = false) :
            Value(_v),
            Name(_n),
            IsConst(_isc),
            IsInnerConstPointer(_iicp),
            IsPointingToInnerConst(_ipic),
            IsZeroConstant(false) {}

        MyValue(const MyValue& other) :
            Value(other.Value),
            Name(other.Name),
            IsConst(other.IsConst),
            IsInnerConstPointer(other.IsInnerConstPointer),
            IsPointingToInnerConst(other.IsPointingToInnerConst),
            IsZeroConstant(other.IsZeroConstant) {}

        void SetIsZeroConstant() {
            IsZeroConstant = true;
        }

        ~MyValue(void) {}

        llvm::Value* Value;
        std::string Name;

        bool IsConst;
        bool IsInnerConstPointer;
        bool IsPointingToInnerConst;

        bool IsZeroConstant;
    };

    //Pure virtual class for ast node
    class Node {
    public:
        Node(void) {}
        ~Node(void) {}
        virtual MyValue codegen(CodeGenerator& Gen) = 0;
        virtual std::string nodeJson() = 0;
    };

    //The root node of ast
    class Program : public Node {
    public:
        Decls* _Decls;

        Program(Decls* _Decls) :_Decls(_Decls) {}
        ~Program(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
        void genHTML(std::string FileName);
    };

    //Pure virtual class for statement
    class Stmt : public Node {
    public:
        Stmt(void) {}
        ~Stmt(void) {}
        virtual MyValue codegen(CodeGenerator& Gen) = 0;
        virtual std::string nodeJson() = 0;
    };

    //Pure virtual class for Declarations
    class Decl : public Stmt {
    public:
        Decl(void) {}
        ~Decl(void) {}
        virtual MyValue codegen(CodeGenerator& Gen) = 0;
        virtual std::string nodeJson() = 0;
    };

    //Function declaration
    class FuncDecl : public Decl {
    public:
        VarType* _RetType;
        std::string _FuncName;

        ArgList* _ArgList;

        std::string _TypeName; //TypeName is empty for function without base type

        //Funcbody is NULL for declaration
        FuncBody* _FuncBody;

        FuncDecl(VarType* __RetType, const std::string& __FuncName, ArgList* __ArgList, const std::string& __TypeName = "", FuncBody* __FuncBody = NULL) :
            _RetType(__RetType), _FuncName(__FuncName), _ArgList(__ArgList), _TypeName(__TypeName), _FuncBody(__FuncBody) {}
        ~FuncDecl(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Function body
    class FuncBody : public Node {
    public:
        Stmts* _Content;

        FuncBody(Stmts* __Content) :_Content(__Content) {}
        ~FuncBody(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Variable declaration
    class VarDecl : public Decl {
    public:
        VarType* _VarType;
        VarList* _VarList;

        VarDecl(VarType* __VarType, VarList* __VarList) :
            _VarType(__VarType), _VarList(__VarList) {}
        ~VarDecl(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen) { return MyValue(); }
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen) { return MyValue(); }
        //Determine class type
        virtual bool isPointerType(void) = 0;
        virtual bool isArrayType(void) = 0;
        virtual bool isStructType(void) = 0;
        virtual std::string nodeJson() = 0;
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
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return false; }
        std::string nodeJson();
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
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return false; }
        std::string nodeJson();
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
        bool isPointerType(void) { return true; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return false; }

        bool isInnerConst(void) { return _BaseType->_isConst; }
        std::string nodeJson();
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
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return true; }
        bool isStructType(void) { return false; }
        std::string nodeJson();
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
        llvm::Type* GenerateStructTypeHead(CodeGenerator& Gen, const std::string& __Name = "<unnamed>");
        llvm::Type* GenerateStructTypeBody(CodeGenerator& Gen);
        //Get the element index according to its name
        size_t GetElementIndex(const std::string& __MemName);
        //Determine class type
        bool isPointerType(void) { return false; }
        bool isArrayType(void) { return false; }
        bool isStructType(void) { return true; }
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen) { return MyValue(); }
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen) { return MyValue(); }
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen) { return MyValue(); }
        std::string nodeJson();
    };

    //Statement block
    class Block : public Stmt {
    public:
        //Its content
        Stmts* _Content;

        Block(Stmts* __Content) :_Content(__Content) {}
        ~Block(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Switch statement
    class SwitchStmt : public Stmt {
    public:
        //Switch-matcher and case statement list
        Expr* _Matcher;
        CaseList* _CaseList;

        SwitchStmt(Expr* __Matcher, CaseList* __CaseList) : _Matcher(__Matcher), _CaseList(__CaseList) {}
        ~SwitchStmt(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Continue statement
    class ContinueStmt : public Stmt {
    public:
        ContinueStmt(void) {}
        ~ContinueStmt(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Break statement
    class BreakStmt : public Stmt {
    public:
        BreakStmt(void) {}
        ~BreakStmt(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Return statement
    class ReturnStmt : public Stmt {
    public:
        //The expression to be returned (if any)
        Expr* _RetVal;
        ReturnStmt(Expr* __RetVal = NULL) : _RetVal(__RetVal) {}
        ~ReturnStmt(void) {}
        MyValue codegen(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Pure virtual class for expression
    class Expr : public Stmt {
    public:
        Expr(void) {}
        ~Expr(void) {}
        //This function is used to get the "value" of the expression.
        virtual MyValue codegen(CodeGenerator& Gen) = 0;
        //This function is used to get the "pointer" of the instance(must be Expr of left).
        //It is used to implement the "left value" in C language,
        //e.g., the LeftOp of the assignment.
        //right value cannot get ptr
        virtual MyValue codegenPtr(CodeGenerator& Gen) = 0;
        virtual std::string nodeJson() = 0;
    };

    //Subscript, e.g. a[10]
    class Subscript : public Expr {
    public:
        Expr* _Array;
        Expr* _Index;
        Subscript(Expr* __Array, Expr* __Index) : _Array(__Array), _Index(__Index) {}
        ~Subscript(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //Function call
    class FunctionCall : public Expr {
    public:
        std::string _FuncName;
        ExprList* _ArgList;
        Expr* _StructRef;
        Expr* _StructPtr;
        FunctionCall(const std::string& __FuncName, ExprList* __ArgList, Expr* __StructRef = NULL, Expr* __StructPtr = NULL) :
            _FuncName(__FuncName), _ArgList(__ArgList), _StructRef(__StructRef), _StructPtr(__StructPtr) {}
        ~FunctionCall(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class StructReference : public Expr {
    public:
        Expr* _Struct;
        std::string _MemName;
        StructReference(Expr* __Struct, const std::string& __MemName) : _Struct(__Struct), _MemName(__MemName) {}
        ~StructReference(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class StructDereference : public Expr {
    public:
        Expr* _StructPtr;
        std::string _MemName;
        StructDereference(Expr* __StructPtr, const std::string& __MemName) : _StructPtr(__StructPtr), _MemName(__MemName) {}
        ~StructDereference(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class UnaryPlus : public Expr {
    public:
        Expr* _Operand;
        UnaryPlus(Expr* __Operand) : _Operand(__Operand) {}
        ~UnaryPlus(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class UnaryMinus : public Expr {
    public:
        Expr* _Operand;
        UnaryMinus(Expr* __Operand) : _Operand(__Operand) {}
        ~UnaryMinus(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class TypeCast : public Expr {
    public:
        VarType* _VarType;
        Expr* _Operand;
        TypeCast(VarType* __VarType, Expr* __Operand) : _VarType(__VarType), _Operand(__Operand) {}
        ~TypeCast(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class PrefixInc : public Expr {
    public:
        Expr* _Operand;
        PrefixInc(Expr* __Operand) : _Operand(__Operand) {}
        ~PrefixInc(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class PostfixInc : public Expr {
    public:
        Expr* _Operand;
        PostfixInc(Expr* __Operand) : _Operand(__Operand) {}
        ~PostfixInc(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class PrefixDec : public Expr {
    public:
        Expr* _Operand;
        PrefixDec(Expr* __Operand) : _Operand(__Operand) {}
        ~PrefixDec(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class PostfixDec : public Expr {
    public:
        Expr* _Operand;
        PostfixDec(Expr* __Operand) : _Operand(__Operand) {}
        ~PostfixDec(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Indirection : public Expr {
    public:
        Expr* _Operand;
        Indirection(Expr* __Operand) : _Operand(__Operand) {}
        ~Indirection(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class AddressOf : public Expr {
    public:
        Expr* _Operand;
        AddressOf(Expr* __Operand) : _Operand(__Operand) {}
        ~AddressOf(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicNot : public Expr {
    public:
        Expr* _Operand;
        LogicNot(Expr* __Operand) : _Operand(__Operand) {}
        ~LogicNot(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseNot : public Expr {
    public:
        Expr* _Operand;
        BitwiseNot(Expr* __Operand) : _Operand(__Operand) {}
        ~BitwiseNot(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Division : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        Division(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~Division(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Multiplication : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        Multiplication(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~Multiplication(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Modulo : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        Modulo(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~Modulo(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Addition : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        Addition(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~Addition(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Subtraction : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        Subtraction(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~Subtraction(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LeftShift : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LeftShift(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LeftShift(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class RightShift : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        RightShift(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~RightShift(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicGT : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicGT(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicGT(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicGE : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicGE(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicGE(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicLT : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicLT(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicLT(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicLE : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicLE(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicLE(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicEQ : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicEQ(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicEQ(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicNEQ : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicNEQ(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicNEQ(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseAND : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        BitwiseAND(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~BitwiseAND(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseXOR : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        BitwiseXOR(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~BitwiseXOR(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseOR : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        BitwiseOR(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~BitwiseOR(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicAND : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicAND(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicAND(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class LogicOR : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        LogicOR(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~LogicOR(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class TernaryCondition : public Expr {
    public:
        Expr* _Condition;
        Expr* _Then;
        Expr* _Else;
        TernaryCondition(Expr* __Condition, Expr* __Then, Expr* __Else) : _Condition(__Condition), _Then(__Then), _Else(__Else) {}
        ~TernaryCondition(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class DirectAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        DirectAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~DirectAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class DivAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        DivAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~DivAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class MulAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        MulAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~MulAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class ModAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        ModAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~ModAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class AddAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        AddAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~AddAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class SubAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        SubAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~SubAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class SHLAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        SHLAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~SHLAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };
    class SHRAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        SHRAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~SHRAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseANDAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        BitwiseANDAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~BitwiseANDAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseXORAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        BitwiseXORAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~BitwiseXORAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class BitwiseORAssign : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        BitwiseORAssign(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~BitwiseORAssign(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class CommaExpr : public Expr {
    public:
        Expr* _LeftOp;
        Expr* _RightOp;
        CommaExpr(Expr* __LeftOp, Expr* __RightOp) : _LeftOp(__LeftOp), _RightOp(__RightOp) {}
        ~CommaExpr(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class Variable : public Expr {
    public:
        std::string _Name;
        Variable(const std::string& __Name) : _Name(__Name) {}
        ~Variable(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
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
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class GlobalString : public Constant {
    public:
        std::string _Content;
        GlobalString(const std::string& __Content) : Constant(0), _Content(__Content) {}
        ~GlobalString(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    class This : public Expr {
    public:
        This(void) {}
        ~This(void) {}
        MyValue codegen(CodeGenerator& Gen);
        MyValue codegenPtr(CodeGenerator& Gen);
        std::string nodeJson();
    };

    //wrapped llvm::Function
    class MyFunction {
    public:
        explicit MyFunction(llvm::Function* _f, ArgList* _a, VarType* _r, std::string _t) :
            LLVMFunc(_f),
            Args(_a),
            RetType(_r),
            TypeName(_t) {}

        llvm::Function* LLVMFunc;
        ArgList* Args;
        VarType* RetType;
        std::string TypeName;
    };

    //wrapped llvm::Type
    class MyType {
    public:
        explicit MyType(llvm::Type* _t, FieldDecls* _f) :
            LLVMType(_t),
            Fileds(_f) {}

        llvm::Type* LLVMType;
        FieldDecls* Fileds;
    };
}