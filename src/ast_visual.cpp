//This implementation is referenced!

#include "ast.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace ast;

extern const char* Html;

string getString(string name) {
    std::string s = "";
    for (char c : name) {
        if (c == '\"' || c == '\'' || c == '\\')
            s = s + "\\" + c;
        else if (c == '\n')
            s += "\\n";
        else if (c == '\t')
            s += "\\t";
        else if (c == '\r')
            s += "\\r";
        else if (isprint((unsigned char)c))
            s += c;
        else {
            std::stringstream stream;
            stream << std::hex << (unsigned int)(unsigned char)(c);
            std::string code = stream.str();
            s += std::string("\\x") + (code.size() < 2 ? "0" : "") + code;
        }
    }
    return s;
}

string getJson(string name) {
    return "{ \"name\" : \"" + getString(name) + "\" }";
}

string getJson(string name, vector<string> children) {
    string result = "{ \"name\" : \"" + name + "\", \"children\" : [ ";
    int i = 0;
    for (auto& child : children) {
        if (i != children.size() - 1)
            result += child + ", ";
        else
            result += child + " ";
        i++;
    }
    return result + " ] }";
}

string getJson(string name, string value) {
    return getJson(name, vector<string>{value});
}

string Program::nodeJson() {
    vector<string> declJson;	//children node json
    for (auto x : *_Decls)
        declJson.push_back(x->nodeJson());
    return getJson("Program", declJson);
}

void Program::genHTML(std::string FileName) {
    std::string OutputString = Html;
    std::string Json = nodeJson();
    std::string Target = "${ASTJson}";
    auto Pos = OutputString.find(Target);
    OutputString.replace(Pos, Target.length(), Json.c_str());
    std::ofstream HTMLFile(FileName);
    HTMLFile << OutputString;
}

string FuncDecl::nodeJson() {
    vector<string> children;
    children.push_back(getJson("Return Type", _RetType->nodeJson()));
    children.push_back(getJson("Function Name", getJson(_FuncName)));
    vector<string> argListJson;
    for (auto& x : *_ArgList)
        argListJson.push_back(x->nodeJson());
    if (_ArgList->_DotsArg == true)
        argListJson.push_back(getJson("...", ""));
    children.push_back(getJson("ArgList", argListJson));
    if (_FuncBody != NULL)
        children.push_back(_FuncBody->nodeJson());
    return getJson("FuncDeclaration", children);
}

string ArgList::nodeJson() {
    if (_DotsArg == true)
        return getJson("..", getJson("True"));
    else
        return getJson("..", getJson("False"));
    return "";
}

string FuncBody::nodeJson() {
    vector<string> funcBodyJson;	//children node json
    for (auto& x : *_Content)
        funcBodyJson.push_back(x->nodeJson());
    // children.push_back(getJson("Decls", declJson));
    return getJson("FuncBody", funcBodyJson);
}

string VarDecl::nodeJson() {
    vector<string> children;
    children.push_back(_VarType->nodeJson());
    vector<string> varListJson;
    for (auto& x : *_VarList)
        varListJson.push_back(x->nodeJson());
    children.push_back(getJson("VarList", varListJson));
    return getJson("VarDeclaration", children);
}

string VarInit::nodeJson() {
    vector<string> children;
    children.push_back(getJson(_Name));
    if (_InitialExpr != NULL)
    {
        //children.push_back(getJson("="));
        children.push_back(_InitialExpr->nodeJson());
    }
    return getJson("Varinit", children);
}

string TypeDecl::nodeJson() {
    vector<string> children;
    children.push_back(_VarType->nodeJson());
    children.push_back(getJson(_Alias));
    return getJson("TypeDeclaration", children);
    // return "";
}

string DefinedType::nodeJson() {
    vector<string> children;
    children.push_back(getJson(_Name));
    return getJson("Definedtype", children);
    // return "";
}

string PointerType::nodeJson() {
    vector<string> children;
    children.push_back(_BaseType->nodeJson());
    return getJson("PointerType", children);
    // return "";
}

string ArrayType::nodeJson() {
    vector<string> children;
    children.push_back(_BaseType->nodeJson());
    children.push_back(getJson(to_string(_Length)));
    return getJson("ArrayType", children);
    // return "";
}

string StructType::nodeJson() {
    vector<string> children;
    for (auto& x : *_StructBody)
        children.push_back(x->nodeJson());
    return getJson("StructType", children);
}

string AutoType::nodeJson() {
    return getJson("AutoType", getJson("auto"));
}


string FieldDecl::nodeJson() {
    vector<string> children;
    children.push_back(_VarType->nodeJson());
    vector<string> memListJson;
    for (auto x : *_MemList)
        memListJson.push_back(getJson(x));
    children.push_back(getJson("MemList", memListJson));
    return getJson("FieldDecl", children);
    // return "";
}

string Arg::nodeJson()
{
    vector<string> children;
    children.push_back(_VarType->nodeJson());
    if (_Name != "") children.push_back(getJson(_Name));
    return getJson("Arg", children);
    // return "";
}

string Block::nodeJson() {
    vector<string> blockJson;	//children node json
    for (auto& x : *_Content)
        blockJson.push_back(x->nodeJson());
    // children.push_back(getJson("Decls", declJson));
    return getJson("Block", blockJson);
}

string IfStmt::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Condition->nodeJson());
    children.push_back(_Then->nodeJson());
    if (_Else != NULL)
        children.push_back(_Else->nodeJson());
    return getJson("IfStmt", children);
}

string ForStmt::nodeJson() {
    vector<string> children;	//children node json
    if (_Initial != NULL)
        children.push_back(_Initial->nodeJson());
    if (_Condition != NULL)
        children.push_back(_Condition->nodeJson());
    if (_Tail != NULL)
        children.push_back(_Tail->nodeJson());
    if (_LoopBody != NULL)
        children.push_back(_LoopBody->nodeJson());
    return getJson("ForStmt", children);
}

string SwitchStmt::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Matcher->nodeJson());
    vector<string> caseListJson;
    for (auto x : *_CaseList)
        caseListJson.push_back(x->nodeJson());
    children.push_back(getJson("CaseList", caseListJson));
    return getJson("SwitchStmt", children);
}

string CaseStmt::nodeJson() {
    vector<string> children;	//children node json
    if (_Condition != NULL)
        children.push_back(_Condition->nodeJson());
    else
        children.push_back(getJson("Default"));
    vector<string> stmtListJson;
    for (auto x : *_Content)
        stmtListJson.push_back(x->nodeJson());
    children.push_back(getJson("StatmentList", stmtListJson));
    return getJson("CaseStmt", children);
}

string BreakStmt::nodeJson() {
    return getJson("BreakStmt");
}

string ContinueStmt::nodeJson() {
    return getJson("ContinueStmt");
}

string ReturnStmt::nodeJson() {
    vector<string> children;	//children node json
    if (_RetVal != NULL)
        children.push_back(_RetVal->nodeJson());
    return getJson("ReturnStmt", children);
}

string Subscript::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Array->nodeJson());
    children.push_back(_Index->nodeJson());
    return getJson("Subscript", children);
}

string SizeOf::nodeJson() {
    vector<string> children;	//children node json
    if (_Arg1 != NULL)
        children.push_back(_Arg1->nodeJson());
    if (_Arg2 != NULL)
        children.push_back(_Arg2->nodeJson());
    return getJson("Sizeof", children);
}

string FunctionCall::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(getJson("Function Name", getJson(_FuncName)));
    vector<string> argListJson;
    for (auto x : *_ArgList)
        argListJson.push_back(x->nodeJson());
    children.push_back(getJson("ArgList", argListJson));
    return getJson("FunctionCall", children);
}

string StructReference::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Struct->nodeJson());
    children.push_back(getJson(_MemName));
    return getJson("StructReference", children);
}

string StructDereference::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_StructPtr->nodeJson());
    children.push_back(getJson(_MemName));
    return getJson("StructDereference", children);
}

string UnaryPlus::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("+"));
    children.push_back(_Operand->nodeJson());
    return getJson("UnaryPlus", children);
}

string UnaryMinus::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("-"));
    children.push_back(_Operand->nodeJson());
    return getJson("UnaryMinus", children);
}

string TypeCast::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_VarType->nodeJson());
    children.push_back(_Operand->nodeJson());
    return getJson("TypeCast", children);
}

string PrefixInc::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("++"));
    children.push_back(_Operand->nodeJson());
    return getJson("PrefixInc", children);
}

string PostfixInc::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Operand->nodeJson());
    //children.push_back(getJson("++"));
    return getJson("PostfixInc", children);
}

string PrefixDec::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("--"));
    children.push_back(_Operand->nodeJson());
    return getJson("PrefixDec", children);
}

string PostfixDec::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Operand->nodeJson());
    //children.push_back(getJson("--"));
    return getJson("PostfixDec", children);
}

string Indirection::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("*"));
    children.push_back(_Operand->nodeJson());
    return getJson("Indirection", children);
}

string AddressOf::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("&"));
    children.push_back(_Operand->nodeJson());
    return getJson("Addressof", children);
}

string LogicNot::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("!"));
    children.push_back(_Operand->nodeJson());
    return getJson("LogicNot", children);
}

string BitwiseNot::nodeJson() {
    vector<string> children;	//children node json
    //children.push_back(getJson("~"));
    children.push_back(_Operand->nodeJson());
    return getJson("BitwiseNot", children);
}

string Division::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("/"));
    children.push_back(_RightOp->nodeJson());
    return getJson("Division", children);
}

string Multiplication::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("*"));
    children.push_back(_RightOp->nodeJson());
    return getJson("Multiplication", children);
}

string Modulo::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("%"));
    children.push_back(_RightOp->nodeJson());
    return getJson("Modulo", children);
}

string Addition::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("+"));
    children.push_back(_RightOp->nodeJson());
    return getJson("Addition", children);
}

string Subtraction::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("-"));
    children.push_back(_RightOp->nodeJson());
    return getJson("Subtraction", children);
}

string LeftShift::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("<<"));
    children.push_back(_RightOp->nodeJson());
    return getJson("LeftShift", children);
}

string RightShift::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson(">>"));
    children.push_back(_RightOp->nodeJson());
    return getJson("RightShift", children);
}

string LogicGT::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson(">"));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicGT", children);
}

string LogicGE::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson(">="));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicGE", children);
}

string LogicLT::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("<"));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicLT", children);
}

string LogicLE::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("<="));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicLE", children);
}

string LogicEQ::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("=="));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicEQ", children);
}

string LogicNEQ::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("!="));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicNEQ", children);
}

string BitwiseAND::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("&"));
    children.push_back(_RightOp->nodeJson());
    return getJson("BitwiseAND", children);
}

string BitwiseXOR::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("^"));
    children.push_back(_RightOp->nodeJson());
    return getJson("BitwiseXOR", children);
}

string BitwiseOR::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("|"));
    children.push_back(_RightOp->nodeJson());
    return getJson("BitwiseOR", children);
}

string LogicAND::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("&&"));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicAND", children);
}

string LogicOR::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("||"));
    children.push_back(_RightOp->nodeJson());
    return getJson("LogicOR", children);
}

string TernaryCondition::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_Condition->nodeJson());
    children.push_back(_Then->nodeJson());
    children.push_back(_Else->nodeJson());
    return getJson("TernaryCondition", children);
}

string DirectAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("="));
    children.push_back(_RightOp->nodeJson());
    return getJson("DirectAssign", children);
}

string DivAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("/="));
    children.push_back(_RightOp->nodeJson());
    return getJson("DivAssign", children);
}

string MulAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("*="));
    children.push_back(_RightOp->nodeJson());
    return getJson("MulAssign", children);
}

string ModAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("%="));
    children.push_back(_RightOp->nodeJson());
    return getJson("ModAssign", children);
}

string AddAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("+="));
    children.push_back(_RightOp->nodeJson());
    return getJson("AddAssign", children);
}

string SubAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("-="));
    children.push_back(_RightOp->nodeJson());
    return getJson("SubAssign", children);
}

string SHLAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("<<="));
    children.push_back(_RightOp->nodeJson());
    return getJson("SHLAssign", children);
}

string SHRAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson(">>="));
    children.push_back(_RightOp->nodeJson());
    return getJson("SHRAssign", children);
}

string BitwiseANDAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("&="));
    children.push_back(_RightOp->nodeJson());
    return getJson("BitwiseANDAssign", children);
}

string BitwiseXORAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("^="));
    children.push_back(_RightOp->nodeJson());
    return getJson("BitwiseXORAssign", children);
}

string BitwiseORAssign::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson("|="));
    children.push_back(_RightOp->nodeJson());
    return getJson("BitwiseORAssign", children);
}

string CommaExpr::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(_LeftOp->nodeJson());
    //children.push_back(getJson(","));
    children.push_back(_RightOp->nodeJson());
    return getJson("CommaExpr", children);
}

string Variable::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(getJson(_Name));
    return getJson("Variable", children);
}

string GlobalString::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(getJson("\"" + getString(_Content) + "\""));
    return getJson("Global String", children);
}

string Constant::nodeJson() {
    vector<string> children;	//children node json
    switch (_Type)
    {
    case BuiltInType::TypeID::_Bool:
        return (_Bool == true) ? getJson("Constant", getJson("True")) : getJson("Constant", getJson("False"));
        break;
    case BuiltInType::TypeID::_Short:
    case BuiltInType::TypeID::_Int:
    case BuiltInType::TypeID::_Long:
        return getJson("Constant", getJson(to_string(_Integer)));
        break;
    case BuiltInType::TypeID::_Char:
        return getJson("Constant", getJson("\'" + getString(string(1, _Character)) + "\'"));
        break;
    case BuiltInType::TypeID::_Float:
    case BuiltInType::TypeID::_Double:
        return getJson("Constant", getJson(to_string(_Real)));
        break;
    default:
        break;
    }
    return "";
}

string BuiltInType::nodeJson() {
    switch (_Type)
    {
    case _Bool:
        return getJson("BuildInType", getJson("Bool"));
        break;
    case _Short:
        return getJson("BuildInType", getJson("Short"));
        break;
    case _Int:
        return getJson("BuildInType", getJson("Int"));
        break;
    case _Long:
        return getJson("BuildInType", getJson("Long"));
        break;
    case _Char:
        return getJson("BuildInType", getJson("Char"));
        break;
    case _Float:
        return getJson("BuildInType", getJson("Float"));
        break;
    case _Double:
        return getJson("BuildInType", getJson("Double"));
        break;
    case _Void:
        return getJson("BuildInType", getJson("Void"));
        break;
    default:
        break;
    }
}


string This::nodeJson() {
    vector<string> children;	//children node json
    children.push_back(getJson("this"));
    return getJson("This", children);
}

