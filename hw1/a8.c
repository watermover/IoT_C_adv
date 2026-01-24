#include <stdio.h>

int main(void) {
    long long m;
    if (scanf("%lld", &m) != 1) return 0;

    if (m == 0) {
        printf("0\n");
        return 0;
    }

    long long cnt = 0;
    int pos = 0;               
    while (m > 0) {
        if (pos > 12) {        
            printf("-1\n");
            return 0;
        }

        int r = m % 3;

        if (r == 0) {
            m /= 3;
        } else if (r == 1) {
            cnt++;
            m /= 3;
        } else {             
            cnt++;
            m = m / 3 + 1;
        }

        pos++;
    }

    printf("%lld\n", cnt);
    return 0;
}
