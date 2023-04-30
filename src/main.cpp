#include <iostream>
#include <string>
#include "ast.hpp"

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
    if (argc != 3) {
        printMsg("<Usage>: " + string(argv[0]) + " <sourcefilename> <objectfilename>");
        return 1;
    }

    string InputFile = argv[1];
    string OutputObjectFile = argv[2];
    string LLVMIRCodeFile;	bool GenIR = false;
    string VisualizationFile = InputFile + ".html";	bool GenVisual = true;

    freopen(InputFile.c_str(), "r", stdin); //read file into stdin

    yyparse(); //parse the input file read into stdin, and generate ast

    if (GenVisual) {
        GenHTML(VisualizationFile, *Root);
    }

    // CodeGenerator Gen;

    // Gen.GenerateCode(*Root, OptimizeLevel);

    // Gen.GenObjectCode(OutputObjectFile);
}