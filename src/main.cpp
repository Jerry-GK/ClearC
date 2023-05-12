#include <iostream>
#include <string>
#include "ast.hpp"
#include "codegen.hpp"
#include "../include/util.h"

using namespace std;

extern int yyparse();
extern ast::Program* Root;

int main(int argc, const char* argv[]) {
    PrintMsg("[ClearC --Version1.0]");

    if (argc != 3) {
        PrintError("[Command Error]: Usage: " + string(argv[0]) + " <sourcefilename> <objectfilename>");
        return 1;
    }

    string InputFile = argv[1];
    string VisualizationFile = InputFile + "_AST.html";	bool GenVisual = true;
    string LLVMIRCodeFile = InputFile + "_IR.html";	bool GenIR = true;
    string OutputObjectFile = argv[2];
    string OptimizeLevel = "";

    if (!freopen(InputFile.c_str(), "r", stdin)) { //read file into stdin
        PrintError("[File Error]: Cannot open " + InputFile);
        return 1;
    }

    PrintMsg("[Success]: Compiling \"" + InputFile + "\" ...");

    yyparse(); //parse the input file read into stdin, and generate ast

    PrintMsg("[Success]: Parse successfully");

    if (GenVisual) {
        Root->genHTML(VisualizationFile);
    }

    //Root is now the root of the ast

    CodeGenerator Gen;

    try {
        Gen.GenerateIRCode(*Root, OptimizeLevel);
    }
    catch (const std::exception& e) {
        PrintError("[Semantic Error]: " + string(e.what()));
        return 1;
    }
    PrintMsg("[Success]: IR code generated successfully");


    //generate and verify IR
    bool IRValid = true;
    if (GenIR) {
        IRValid = Gen.OutputIR(LLVMIRCodeFile);
        PrintMsg("[Success]: IR code output successfully");
    }
    if (!IRValid) {
        PrintError("[IR Error]: Look at the IR code for details");
        return 1;
    }

    try {
        Gen.GenObjectCode(OutputObjectFile);
    }
    catch (const std::exception& e) {
        PrintError("[Codegen Error]: " + string(e.what()));
        return 1;
    }
    PrintMsg("[Success]: Object code generated successfully, output file: \"" + OutputObjectFile + "\"");

    return 0;
}