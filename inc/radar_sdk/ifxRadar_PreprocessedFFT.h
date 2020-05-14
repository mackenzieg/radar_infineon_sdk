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
 * @file ifxRadar_PreprocessedFFT.h
 *
 * @brief Pre-processed FFT is 1D FFT along with pre-processing like mean removal
 * and windowing. Following list of signal processing blocks are used within this module.
 * 1. mean removal (optional)
 * 2. windowing
 * 3. FFT (R2C or C2C)
 *
 */

#ifndef IFX_RADAR_SDK_PREPROCESSED_FFT_H
#define IFX_RADAR_SDK_PREPROCESSED_FFT_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include <stdint.h>

#include "ifxRadar_Error.h"
#include "ifxRadar_FFT.h"
#include "ifxRadar_Math.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Window.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
==============================================================================
   2. DEFINITIONS
==============================================================================
*/
#define LIGHT_SPEED_MPS 299792458
/*
==============================================================================
   3. TYPES
==============================================================================
*/

typedef struct
{
    /** Size of the input (time domain) data samples, in units of sample count. Should be
        non-zero and not greater than FFT size. */
    uint32_t fft_input_len;

    /** FFT type is defined by \ref ifx_FFT_Type_t. Type is defined based on FFT input data nature,
        i.e. real or complex. */
    ifx_FFT_Type_t fft_type;

    /** The FFT parameters are defined by structure \ref ifx_FFT_Size_t. Must be power of 2. */
    ifx_FFT_Size_t fft_size;

    /** Zero value considered as False (skip mean removal feature), while non-zero value would
        be considered as true for mean removal. */
    uint8_t mean_removal_flag;

    /** This value is used to calculate the frequency bins in the FFT output. */
    uint32_t input_sampling_freq_khz;

    /** Window type, length and attenuation used for range FFT */
    ifx_Window_Config_t window_config;

    /** Setting to zero will turn off this feature. If set to non-zero value the effect of different
     * window type is normalized. Changing window type will result in similar magnitude range. */
    uint8_t is_normalized_window;

} ifx_Preprocessed_FFT_Config_t;

typedef struct ifx_Preprocessed_FFT_s* ifx_Preprocessed_FFT_Handle_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief This creates a handle (object) for 1D FFT chain, containing all required settings and internal memories.
 *
 * @param [in] config        Mean removal flag and FFT settings e.g. FFT type, size and input length
 * @param [out] handle        A handle to the 1D pre-processed FFT object
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_create(const ifx_Preprocessed_FFT_Config_t* config,
                                        ifx_Preprocessed_FFT_Handle_t* handle);

/**
 * @brief It calculates 1D FFT for real input with some pre-processing steps like mean removal and windowing.
 *
 * @param [in] handle        A handle to the 1D pre-processed FFT object
 * @param [in] input         Real input vector (single chirp data with either I or Q samples)
 * @param [out] output        FFT output is always complex. But only half of the output
 *                      is useful as next half is just conjugate symmetric part of first half
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_run_r(ifx_Preprocessed_FFT_Handle_t handle,
                                       const ifx_Vector_R_t* input,
                                       ifx_Vector_C_t* output);

/**
 * @brief It calculates 1D FFT for complex input with some pre-processing steps like mean removal and windowing.
 *
 * @param [in] handle        A handle to the 1D pre-processed FFT object
 * @param [in] input         Complex input vector (single chirp data with both I & Q samples)
 * @param [out] output        FFT output is always complex, and full spectrum of FFT size for complex input
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_run_c(ifx_Preprocessed_FFT_Handle_t handle,
                                       const ifx_Vector_C_t* input,
                                       ifx_Vector_C_t* output);

/**
 * @brief This destroys handle (object) for 1D FFT chain along with internal memories.
 *
 * @param  [in] handle        A handle to the 1D pre-processed FFT object
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_destroy(ifx_Preprocessed_FFT_Handle_t handle);

/**
 * @brief This function sets the mean removal flag to true or false.
 *
 * @param [in, out] handle        A handle to the 1D pre-processed FFT object
 * @param [in] flag          Zero means false, else its true. Mean is calculated and removed before FFT
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_set_mean_removal_flag(ifx_Preprocessed_FFT_Handle_t handle,
        uint8_t flag);

/**
 * @brief This function gets the current mean removal flag values used within \ref ifx_Preprocessed_FFT_Handle_t.
 *
 * @param [in] handle        A handle to the 1D pre-processed FFT object
 * @param [out]  flag          Zero means false, else its true
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_get_mean_removal_flag(ifx_Preprocessed_FFT_Handle_t handle,
        uint8_t* flag);

/**
  *@brief This function facilitates to update window parameters used before FFT operation.
 *
 * For example, if window type or its scale needs to be modified, one can update by passing the
 * new window type or attenuation scale in window setting structure defined by \ref ifx_Window_Config_t.
 *
 * @param [in] config        Window configuration defined by \ref ifx_Window_Config_t with new gain value
 *                      or window type
 * @param [in] handle        A handle to the 1D pre-processed FFT object
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_preprocessed_fft_update_window(ifx_Preprocessed_FFT_Handle_t handle,
        const ifx_Window_Config_t* config);

ifx_Error_t ifx_preprocessed_fft_get_freq_axis_info(ifx_Preprocessed_FFT_Handle_t handle,
        ifx_Math_Axis_Spec_t* fft_freq_axis_spec_hz);

#ifdef IFX_HAVE_GENERICS
#define ifx_preprocessed_fft_run(handle, input, output) \
    ifx_vector_gen(input, ifx_preprocessed_fft_run_r, ifx_preprocessed_fft_run_c)(handle, input, output) ;
#endif /* IFX_HAVE_GENERICS */

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_SDK_PREPROCESSED_FFT_H */

