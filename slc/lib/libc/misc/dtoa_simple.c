#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "gdtoa/gdtoaimp.h" // for (n)rv_alloc


char *dtoa_simple(double x, int mode, int _prec, int *decpt, int *sign, char **rve)
{
    if (unlikely(isnan(x))) { *decpt = 9999; return nrv_alloc("NaN", rve, 3); }
    
    *sign = signbit(x);
    if (unlikely(isinf(x))) { *decpt = 9999; return nrv_alloc("Infinity", rve, 8); }

    if (*sign)
        x = -x;

#ifdef __slc_short_double__
    if (x < __FLT_MIN__)
#else
    if (x < __DBL_MIN__)
#endif
    {
        *decpt = 1;
        return nrv_alloc("0", rve, 1);
    }

    /* -- find exponent and normalize -- */
    int exp = 1;
#ifndef __slc_short_double__
    // Cannot divide by 1e100 with 32-bit floats
    while (x >= 1e100) { x /= 1e100; exp += 100; }
#endif
    while (x >= 1e10) { x /= 1e10; exp += 10; }
    while (x >= 10.) { x /= 10.; ++exp; }
#ifndef __slc_short_double
    while (x && x < 1e-100) { x *= 1e100; exp -= 100; }
#endif
    while (x && x < 1e-10) { x *= 1e10; exp -= 10; }
    while (x && x < 1.0) { x *= 10.; --exp; }

    int k = exp;
    int prec = _prec;
    if (mode == 2 && prec <= 0)
        prec = 1;
    else if (mode == 3) {
        prec += exp + 1;
        if (prec <= 0) { k = -_prec; prec = 1; }
    }
    if (prec > 15) prec = 15;

    char *buf = rv_alloc(prec);
    char *p = buf;
    unsigned long d;

    while(prec--) {
        d = (unsigned long)x;
        *p++ = d + '0';
        x = (x - d) * 10.;
    };

    unsigned long last = (unsigned long) x;
    if (last == 9) {
        // need to round off
        while (*--p == '9')
            if (p == buf) {
                ++k;
                *p++ = '1';
                goto ret;
            }
        ++*p++;
    }
    // remove trailing zeroes
    while(*--p == '0'); ++p;

ret:
    *decpt = k;
    // trailing nul
    *p = 0;

    if (rve) *rve = p;
    return buf;
}

