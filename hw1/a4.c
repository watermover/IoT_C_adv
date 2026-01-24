#include <stdio.h>

int main(void) {
    unsigned int N;
    int K;

    if (scanf("%u %d", &N, &K) != 2) return 0;

    unsigned int max_val = 0;

    for (int i = 0; i <= 32 - K; ++i) {
        unsigned int val = (N >> i) & ((1U << K) - 1U);
        if (val > max_val)
            max_val = val;
    }

    printf("%u\n", max_val);
    return 0;
}
