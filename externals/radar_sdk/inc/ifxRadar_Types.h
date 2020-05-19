/* ===========================================================================
** Copyright (C) 2018 Infineon Technologies AG. All rights reserved.
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
 * @file ifxRadar_Types.h
 *
 * @brief This file defines the data types used within the API
 *
 */


#ifndef IFX_RADAR_TYPES_H
#define IFX_RADAR_TYPES_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
==============================================================================
   3. TYPES
==============================================================================
*/

#ifdef USE_DOUBLE
typedef double ifx_Float_t;
#else
typedef float ifx_Float_t;
#endif


typedef struct ifx_Polar_s
{
    ifx_Float_t radius;
    ifx_Float_t angle;
} ifx_Polar_t;

#ifdef IFX_HAVE_GENERICS
#ifdef __CDT_PARSER__
#define ifx_type_gen(t, t1, f1, t2, f2) IFX_OK
#define ifx_type_gen2(a, b, t1, t2, f1_1, f1_2, f2_1, f2_2) IFX_OK
#else
#define ifx_type_gen(t, t1, f1, t2, f2) _Generic(t, t1:f1, t2:f2, const t1:f1, const t2:f2)
#define ifx_type_gen2(a, b, t1, t2, f1_1, f1_2, f2_1, f2_2)\
    _Generic(a, t1:_Generic(b, t1:f1_1, t2:f1_2), t2:_Generic(b, t1:f2_1, t2:f2_2))
#endif


#define ifx_real_cmplx_gen(type, float_func, complex_func) \
    ifx_type_gen(type, ifx_Float_t, float_func, ifx_Complex_t, complex_func)
#endif /* IFX_HAVE_GENERICS */

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif

