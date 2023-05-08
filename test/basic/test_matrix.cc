//ClearC
func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;

func main() -> int {
    array<int, 100> A;
    array<int, 100> B;
    array<int, 100> C;
    int M, N, P;

    //read size
    scanf("%d %d %d", addr(M), addr(N), addr(P));

    //read A
    for int i = 0; i < M; i++ {
        for int j = 0; j < N; j++ {
            scanf("%d", addr(A[i * N + j]));
        }
    }

    //read B
    for int i = 0; i < N; i++ {
        for int j = 0; j < P; j++ {
            scanf("%d", addr(B[i * P + j]));
        }
    }

    //output the product
    for int i = 0; i < M; i++ {
        for int j = 0; j < P; j++ {
            int sum = 0;
            for int k = 0; k < N; k++ {
                sum += A[i * N + k] * B[k * P + j];
            }
            C[i * P + j] = sum;
            printf("%d ", sum);
        }
        printf("\n");
    }
    return 0;
}


//C Version
// # include <stdio.h>

// // int printf(char ptr, ...);
// // int scanf(char ptr, ...);

// int main() {
//     int A[100];
//     int B[100];
//     int C[100];
//     int M, N, P;

//     //read size
//     scanf("%d %d %d", &M, &N, &P);

//     //read A
//     for (int i = 0; i < M; i++) {
//         for (int j = 0; j < N; j++) {
//             scanf("%d", &A[i * N + j]);
//         }
//     }

//     //read B
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < P; j++) {
//             scanf("%d", &B[i * P + j]);
//         }
//     }

//     //output the product
//     for (int i = 0; i < M; i++) {
//         for (int j = 0; j < P; j++) {
//             int sum = 0;
//             for (int k = 0; k < N; k++) {
//                 sum += A[i * N + k] * B[k * P + j];
//             }
//             C[i * P + j] = sum;
//             printf("%d ", sum);
//         }
//         printf("\n");
//     }
// }