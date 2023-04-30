# include <stdio.h>

// int printf(char ptr, ...);
// int scanf(char ptr, ...);

int main() {
    int n;
    int isPrime = 1;

    scanf("%d", &n);

    if (n < 2) {
        isPrime = 0;
    }

    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            isPrime = 0;
            break;
        }
    }

    printf("%d", isPrime);

}