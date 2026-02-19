#ifndef MATHFUNCS_H
#define MATHFUNCS_H

/* исходные функции */
double f1(double x);
double f2(double x);
double f3(double x);

/* разности для поиска корней */
double f1_minus_f2(double x);
double f1_minus_f3(double x);
double f2_minus_f3(double x);

/* указатель на функцию одного аргумента */
typedef double (*func_t)(double);

/* поиск корня методом бисекции на [a,b] */
double bisection(func_t f, double a, double b,
                 double eps, int max_iter, int *iters);

/* численное интегрирование */
double calcIntegralSquare(func_t f, double a, double b, int n);
double calcIntegralTrap(func_t f, double a, double b, int n);
double calcIntegralSimpson(func_t f, double a, double b, int n);
double calcIntegralMonteCarlo(func_t f, double a, double b, int n);

/* найденные корни и количество итераций */
extern double root12, root13, root23;
extern int it12, it13, it23;

/* вычисление площадей всеми методами */
void compute_area_all(double *sSquare,
                      double *sTrap,
                      double *sSimpson,
                      double *sMonteCarlo,
                      int    *total_iters);

#endif
