#include <string>
using namespace std;

class CommandParser {
public:
    CommandParser() {
        InputFile = "";
        OutputObjectFile = "";
        OptimizeLevel = "O0";
    };
    ~CommandParser() {}
    void ParseCommand(int argc, const char* argv[]);
    string GetInputFile();
    string GetOutputObjectFile();
    string GetOptimizeLevel();

private:
    string InputFile;
    string OutputObjectFile;
    string OptimizeLevel;
};