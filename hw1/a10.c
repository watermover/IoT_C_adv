#include <stdio.h>
#include <stdint.h>

int extractExp(float x) {
    union {
        float f;
        uint32_t u;
    } conv;

    conv.f = x;
    uint32_t exp = (conv.u >> 23) & 0xFFu;
    return (int)exp;
}

int main(void) {
    float x;
    if (scanf("%f", &x) != 1) return 0;
    printf("%d\n", extractExp(x));
    return 0;
}
