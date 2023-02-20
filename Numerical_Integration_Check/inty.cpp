#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_vegas.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <random>

#include <boost/numeric/odeint.hpp>
#include <matplot/matplot.h>
#include <csv.hpp>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
#define sqr(a) a*a
#define cub(a) a*a*a
#define qrt(a) a*a*a*a
#define epsilon 0.01
#define PI 3.141592653589793238462643


double h3_integrand (double *_x, size_t dim, void *params) {
    (void)(dim); (void)(params);/* avoid unused parameter warnings */
    double* _p = (double*)(params);
    double v = _p[0], c = _p[1];
    double x = _x[0], y = _x[1];
    if(1.0 - x < y)
        return 0;
    double detx = 4.0*v*v*x*y + c*c*c*c*(1.0-x-y)*(1.0-x-y) - c*c*(1.0+v*v)*( x*x-x + y*y-y+2.0*x*y );
    double sqdetx = std::sqrt(detx);
    double i1 = matplot::pi*c*2.0/sqdetx  +  matplot::pi*c*c*c*(1.0-x-y)*(1.0-v*v)/sqdetx/detx;
    return i1;
}


double h3_prod(double v, double c, size_t num_calls = 100000)
{
    double params[2] = {v,c};
    gsl_monte_function G = { &h3_integrand, 2, params };
    ///
    double res, err;
    size_t calls = num_calls;
    double xl[2] = { 0.0, 0.0 };
    double xu[2] = { 1.0, 1.0 };
    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    gsl_monte_vegas_state *s = gsl_monte_vegas_alloc (2);
    do{
        gsl_monte_vegas_integrate (&G, xl, xu, 2, calls/5, r, s, &res, &err);
    } while (fabs (gsl_monte_vegas_chisq (s) - 1.0) > 0.5);
    gsl_monte_vegas_free (s);
    gsl_rng_free (r);
    return res;
}

double h3_check(double v, double c)
{
    double s = c/v;
    double chat = c/sqrt(1.0 + sqr(v));
    double factor1 =  PI/sqrt(1.0 + sqr(v));
    double factor2 = 4.0 * sqr(s) / (1.0 - sqr(s));
    double term1   = atan( sqrt(sqr(s) - sqr(chat)) / chat );
    double term2   = asin( sqrt( 1.0-sqr(chat) ) );
    double term3   = sqrt(sqr(s)-sqr(chat));
    double term4   = sqrt(1.0-sqr(chat));
    // cout << factor1 << " " << factor2 << " " << term1 << " " << term2 << " " << term3 << endl;
    return factor1*factor2*( term1/term3 - term2/term4 );
}


int main()
{
    const int num_tests = 100; // number of random tests to run
    const double tol = 1e-1;   // tolerance for comparing double values

    std::default_random_engine rng;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < num_tests; i++) {
        double x = dist(rng);
        double y = dist(rng);

        double expected = h3_check(x, y);
        double actual   = h3_prod (x, y);

        if (std::fabs(actual - expected) > tol) {
            std::cout << "Test " << i + 1 << " failed. Inputs: (" << x << ", " << y << ")." << std::endl;
            std::cout << "Expected: " << expected << ", actual: " << actual << std::endl;
            return 1; // exit with non-zero status to indicate failure
        }
    }

    std::cout << "All " << num_tests << " tests passed!" << std::endl;
    return 0;
}


