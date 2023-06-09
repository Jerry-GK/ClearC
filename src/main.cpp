#include <iostream>
#include <string>
#include "ast.hpp"
#include "generator.hpp"
#include "include/util.h"
#include "include/command.h"

using namespace std;

extern int yyparse();
extern ast::Program* Root;
string VersionStr = "1.2";

//program entry
int main(int argc, const char* argv[]) {
    PrintMsg("[ClearC --Version" + VersionStr + "]");

    //Parse command
    CommandParser* cp = new CommandParser();
    try {
        cp->ParseCommand(argc, argv);
    }
    catch (const std::exception& e) {
        PrintError("[Command Error]: " + string(e.what()));
        PrintMsg("[Usage]: " + string(argv[0]) + " (<Optimize level like -O3> optional)  <input file> -o <output file>");
        return 1;
    }
    string InputFile = cp->GetInputFile();
    string OutputObjectFile = cp->GetOutputObjectFile();
    string OptimizationLevel = cp->GetOptimizationLevel();
    delete cp;

    //Read input file
    string VisualizationFile = InputFile + "_AST.html";	bool GenVisual = true;
    string LLVMIRCodeFile = InputFile + "_IR.html";	bool GenIR = true;
    if (!freopen(InputFile.c_str(), "r", stdin)) { //read file into stdin
        PrintError("[File Error]: Cannot open " + InputFile);
        return 1;
    }
    PrintMsg("[Success]: Compiling \"" + InputFile + "\" ...");

    //Parse
    yyparse(); //parse the input file read into stdin, and generate ast
    PrintMsg("[Success]: Parse successfully");
    if (GenVisual) {
        Root->genHTML(VisualizationFile);
    }


    //Generate IR code
    Generator Gen;
    try {
        Gen.GenerateIRCode(*Root);
    }
    catch (const std::exception& e) {
        PrintError("[Semantic Error]: " + string(e.what()));
        return 1;
    }
    PrintMsg("[Success]: IR code generated successfully");

    //Optimize IR code
    try {
        Gen.OptimizeIRCode(OptimizationLevel);
    }
    catch (const std::exception& e) {
        PrintError("[Optimization Error]: " + string(e.what()));
        return 1;
    }
    PrintMsg("[Success]: IR code optimized successfully (level = " + OptimizationLevel + ")");

    //Output and verify IR file
    bool IRValid = true;
    if (GenIR) {
        IRValid = Gen.OutputIR(LLVMIRCodeFile);
        PrintMsg("[Success]: IR code output successfully");
    }
    if (!IRValid) {
        PrintError("[IR Error]: Look at the IR code for details");
        return 1;
    }

    //Generate object code
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