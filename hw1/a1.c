#include <stdio.h>

int main(void) {
    int N;
    int ch;

    if (scanf("%d", &N) != 1) return 0;

    N %= 26;

    ch = getchar();
    if (ch == ' ') ch = getchar();

    while (ch != EOF) {
        if (ch == '.') {
            putchar('.');
            break;
        }

        if (ch >= 'a' && ch <= 'z') {
            ch = 'a' + (ch - 'a' + N) % 26;
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = 'A' + (ch - 'A' + N) % 26;
        } else if (ch == ' ') {
            
        }

        putchar(ch);
        ch = getchar();
    }

    return 0;
}
