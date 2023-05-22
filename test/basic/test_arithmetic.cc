// ClearC
// Written in OOP style

func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;
func malloc(long)->ptr<void>;
func free(ptr<void>) -> void;
func strlen(ptr<const char>) -> int;
func strcpy(ptr<char> dest, ptr<const char> src)->ptr<char>;

// Declaration
typedef Calculator struct {
    ptr<char> expr;
};

func Calculator : Init() -> void;
func Calculator : ReadExpr(const ptr<const char> expr) -> void;
func Calculator : Calc() -> int;
func Calculator : getNum(ptr<ptr<const char> > p) -> int;
func Calculator : getTerm(ptr<ptr<const char> > p) -> int;

// main
func main() -> int {
    Calculator c;
    c.Init();

    array<char, 100> expr;
    scanf("%s", expr);
    c.ReadExpr(expr);
    int result = c.Calc();

    printf("%d\n", result);
    return 0;
}

// Calculator implementation 
func Calculator : Init() -> void {
    this->expr = null;
    return;
}

func Calculator : ReadExpr(const ptr<const char> expr) -> void {
    //deep copy
    if this->expr != null {
        free(this->expr);
    }
    this->expr = typecast(malloc(sizeof(char) * (strlen(expr) + 1)), ptr<char>);
    strcpy(this->expr, expr);
    return;
}

func Calculator : Calc() -> int {
    ptr<const char> p = this->expr;
    int left = this->getTerm(addr(p));
    for dptr(p) != '\0' && dptr(p) != ')' {
        char op = dptr(p);
        p = p + sizeof(char);
        int right = this->getTerm(addr(p));
        if op == '+' {
            left += right;
        }
        else {
            left -= right;
        }
    }
    return left;
}

func Calculator : getNum(ptr<ptr<const char> > p) -> int {
    int num = 0;
    for dptr(dptr(p)) >= '0' && dptr(dptr(p)) <= '9' {
        num = num * 10 + (dptr(dptr(p)) - '0');
        dptr(p) = dptr(p) + sizeof(char);
    }
    return num;
}

func Calculator : getTerm(ptr<ptr<const char> > p) -> int {
    int left = this->getNum(p);
    for dptr(dptr(p)) == '*' || dptr(dptr(p)) == '/' {
        char op = dptr(dptr(p));
        dptr(p) = dptr(p) + sizeof(char);
        int right = this->getNum(p);
        if op == '*' {
            left *= right;
        }
        else {
            left /= right;
        }
    }
    return left;
}


// C Version
// #include <stdio.h>
// #include <stdlib.h>

// int getNum(char** ptr) {
//     int num = 0;
//     while (**ptr >= '0' && **ptr <= '9') {
//         num = num * 10 + (**ptr - '0');
//         (*ptr)++;
//     }
//     return num;
// }

// int getTerm(char** ptr) {
//     int left = getNum(ptr);
//     while (**ptr == '*' || **ptr == '/') {
//         char op = **ptr;
//         (*ptr)++;
//         int right = getNum(ptr);
//         if (op == '*') {
//             left *= right;
//         }
//         else {
//             left /= right;
//         }
//     }
//     return left;
// }

// int calculate(char* expr) {
//     char* ptr = expr;
//     int left = getTerm(&ptr);
//     while (*ptr != '\0' && *ptr != ')') {
//         char op = *ptr;
//         ptr++;
//         int right = getTerm(&ptr);
//         if (op == '+') {
//             left += right;
//         }
//         else {
//             left -= right;
//         }
//     }
//     return left;
// }

// int main() {
//     char expr[100];
//     scanf("%s", expr);
//     printf("%d\n", calculate(expr));
//     return 0;
// }
