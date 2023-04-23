//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: math.h 523 2007-12-01 19:39:12Z ingo $

#ifndef _HPMATH_H
#define _HPMATH_H
#ifndef USING_DECFLOAT

// SKIP INCLUSION, IF USING DECIMAL FLOATING POINT CLASS !
// <ibl ; 2007-12-01>

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif


/*!    \def    min(a,b) ((a) < (b) ? (a) : (b))
*/

/*!    \def    max(a,b) ((a) > (b) ? (a) : (b))
*/

/*!    \def    abs(a) ((a) < 0 ? -(a) : (a))
*/


#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef abs
#define abs(a) ((a) < 0 ? -(a) : (a))
#endif



double ipow(double x,  int n);
// NEW 10^X ALGORITHM BY CLAUDIO
double i10powx(int n);
// INTERNAL FUNCTIONS
// CREATE A DOUBLE FROM A 64-BIT MANTISSA AND AN EXPONENT
double assemble_double(unsigned int sign,unsigned long long mantissa,int exponent);
// CALCULATE 10^X WITH 64-BIT PRECISION
// RETURN A BASE-2 EXPONENT + A 64-BIT MANTISSA
// MORE ACCURATE THAN STANDARD POW AND IPOW
int _i10pow64(int exp10,unsigned long long *mant);



double square(double x);
double round(double a);



// libm

double modf(double x, double *iptr);
double frexp(double x, int *exp);
double scalbn (double x, int n);
double copysign(double x, double y);

double __ieee754_log(double x);
double __ieee754_log10(double x);
double __ieee754_exp(double x);
double __ieee754_pow(double x, double y);
double __ieee754_sqrt(double x);

double __ieee754_asin(double x); //EDIT BY AL, 29th Oct
double __ieee754_acos(double x);
double __ieee754_atan2(double y, double x);

double sin(double x);
double cos(double x);
double tan(double x);

double atan(double x); //EDIT BY AL, 29th Oct

double floor(double x);
double ceil(double x);


#define log     __ieee754_log
#define log10     __ieee754_log10
#define exp     __ieee754_exp
#define pow     __ieee754_pow
#define sqrt    __ieee754_sqrt

#define asin     __ieee754_asin //EDIT BY AL, 29th Oct
#define acos    __ieee754_acos
#define atan2    __ieee754_atan2

#define log2(x) (log(x) / M_LOG2_E)
#define _fabs(x) ( (x) < 0.0 ? (-x) : (x) )

double fabs(double x);

#define M_E        2.7182818284590452354
#define M_LOG2E        1.4426950408889634074
#define M_LOG10E    0.43429448190325182765
#define M_LN2        0.69314718055994530942
#define M_LN10        2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_TWOPI        (M_PI * 2.0)
#define M_PI_2        1.57079632679489661923
#define M_PI_4        0.78539816339744830962
#define M_3PI_4        2.3561944901923448370E0
#define M_SQRTPI    1.77245385090551602792981
#define M_1_PI        0.31830988618379067154
#define M_2_PI        0.63661977236758134308
#define M_2_SQRTPI    1.12837916709551257390
#define M_SQRT2        1.41421356237309504880
#define M_SQRT1_2    0.70710678118654752440
#define M_LN2LO        1.9082149292705877000E-10
#define M_LN2HI        6.9314718036912381649E-1
#define M_SQRT3        1.73205080756887719000
#define M_IVLN10    0.43429448190325182765 /* 1 / log(10) */
#define M_LOG2_E    0.693147180559945309417
#define M_INVLN2    1.4426950408889633870E0  /* 1 / log(2) */




/*!    \fn    double random()
    \brief    53 bit pseudo RNG on [0,1)
    \retval A double pseudo random number, based on mwc() generator
*/

/*!    \fn double gauss()
    \brief Returns a normal distributed random number.
    \retval A normalized,normal-distributed random number with mu=0.0 and sigma = 1.0
*/

/*!    \fn    double normal(double mu, double sigma)
    \brief  Returns a normal distributed random number with selected parameters.
    \retval A normal-distributed random number with parameters (\a mu, \a sigma)
*/

/*!    \def    prob(p) ((p) >= 1.0 ? 1 : random() < (p))
    \brief  Returns TRUE (1) if a uniform distributed random variable is less than p
*/

double    random(); // 53 bit pseudo RNG on [0,1)
double     gauss();
double     normal(double mu, double sigma);

#define prob(p) ((p) >= 1.0 ? 1 : random() < (p))

/*!
      \fn int qlog10(double x, double *y)
      \brief find y and return so that x=y*(10**return)
     
      \param x source number
      \param *y location for the remainder term
      \retval integer log10 of x
 */
 
int qlog10(double x, double *y);



/*!
    \def ilog10 qlog10
    \brief synonym to better reflect the Integer result type
*/

#define ilog10 qlog10

/*!
    \fn    double dround(double value, int digits)
    \brief Round \c value to \c digits places towards +/- infinity
    \param value The double value to round
    \param digits Number of decimal places
    \retval The rounded value
*/
    
double dround(double value, int digits);

typedef double (*DFUNC1 ) (double);

// Numerical derivatives & root finding...

// Computes nth derivative for f(x) :: R->R
double df(int n,DFUNC1 f, double x);

// Newtonean root iteration for monadic real functions R->R
double newton(DFUNC1 f, double x0,double y);
#define MATH_ERR_NEWTON_INVALID -999.666

#endif // USING_DECFLOAT
#endif // _MATH_H


