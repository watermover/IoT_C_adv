#include <stdio.h>
#include <string.h>
#include "mathfuncs.h"

static void print_help(const char *prog)
{
    printf("Usage: %s [options]\n", prog);
    printf("Без опций: печатает площадь фигуры (метод Симпсона).\n");
    printf("Опции:\n");
    printf("  --roots   печатать абсциссы точек пересечения кривых\n");
    printf("  --iters   печатать суммарное и по каждой паре число итераций\n");
    printf("  --all     печатать площади всеми методами (прямоугольники, трапеции, Симпсон, Монте‑Карло)\n");
    printf("  --help    показать это сообщение и выйти\n");
}

int main(int argc, char *argv[])
{
    int show_roots = 0;
    int show_iters = 0;
    int show_all   = 0;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) {
            print_help(argv[0]);
            return 0;
        } else if (!strcmp(argv[i], "--roots")) {
            show_roots = 1;
        } else if (!strcmp(argv[i], "--iters")) {
            show_iters = 1;
        } else if (!strcmp(argv[i], "--all")) {
            show_all = 1;
        } else {
            printf("Неизвестный ключ: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    int total_iters = 0;

    if (!show_all) {
        double area = compute_area(&total_iters);
        printf("Area (Simpson) = %.8f\n", area);
    } else {
        double sSq, sTr, sSi, sMc;
        compute_area_all(&sSq, &sTr, &sSi, &sMc, &total_iters);
        printf("Area (rectangles)   = %.8f\n", sSq);
        printf("Area (trapezoidal)  = %.8f\n", sTr);
        printf("Area (Simpson)      = %.8f\n", sSi);
        printf("Area (Monte Carlo)  = %.8f\n", sMc);
    }

    if (show_roots) {
        printf("Roots (x):\n");
        printf("  f1 = f2 : x = %.10f\n", root12);
        printf("  f1 = f3 : x = %.10f\n", root13);
        printf("  f2 = f3 : x = %.10f\n", root23);
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
