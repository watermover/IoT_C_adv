#include <stdio.h>
#include <string.h>
#include "mathfuncs.h"

static void print_help(const char *prog)
{
    printf("Usage: %s [options]\n", prog);
    printf("Без опций: печатает площадь фигуры, найденную разными методами.\n");
    printf("Опции:\n");
    printf("  --roots   печатать абсциссы точек пересечения кривых\n");
    printf("  --iters   печатать суммарное и по каждой паре число итераций\n");
    printf("  --help    показать это сообщение и выйти\n");
}

int main(int argc, char *argv[])
{
    int show_roots = 0;
    int show_iters = 0;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) {
            print_help(argv[0]);
            return 0;
        } else if (!strcmp(argv[i], "--roots")) {
            show_roots = 1;
        } else if (!strcmp(argv[i], "--iters")) {
            show_iters = 1;
        } else {
            printf("Неизвестный ключ: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    double sSquare, sTrap, sSimpson, sMonte;
    int total_iters = 0;

    compute_area_all(&sSquare, &sTrap, &sSimpson, &sMonte, &total_iters);

    printf("Area (rectangles)   = %.8f\n", sSquare);
    printf("Area (trapezoidal)  = %.8f\n", sTrap);
    printf("Area (Simpson)      = %.8f\n", sSimpson);
    printf("Area (Monte Carlo)  = %.8f\n", sMonte);

    if (show_roots) {
        printf("Roots (x):\n");
        printf("  f1 = f2 : x = %.8f\n", root12);
        printf("  f1 = f3 : x = %.8f\n", root13);
        printf("  f2 = f3 : x = %.8f\n", root23);
    }

    if (show_iters) {
        printf("Iterations:\n");
        printf("  f1 vs f2: %d\n", it12);
        printf("  f1 vs f3: %d\n", it13);
        printf("  f2 vs f3: %d\n", it23);
        printf("  total   : %d\n", total_iters);
    }

    return 0;
}
