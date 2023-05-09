#include <iostream>
#include <string>
#include "ast.hpp"
#include "codegen.hpp"

using namespace std;

extern int yyparse();
extern ast::Program* Root;
extern const char* Html;


void printMsg(const std::string& msg) {
    cout << msg << endl;
}


void GenHTML(std::string FileName, ast::Program& Root) {
    std::string OutputString = Html;
    std::string Json = Root.astJson();
    std::string Target = "${ASTJson}";
    auto Pos = OutputString.find(Target);
    OutputString.replace(Pos, Target.length(), Json.c_str());
    std::ofstream HTMLFile(FileName);
    HTMLFile << OutputString;
}

int main(int argc, const char* argv[]) {
    printMsg("[ClearC --Version1.0]");

    if (argc != 3) {
        printMsg("[Command Error]: Usage: " + string(argv[0]) + " <sourcefilename> <objectfilename>");
        return 1;
    }

    string InputFile = argv[1];
    string VisualizationFile = InputFile + "_AST.html";	bool GenVisual = true;
    string LLVMIRCodeFile = InputFile + "_IR.html";	bool GenIR = true;
    string OutputObjectFile = argv[2];
    string OptimizeLevel = "";

    if (!freopen(InputFile.c_str(), "r", stdin)) { //read file into stdin
        printMsg("[File Error]: Cannot open " + InputFile);
        return 1;
    }

    printMsg("[Success]: Compiling \"" + InputFile + "\" ...");

    yyparse(); //parse the input file read into stdin, and generate ast

    printMsg("[Success]: Parse successfully");

    if (GenVisual) {
        GenHTML(VisualizationFile, *Root);
    }

    //Root is now the root of the ast

    CodeGenerator Gen;

    try {
        Gen.GenerateIRCode(*Root, OptimizeLevel);
    }
    catch (const std::exception& e) {
        string bar = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        printMsg("\n" + bar);
        printMsg("[Semantic Error]: " + string(e.what()));
        printMsg(bar + "\n");
        return 1;
    }
    printMsg("[Success]: IR code generated successfully");


    //generate and verify IR
    bool IRValid = true;
    if (GenIR) {
        IRValid = Gen.OutputIR(LLVMIRCodeFile);
        printMsg("[Success]: IR code output successfully");
    }
    if (!IRValid) {
        printMsg("[IR Error]: Look at the IR code for details");
        return 1;
    }

    try {
        Gen.GenObjectCode(OutputObjectFile);
    }
    catch (const std::exception& e) {
        printMsg("[Codegen Error]: " + string(e.what()));
        return 1;
    }
    printMsg("[Success]: Object code generated successfully");

    return 0;
}