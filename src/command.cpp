#include "include/command.h"

void CommandParser::ParseCommand(int argc, const char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-o") {
            if (i + 1 >= argc) {
                throw std::logic_error("No output file specified");
                return;
            }
            this->OutputObjectFile = string(argv[++i]);
        }
        else if (string(argv[i]).find("-O") == 0) {
            this->OptimizationLevel = string(argv[i]).substr(1, -1);
        }
        else {
            if (this->InputFile != "") {
                throw std::logic_error("Multiple input files is not supported in ClearC");
                return;
            }
            this->InputFile = string(argv[i]);
        }
    }

    if (this->InputFile == "") {
        throw std::logic_error("No input file specified");
        return;
    }
    if (this->OutputObjectFile == "") {
        this->OutputObjectFile = this->InputFile + ".o";
    }
}

string CommandParser::GetInputFile() {
    return this->InputFile;
}
string CommandParser::GetOutputObjectFile() {
    return this->OutputObjectFile;
}
string CommandParser::GetOptimizationLevel() {
    return this->OptimizationLevel;
}