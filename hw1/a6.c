#include <stdio.h>

int main(void) {
    unsigned int N;
    if (scanf("%u", &N) != 1) return 0;

    unsigned int mask = 0xFF000000U;  
    N ^= mask;                       

    printf("%u\n", N);
    return 0;
}
