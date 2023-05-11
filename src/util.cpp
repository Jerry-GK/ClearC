#include "../include/util.h"

void printMsg(const std::string& msg) {
    std::cout << msg << std::endl;
}

void printError(const std::string& msg) {
    std::string bar = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    printMsg("\n" + bar);
    printMsg(msg);
    printMsg(bar + "\n");
}