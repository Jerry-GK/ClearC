//ClearC
func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;

func hanoi(int n, char a, char b, char c) -> void {
    if n == 1 {
        printf("%c->%c\n", a, c);
    }
    else {
        hanoi(n - 1, a, c, b);
        printf("%c->%c\n", a, c);
        hanoi(n - 1, b, a, c);
    }
    return;
}

func main() -> int {
    int n;
    scanf("%d", addr(n));
    hanoi(n, 'A', 'B', 'C');
    return 0;
}

//C Version
// # include <stdio.h>

// // int printf(char ptr, ...);
// // int scanf(char ptr, ...);

// void hanoi(int n, char a, char b, char c) {
//     if (n == 1) {
//         printf("%c->%c\n", a, c);
//     }
//     else {
//         hanoi(n - 1, a, c, b);
//         printf("%c->%c\n", a, c);
//         hanoi(n - 1, b, a, c);
//     }
// }

// int main() {
//     int n;
//     scanf("%d", &n);
//     hanoi(n, 'A', 'B', 'C');
// }