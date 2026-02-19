#include <math.h>
#include <stdlib.h>
#include "mathfuncs.h"

/* функции */
double f1(double x) { return 0.6 * x + 3.0; }

double f2(double x)
{
    double t = x - 2.0;
    return t * t * t - 1.0;
}

double f3(double x) { return 3.0 / x; }

/* разности */
double f1_minus_f2(double x) { return f1(x) - f2(x); }
double f1_minus_f3(double x) { return f1(x) - f3(x); }
double f2_minus_f3(double x) { return f2(x) - f3(x); }

/* бисекция */
double bisection(func_t f, double a, double b,
                 double eps, int max_iter, int *iters)
{
    double fa = f(a);
    double fb = f(b);
    int k = 0;

    if (fa * fb > 0.0)
        return NAN;

    double c, fc;
    while ((b - a) > eps && k < max_iter) {
        c = 0.5 * (a + b);
        fc = f(c);
        if (fa * fc <= 0.0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
        ++k;
    }
    if (iters) *iters = k;
    return 0.5 * (a + b);
}

/* Симпсон */
double simpson(func_t f, double a, double b, int n)
{
    if (n % 2) ++n;
    double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; ++i) {
        double x = a + h * i;
        if (i % 2 == 0)
            s += 2.0 * f(x);
        else
            s += 4.0 * f(x);
    }
    return s * h / 3.0;
}

/* прямоугольники (левые) */
double calcIntegralSquare(func_t f, double a, double b, int n)
{
    double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        s += f(x);
    }
    return s * h;
}

/* трапеции */
double calcIntegralTrap(func_t f, double a, double b, int n)
{
    double h = (b - a) / n;
    double s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        s += f(x);
    }
    return s * h;
}

/* Монте‑Карло */
double calcIntegralMonteCarlo(func_t f, double a, double b, int n)
{
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        double r = (double)rand() / (double)RAND_MAX;
        double x = a + (b - a) * r;
        s += f(x);
    }
    return (b - a) * s / n;
}

/* глобальные корни и итерации (правые пересечения) */
double root12 = NAN;  /* f1 = f2 */
double root13 = NAN;  /* f1 = f3 */
double root23 = NAN;  /* f2 = f3 */
int it12 = 0, it13 = 0, it23 = 0;

/* модуль разностей для площади */
static double diff_f1_f3(double x) { return fabs(f1(x) - f3(x)); }
static double diff_f1_f2(double x) { return fabs(f1(x) - f2(x)); }

/* общая функция: найти корни и вернуть границы x1<x2<x3 */
static int find_roots(double *x1, double *x2, double *x3, int *total_iters)
{
    int it;

    /* интервалы вокруг правых корней по данным GeoGebra */
    root13 = bisection(f1_minus_f3, 0.3, 1.5, 1e-8, 1000, &it);  /* ~0.8541 */
    it13 = it;

    root23 = bisection(f2_minus_f3, 2.7, 3.7, 1e-8, 1000, &it);  /* ~3.2439 */
    it23 = it;

    root12 = bisection(f1_minus_f2, 3.0, 6.0, 1e-8, 1000, &it);  /* пересечение f1 и f2 справа */
    it12 = it;

    if (total_iters)
        *total_iters = it12 + it13 + it23;

    if (isnan(root12) || isnan(root13) || isnan(root23))
        return 0;

    /* по графику: root13 < root23 < root12 */
    *x1 = root13;
    *x2 = root23;
    *x3 = root12;
    return 1;
}

/* площадь Симпсоном (как раньше) */
double compute_area(int *total_iters)
{
    double x1, x2, x3;
    if (!find_roots(&x1, &x2, &x3, total_iters))
        return NAN;

    int n = 2000;

    double s1 = simpson(diff_f1_f3, x1, x2, n);
    double s2 = simpson(diff_f1_f2, x2, x3, n);

    return s1 + s2;
}

/* площади всеми методами по той же области */
void compute_area_all(double *sSquare,
                      double *sTrap,
                      double *sSimpson,
                      double *sMonteCarlo,
                      int    *total_iters)
{
    double x1, x2, x3;
    if (!find_roots(&x1, &x2, &x3, total_iters)) {
        if (sSquare)     *sSquare     = NAN;
        if (sTrap)       *sTrap       = NAN;
        if (sSimpson)    *sSimpson    = NAN;
        if (sMonteCarlo) *sMonteCarlo = NAN;
        return;
    }

    int n = 2000;

    /* [x1,x2]: между f1 и f3 */
    double s1_sq = calcIntegralSquare (diff_f1_f3, x1, x2, n);
    double s1_tr = calcIntegralTrap   (diff_f1_f3, x1, x2, n);
    double s1_si = simpson            (diff_f1_f3, x1, x2, n);
    double s1_mc = calcIntegralMonteCarlo(diff_f1_f3, x1, x2, n);

    /* [x2,x3]: между f1 и f2 */
    double s2_sq = calcIntegralSquare (diff_f1_f2, x2, x3, n);
    double s2_tr = calcIntegralTrap   (diff_f1_f2, x2, x3, n);
    double s2_si = simpson            (diff_f1_f2, x2, x3, n);
    double s2_mc = calcIntegralMonteCarlo(diff_f1_f2, x2, x3, n);

    if (sSquare)     *sSquare     = s1_sq + s2_sq;
    if (sTrap)       *sTrap       = s1_tr + s2_tr;
    if (sSimpson)    *sSimpson    = s1_si + s2_si;
    if (sMonteCarlo) *sMonteCarlo = s1_mc + s2_mc;
}
