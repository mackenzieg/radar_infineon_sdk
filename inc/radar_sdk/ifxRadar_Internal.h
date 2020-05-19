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
 * @file ifxRadar_Internal.h
 *
 * @brief This file defines standard math.h functions as Macros to use float or double precision
 *
 */

#ifndef IFX_RADAR_INTERNAL_H
#define IFX_RADAR_INTERNAL_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#define _USE_MATH_DEFINES
#include <math.h>

#include "ifxRadar_Types.h"

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
#define CABS   cabs
#define FABS   fabs
#define CREAL  creal
#define CIMAG  cimag
#define LOG10  log10
#define LOGN   log
#define SIN    sin
#define ASIN   asin
#define COS    cos
#define COSH   cosh
#define ATAN   atan
#define ATAN2  atan2
#define ACOS   acos
#define ACOSH  acosh
#define HYPOT  hypot
#define POW    pow
#define IFX_PI M_PI
#else
#define CABS   cabsf
#define FABS   fabsf
#define CREAL  crealf
#define CIMAG  cimagf
#define LOG10  log10f
#define LOGN   logf
#define SIN    sinf
#define ASIN   asinf
#define COS    cosf
#define ACOS   acosf
#define ATAN   atanf
#define ATAN2  atan2f
#define COSH   coshf
#define ACOSH  acoshf
#define HYPOT  hypotf
#define POW    powf
#define IFX_PI (ifx_Float_t)(M_PI)
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif  // IFX_RADAR_INTERNAL_H
