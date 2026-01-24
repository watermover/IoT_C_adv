#include <stdio.h>

int main(void) {
    unsigned int N;
    int K;

    if (scanf("%u %d", &N, &K) != 2) return 0;

    K %= 32;             
    if (K < 0) K += 32;  

    unsigned int res = (N >> K) | (N << (32 - K));

    printf("%u\n", res);
    return 0;
}
