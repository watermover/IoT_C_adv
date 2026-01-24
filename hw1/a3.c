#include <stdio.h>

int main(void) {
    unsigned int N;
    int K;

    if (scanf("%u %d", &N, &K) != 2) return 0;

    unsigned int mask = (1U << K) - 1U;  
    unsigned int res  = N & mask;        

    printf("%u\n", res);
    return 0;
}
