#include <stdio.h>

int main(void) {
    int N;
    if (scanf("%d", &N) != 1 || N <= 0) return 0;

    int x;
    scanf("%d", &x);
    int max = x;
    int cnt = 1;

    for (int i = 1; i < N; ++i) {
        scanf("%d", &x);
        if (x > max) {
            max = x;
            cnt = 1;
        } else if (x == max) {
            ++cnt;
        }
    }

    printf("%d\n", cnt);
    return 0;
}
