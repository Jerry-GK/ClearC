#include <string>
using namespace std;

class CommandParser {
public:
    CommandParser() {
        InputFile = "";
        OutputObjectFile = "";
        OptimizationLevel = "O0";
    };
    ~CommandParser() {}
    void ParseCommand(int argc, const char* argv[]);
    string GetInputFile();
    string GetOutputObjectFile();
    string GetOptimizationLevel();

private:
    string InputFile;
    string OutputObjectFile;
    string OptimizationLevel;
};