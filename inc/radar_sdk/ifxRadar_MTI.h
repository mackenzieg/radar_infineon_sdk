/* ===========================================================================
** Copyright (C) 2018-2019 Infineon Technologies AG. All rights reserved.
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
 * @file ifxRadar_MTI.h
 *
 * @brief This file defines the API to perform MTI filter.
 *
 *
 */

#ifndef IFX_RADAR_MTI_H
#define IFX_RADAR_MTI_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include "ifxRadar_Error.h"
#include "ifxRadar_Types.h"
#include "ifxRadar_Vector.h"

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

/**
 *
 * @brief MTI filter handle, pointing to a structure of type \ref ifx_MTI_s
 *
 */

typedef struct ifx_MTI_s* ifx_MTI_Handle_t;


/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**

 * @brief This function instantiates a handle to an MTI filter structure \ref ifx_MTI_s
 * which contains information required to perform MTI (Moving Target
 * Indication) filtering. The MTI filter is basically a first order IIR (Infinite
 * Impulse Response) filter.
 *
 * The MTI filter is typically used on a range spectrum to filter out targets that are
 * stationary. Thus the name "Moving Target Indication" filtering. The effect of static target removal
 * with the MTI filter can be seen when applied on the range spectrums from successive FMCW frames. The ifx_MTI_s
 * stores a historical component of range spectrums which have information
 * of static targets in previous spectrums. This historical component is subtracted from
 * the range spectrum of the next incoming FMCW frame.
 *
 * The filter co-efficient \f$ alpha \f$ needs to be specified when creating the
 * an instance of the MTI module.
 *
 * The MTI filtering is realized according to the following formula:
 *
 * \f[
    RSF_n = |RS_n - RSH_{n-1}| \\
    RSH_n = alpha*RS_n + (1 - alpha)*RSH_{n-1}
 * \f]
 *
 * Where:
 * - \f$ RS_n \f$       = current (\f$ n^{th} \f$) input vector
 * - \f$ RSF_n \f$      = current (\f$ n^{th} \f$) MTI filtered output vector
 * - \f$ RSH_{n-1} \f$    = Historical component from the previous \f$ n-1 \f$ input vector filterings
 * - \f$ RSH_n \f$      = Historical component after filtering the current (\f$ n^{th} \f$) input vector
 * - \f$ alpha \f$      = Filter-coefficient of the IIR filter used for MTI
 *
 *
 * @param [in]     alpha_mti_filter Filter coefficient. Valid between 0.0 and 1.0
 * @param [in]     spectrum_length  Length of input / output vectors used within MTI filter
 * @param [out]    handle           The address of a valid MTI filter handle
 * @return         ifx_Error_t      Error Code
 */
ifx_Error_t ifx_mti_create(const ifx_Float_t alpha_mti_filter,
                           const uint32_t spectrum_length,
                           ifx_MTI_Handle_t* handle);

/**
 * @brief Destroys the MTI filter handle.
 *
 * @param [in,out] handle   Valid MTI filter handle
 * @return ifx_Error_t      Error Code
 */
ifx_Error_t ifx_mti_destroy(ifx_MTI_Handle_t handle);

/**
 * @brief Uses a valid MTI filter handle to filter an input vector of real values.
 *
 * @param [in]       handle      Valid MTI filter handle
 * @param [in, out]  spectrum    Pointer to an allocated and populated vector instance defined by \ref ifx_Vector_R_t
 *                               filled with the magnitude spectrum of a target data that is to be filtered
 * @return ifx_Error_t           Error Code
 */
ifx_Error_t ifx_mti_run(ifx_MTI_Handle_t handle, ifx_Vector_R_t* spectrum);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif  // IFX_RADAR_MTI_H
