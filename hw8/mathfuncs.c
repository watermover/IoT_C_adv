#include <math.h>
#include <stdlib.h>
#include "mathfuncs.h"

/* исходные функции */
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

/* метод бисекции на [a,b] */
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

/* интеграл: метод прямоугольников (левые) */
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

/* интеграл: метод трапеций */
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

/* интеграл: метод Симпсона */
double calcIntegralSimpson(func_t f, double a, double b, int n)
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

/* интеграл: метод Монте‑Карло */
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

/* глобальные корни и количество итераций */
double root12 = NAN, root13 = NAN, root23 = NAN;
int it12 = 0, it13 = 0, it23 = 0;

/* модули разностей для площади */
static double diff_f1_f2(double x) { return fabs(f1(x) - f2(x)); }
static double diff_f2_f3(double x) { return fabs(f2(x) - f3(x)); }

/* поиск корня: грубый скан + бисекция */
static double find_root_scan(func_t f, double x_min, double x_max,
                             double step, double eps, int *iters)
{
    double x1 = x_min;
    double y1 = f(x1);
    int total_iters = 0;

    for (double x2 = x1 + step; x2 <= x_max; x2 += step) {
        double y2 = f(x2);
        if (y1 * y2 <= 0.0) {
            int it = 0;
            double r = bisection(f, x1, x2, eps, 1000, &it);
            total_iters += it;
            if (iters) *iters = total_iters;
            return r;
        }
        x1 = x2;
        y1 = y2;
    }
    if (iters) *iters = total_iters;
    return NAN;
}

/* сортировка трёх чисел по возрастанию */
static void sort3(double *a, double *b, double *c)
{
    double x = *a, y = *b, z = *c;
    if (x > y) { double t = x; x = y; y = t; }
    if (y > z) { double t = y; y = z; z = t; }
    if (x > y) { double t = x; x = y; y = t; }
    *a = x; *b = y; *c = z;
}

/* вычисление площадей всеми методами */
void compute_area_all(double *sSquare,
                      double *sTrap,
                      double *sSimpson,
                      double *sMonteCarlo,
                      int    *total_iters)
{
    root12 = find_root_scan(f1_minus_f2, -5.0, 5.0, 0.1, 1e-6, &it12);
    root13 = find_root_scan(f1_minus_f3, -5.0, 5.0, 0.1, 1e-6, &it13);
    root23 = find_root_scan(f2_minus_f3, -5.0, 5.0, 0.1, 1e-6, &it23);

    if (total_iters)
        *total_iters = it12 + it13 + it23;

    if (isnan(root12) || isnan(root13) || isnan(root23)) {
        if (sSquare)     *sSquare     = NAN;
        if (sTrap)       *sTrap       = NAN;
        if (sSimpson)    *sSimpson    = NAN;
        if (sMonteCarlo) *sMonteCarlo = NAN;
        return;
    }

    double xA = root12, xB = root13, xC = root23;
    sort3(&xA, &xB, &xC);

    int n = 2000;

    double s1_sq = calcIntegralSquare (diff_f1_f2, xA, xB, n);
    double s2_sq = calcIntegralSquare (diff_f2_f3, xB, xC, n);

    double s1_tr = calcIntegralTrap   (diff_f1_f2, xA, xB, n);
    double s2_tr = calcIntegralTrap   (diff_f2_f3, xB, xC, n);

    double s1_si = calcIntegralSimpson(diff_f1_f2, xA, xB, n);
    double s2_si = calcIntegralSimpson(diff_f2_f3, xB, xC, n);

    double s1_mc = calcIntegralMonteCarlo(diff_f1_f2, xA, xB, n);
    double s2_mc = calcIntegralMonteCarlo(diff_f2_f3, xB, xC, n);

    if (sSquare)     *sSquare     = s1_sq + s2_sq;
    if (sTrap)       *sTrap       = s1_tr + s2_tr;
    if (sSimpson)    *sSimpson    = s1_si + s2_si;
    if (sMonteCarlo) *sMonteCarlo = s1_mc + s2_mc;
}
