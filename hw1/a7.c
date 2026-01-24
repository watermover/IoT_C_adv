#include <stdio.h>

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    int x, ans = 0;
    for (int i = 0; i < N; ++i) {
        scanf("%d", &x);
        ans ^= x;
    }

    printf("%d\n", ans);
    return 0;
}
