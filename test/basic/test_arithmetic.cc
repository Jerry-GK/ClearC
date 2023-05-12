func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;
func malloc(long)->ptr<void>;
func strlen(ptr<char>) -> int;

func get_num(ptr<ptr<char> > p) -> int {
    //printf("%s\n", dptr(p));
    int num = 0;
    //printf("**p = %c\n", dptr(dptr(p)));
    for dptr(dptr(p)) >= '0' && dptr(dptr(p)) <= '9' {
        num = num * 10 + (dptr(dptr(p)) - '0');
        //printf("**p = %c\n", dptr(dptr(p)));
        dptr(p) = dptr(p) + sizeof(char);
    }
    //printf("num = %d\n", num);
    return num;
}

func get_term(ptr<ptr<char> > p) -> int {
    int left = get_num(p);
    for dptr(dptr(p)) == '*' || dptr(dptr(p)) == '/' {
        char op = dptr(dptr(p));
        dptr(p) = dptr(p) + sizeof(char);
        int right = get_num(p);
        if op == '*' {
            left *= right;
        }
        else {
            left /= right;
        }
    }
    return left;
}

func calculate(ptr<char> expr) -> int {
    ptr<char> p = expr;
    int left = get_term(addr(p));
    for dptr(p) != '\0' && dptr(p) != ')' {
        char op = dptr(p);
        p = p + sizeof(char);
        int right = get_term(addr(p));
        if op == '+' {
            left += right;
        }
        else {
            left -= right;
        }
    }
    return left;
}

func main() -> int {
    array<char, 100> expr;
    //ptr<char> expr;
    //expr = typecast(malloc(sizeof(char) * 100), ptr<char>);
    scanf("%s", expr);
    printf("%d\n", calculate(expr));
    return 0;
}


// C Version
// #include <stdio.h>
// #include <stdlib.h>

// int get_num(char** ptr) {
//     int num = 0;
//     while (**ptr >= '0' && **ptr <= '9') {
//         num = num * 10 + (**ptr - '0');
//         (*ptr)++;
//     }
//     return num;
// }

// int get_term(char** ptr) {
//     int left = get_num(ptr);
//     while (**ptr == '*' || **ptr == '/') {
//         char op = **ptr;
//         (*ptr)++;
//         int right = get_num(ptr);
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
//     int left = get_term(&ptr);
//     while (*ptr != '\0' && *ptr != ')') {
//         char op = *ptr;
//         ptr++;
//         int right = get_term(&ptr);
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
