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
 * @file ifxRadarSDK.h
 *
 * @brief Master header file for Radar SDK
 *
 */
#ifndef IFX_RADAR_SDK_H
#define IFX_RADAR_SDK_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include "ifxRadar_DeviceControl.h"
#include "ifxRadar_Error.h"
#include "ifxRadar_FFT.h"
#include "ifxRadar_Math.h"
#include "ifxRadar_Matrix.h"
#include "ifxRadar_MTI.h"
#include "ifxRadar_PeakSearch.h"
#include "ifxRadar_PreprocessedFFT.h"
#include "ifxRadar_RangeDopplerMap.h"
#include "ifxRadar_RangeSpectrum.h"
#include "ifxRadar_Types.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Window.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
/*
==============================================================================
   2. LOCAL DEFINITIONS
==============================================================================
*/

#define IFX_RADAR_SDK_VERSION_MAJOR    1
#define IFX_RADAR_SDK_VERSION_MINOR    0
#define IFX_RADAR_SDK_VERSION_PATCH    1

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief This function returns the radar sdk version as a string.
 *        The format is according to semantic versioning v2.0 standard
 *        see https://semver.org/.
 *
 * @return Pointer to the string containing the version.
 */
const char* ifx_radar_sdk_get_version_string(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_SDK_H */

