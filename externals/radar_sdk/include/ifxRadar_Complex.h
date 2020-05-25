/* ===========================================================================
** Copyright (C) 2019 Infineon Technologies AG. All rights reserved.
** ===========================================================================
**
** ===========================================================================
** Infineon Technologies AG (INFINEON) is supplying this file for use
** exclusively with Infineon's sensor products. This file can be freely
** distributed within development tools and software supporting such
** products.
**
** THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
** OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
** INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON
** WHATSOEVER.
** ===========================================================================
*/
/**
 * @file ifxRadar_Complex.h
 *
 * @brief This file defines the data types used within the API
 *
 */


#ifndef IFX_RADAR_COMPLEX_H
#define IFX_RADAR_COMPLEX_H

/*
==============================================================================
    1. INCLUDE FILES
==============================================================================
*/

#ifdef IFX_HAVE_STD_COMPLEX
#include <complex.h>
#endif
#include "ifxRadar_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
==============================================================================
2. DEFINITIONS
==============================================================================
*/

/*
==============================================================================
3. TYPES
==============================================================================
*/

#if defined(_Complex_I) && defined(complex) && defined(I)

#if IFX_USE_DOUBLE
typedef double complex ifx_Complex_t;

#define IFX_COMPLEX_REAL(c) (creal(c))
#define IFX_COMPLEX_IMAG(c) (cimag(c))

#else // #if IFX_USE_DOUBLE
typedef float complex ifx_Complex_t;

#define IFX_COMPLEX_REAL(c) (crealf(c))
#define IFX_COMPLEX_IMAG(c) (cimagf(c))

#endif  // #if IFX_USE_DOUBLE
#define IFX_COMPLEX_SET_REAL(c, r) do{ c = r + IFX_COMPLEX_IMAG(c)*I; } while(0)
#define IFX_COMPLEX_SET_IMAG(c, i) do{ c = IFX_COMPLEX_REAL(c) + i*I; } while(0)
#define IFX_COMPLEX_DEF(r, i) (r + i*I)
#define IFX_COMPLEX_SET(c, r, i) do { c = (r) + ((i)*I); } while(0)
#else // defined(_Complex_I) && defined(complex) && defined(I)

typedef struct ifx_Complex_s
{
    ifx_Float_t data[2];
} ifx_Complex_t;

#define IFX_COMPLEX_REAL(c) ((c).data[0])
#define IFX_COMPLEX_IMAG(c) ((c).data[1])
#define IFX_COMPLEX_SET_REAL(c, r) do{ (c).data[0] = r; } while(0)
#define IFX_COMPLEX_SET_IMAG(c, i) do{ (c).data[1] = i; } while(0)
#define IFX_COMPLEX_DEF(r, i) { .data = {r, i} }
#define IFX_COMPLEX_SET(c, r, i) \
    do { IFX_COMPLEX_SET_REAL(c, r); IFX_COMPLEX_SET_IMAG(c, i); } while(0)
#endif // defined(_Complex_I) && defined(complex) && defined(I)



#define IFX_COMPLEX_IS_EQUAL(a, b) \
    (IFX_COMPLEX_REAL(a) == IFX_COMPLEX_REAL(b)) && \
    (IFX_COMPLEX_IMAG(a) == IFX_COMPLEX_IMAG(b))


/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

ifx_Float_t ifx_complex_abs(ifx_Complex_t z);
ifx_Complex_t ifx_complex_conj(ifx_Complex_t z);

ifx_Complex_t ifx_complex_add(ifx_Complex_t a, ifx_Complex_t b);
ifx_Complex_t ifx_complex_sub(ifx_Complex_t a, ifx_Complex_t b);
ifx_Complex_t ifx_complex_mul(ifx_Complex_t a, ifx_Complex_t b);
ifx_Complex_t ifx_complex_div(ifx_Complex_t a, ifx_Complex_t b);

ifx_Complex_t ifx_complex_add_scalar(ifx_Complex_t a, ifx_Float_t b);
ifx_Complex_t ifx_complex_sub_scalar(ifx_Complex_t a, ifx_Float_t b);
ifx_Complex_t ifx_complex_mul_scalar(ifx_Complex_t a, ifx_Float_t b);
ifx_Complex_t ifx_complex_div_scalar(ifx_Complex_t a, ifx_Float_t b);

ifx_Complex_t ifx_complex_ln(ifx_Complex_t z);
ifx_Complex_t ifx_complex_log10(ifx_Complex_t z);
ifx_Float_t ifx_complex_arg(ifx_Complex_t z);
ifx_Polar_t ifx_complex_to_polar(ifx_Complex_t z);
ifx_Complex_t ifx_complex_from_polar(ifx_Polar_t zp);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_COMPLEX_H */

