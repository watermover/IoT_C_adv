#ifndef MATHFUNCS_H
#define MATHFUNCS_H

double f1(double x);
double f2(double x);
double f3(double x);

double f1_minus_f2(double x);
double f1_minus_f3(double x);
double f2_minus_f3(double x);

typedef double (*func_t)(double);

/* метод бисекции */
double bisection(func_t f, double a, double b,
                 double eps, int max_iter, int *iters);

/* численное интегрирование */
double simpson(func_t f, double a, double b, int n);
double calcIntegralSquare(func_t f, double a, double b, int n);
double calcIntegralTrap(func_t f, double a, double b, int n);
double calcIntegralMonteCarlo(func_t f, double a, double b, int n);

/* корни и количество итераций */
extern double root12, root13, root23;
extern int it12, it13, it23;

/* площадь Симпсоном (как раньше) */
double compute_area(int *total_iters);

/* площади всеми методами по той же области */
void compute_area_all(double *sSquare,
                      double *sTrap,
                      double *sSimpson,
                      double *sMonteCarlo,
                      int    *total_iters);

#endif
