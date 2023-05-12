#include "../include/util.h"

void PrintMsg(const std::string& msg) {
    std::cout << msg << std::endl;
}

void PrintError(const std::string& msg) {
    std::string bar = std::string(msg.length() + 1, 'X');
    PrintMsg("\n" + bar);
    PrintMsg(msg);
    PrintMsg(bar + "\n");
}