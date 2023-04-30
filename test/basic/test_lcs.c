# include <stdio.h>
# include <string.h>
# include <stdlib.h>

// int printf(char ptr, ...);
// int scanf(char ptr, ...);

//get the longest common subsequence of str1 and str2, using dp
char* getLCS(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int dp[len1 + 1][len2 + 1];
    int i, j;

    //init dp
    for (i = 0; i <= len1; i++) {
        dp[i][0] = 0;
    }
    for (j = 0; j <= len2; j++) {
        dp[0][j] = 0;
    }

    //dp
    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else {
                dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }

    //get the lcs
    char* lcs = (char*)malloc(sizeof(char) * (dp[len1][len2] + 1));
    i = len1;
    j = len2;
    int k = dp[len1][len2] - 1;
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            lcs[k--] = str1[i - 1];
            i--;
            j--;
        }
        else {
            if (dp[i - 1][j] > dp[i][j - 1]) {
                i--;
            }
            else {
                j--;
            }
        }
    }
    lcs[dp[len1][len2]] = '\0';
    return lcs;
}


int main() {
    //read a string
    char str1[100];
    char str2[100];
    char* str_res;
    scanf("%s", str1);
    scanf("%s", str2);

    str_res = getLCS(str1, str2);

    printf("%lu", strlen(str_res));
}

