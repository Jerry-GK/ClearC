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

char EscapeOfChar(char c) {
    if ('0' <= c && c <= '9')
        return (char)(c - '0');
    switch (c) {
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    case '\\': return '\\';
    case '\"': return '\"';
    case '\'': return '\'';
    default:
        return c;
    }
}