#include <stdio.h>

int main(void) {
    unsigned int N;
    if (scanf("%u", &N) != 1) return 0;

    int cnt = 0;
    while (N) {
        N &= (N - 1);  
        ++cnt;
    }

    printf("%d\n", cnt);
    return 0;
}
