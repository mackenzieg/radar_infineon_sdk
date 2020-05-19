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
 * @file ifxRadar_RangeDopplerMap.h
 *
 * @brief Range doppler map is a 2D FFT transform on time domain data and the output of this
 * module is 2D matrix of real values (absolute magnitude of FFT transform). Here spectrum means,
 * real absolute amplitude values calculated from complex FFT output.
 * Range doppler spectrum has two processing chains, one is over time domain chirp data (i.e. fast time) named as
 * range FFT computation, second processing chain is over all range FFT bins (i.e. slow time) named as doppler FFT.
 * Afterwards, thresholding is applied on real 2D matrix.
 *
 * Block level signal processing modules looks like following;
 *
 * Raw Data => Range FFT => Matrix transpose => Doppler FFT => FFT Shift => Absolute => Thresholding => Scale conversion
 *
 * In order to compute range doppler spectrum from real/complex IQ raw ADC input data, following signal processing
 * chain is applied;
 * - Range spectrum => mean removal (optional), windowing, zero padding, FFT transform
 * - Matrix transpose
 * - Doppler spectrum => mean removal (optional), windowing, zero padding, FFT transform
 * - Absolute amplitude spectrum on 2D matrix
 * - FFT shift to move DC bin to the center of the spectrum
 * - Thresholding (Values below threshold are clipped to threshold value)
 * - Scaling (to scale up or down the spectrum)
 *
 * Range doppler spectrum output format:
 * - By default dB scale, Linear scale is also possible
 * - Rows of matrix: Range with 0 (first row) to Max (last row) of matrix. For real input, only positive half
 *   of spectrum is computed, thus range is only computed for positive half of spectrum
 * - Columns of matrix: Speed values are mapped with DC in center and positive half on right
 *   and negative on left
 */

#ifndef IFX_RADAR_RANGE_DOPPLER_MAP_H
#define IFX_RADAR_RANGE_DOPPLER_MAP_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include <stdint.h>

#include "ifxRadar_Error.h"
#include "ifxRadar_FFT.h"
#include "ifxRadar_Matrix.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Window.h"
#include "ifxRadar_Math.h"
#include "ifxRadar_PreprocessedFFT.h"

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

typedef struct
{
    /** Threshold is in always linear scale, should be greater than 1-e6.
     * Range spectrum output values below this are set to 1-e6 (-120dB) */
    ifx_Float_t spect_threshold;

    /** Linear or dB scale for the output of range spectrum module */
    ifx_Scale_Type_t output_scale_type;

    /** Preprocessed FFT settings for range FFT e.g. mean removal, FFT settings */
    ifx_Preprocessed_FFT_Config_t range_fft_config;

    /** Preprocessed FFT settings for doppler FFT e.g. mean removal, FFT settings */
    ifx_Preprocessed_FFT_Config_t doppler_fft_config;

    /** This value is used to calculate the range bins in the range axis of the output range
     * doppler map. This value should describe the difference between min freq and max freq in the chirp*/
    ifx_Float_t chirp_bandwidth_khz;

    /** This value is used to calculate the speed bins in the speed axis of the output range
     * doppler map. This value should describe the time of one chirp, including any pre/post
     * delays  */
    ifx_Float_t chirp_repetition_time_sec;

    /** This value is used to calculate the speed bins in the speed axis of the output range
     * doppler map. This value should describe the center frequency of the chirp*/
    ifx_Float_t center_frequency_khz;


} ifx_Range_Doppler_Map_Config_t;

/**
 * @brief Refer to the struct type \ref ifx_Range_Doppler_Map_s
 */
typedef struct ifx_Range_Doppler_Map_s* ifx_Range_Doppler_Map_Handle_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief Aim of this method is to create a range doppler spectrum handle (object), based on the input parameters
 * for range spectrum and doppler spectrum e.g.
 * Range spectrum:
 *  - samples per chirp (defines zero padding for FFT)
 *  - mean removal flag
 *  - window type, length and attenuation
 *  - FFT type (\ref ifx_FFT_Type_t)
 *  - FFT size (\ref ifx_FFT_Size_t)
 * Doppler spectrum:
 *  - number of chirps (defines zero padding for FFT)
 *  - mean removal flag
 *  - window type, length and attenuation
 *  - FFT type (always FFT_TYPE_C2C)
 *  - FFT size (\ref ifx_FFT_Size_t)
 * Threshold:
 *  - Output of range doppler spectrum module below this threshold is clipped to this threshold.
 * Scale type:
 *  - convert output to dB scale or keep linear scale.
 *
 * @param [in]  config   Contains configuration for range/doppler FFT, e.g. mean removal,
 *                       window settings, FFT type and size.
 * @param [out] handle   A handle to the new created processor instance.
 *
 * @return ifx_Error_t   Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_create(const ifx_Range_Doppler_Map_Config_t* config,
        ifx_Range_Doppler_Map_Handle_t* handle);

/**
 * @brief Performs destruction of range doppler spectrum handle (object) to clear internal states and memories
 * used for range doppler spectrum calculation.
 *
 * @param [in] handle   A handle to the range doppler spectrum processing object
 * @return ifx_Error_t  Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_destroy(ifx_Range_Doppler_Map_Handle_t handle);

/**
 * @brief Performs signal processing on a real input I or Q (e.g. mean removal, windowing, zero padding, FFT transform)
 * and produces a real amplitude range doppler spectrum as output.
 *
 * @param [in] handle   A handle to the range doppler spectrum processing object
 * @param [in] input    The real (i.e. either I or Q channel) time domain input data matrix,
 *                      with rows as chirps and columns as samples per chirp
 * @param [out] output  Output is always a real matrix containing absolute amplitude spectrum in linear or dB scale.
 *
 * @return ifx_Error_t  Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_run_r(ifx_Range_Doppler_Map_Handle_t handle,
                                        const ifx_Matrix_R_t* input,
                                        ifx_Matrix_R_t* output);

/**
 * @brief Performs signal processing on a complex input IQ (e.g. mean removal, windowing, zero padding, FFT transform)
 * and produces a real amplitude range doppler spectrum as output.
 *
 * @param [in] handle   A handle to the range doppler spectrum processing object
 * @param [in] input    The complex (i.e. both IQ channels) time domain input data matrix matrix,
 *                      with rows as chirps and columns as samples per chirp
 * @param [out] output  Output is always a real matrix containing absolute amplitude spectrum in linear or dB scale.
 * @return ifx_Error_t  Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_run_c(ifx_Range_Doppler_Map_Handle_t handle,
                                        const ifx_Matrix_C_t* input,
                                        ifx_Matrix_R_t* output);

/**
 * @brief This method modifies the threshold value set within the range doppler spectrum handle.
 * Idea is to provide a runtime modification option to change threshold without destroy/create handle.
 * @param handle       A handle to the range doppler spectrum processing object
 * @param threshold    Threshold in linear scale. Range doppler spectrum output values below this are set to threshold
 *                     value at the moment.
 * @return ifx_Error_t Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_set_threshold(ifx_Range_Doppler_Map_Handle_t handle,
        const ifx_Float_t threshold);

/**
 * @brief This method is used to get the currently used threshold within range doppler spectrum handle.
 *
 * @param handle       A handle to the range doppler spectrum processing object
 * @param threshold    Current threshold value used internally in \ref ifx_Range_Doppler_Map_Handle_t.
 *
 * @return ifx_Error_t Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_get_threshold(const ifx_Range_Doppler_Map_Handle_t handle,
        ifx_Float_t* threshold);

/**
 * @brief Configure at runtime, the range doppler spectrum output to linear or dB scale in the handle.
 *
 * @param [in] handle      A handle to the range doppler spectrum processing object
 * @param [in] output_scale_type  Linear or dB are the possible options.
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_doppler_map_set_output_scale_type(ifx_Range_Doppler_Map_Handle_t handle,
        const ifx_Scale_Type_t output_scale_type);
/**
 * @brief Returns current range doppler spectrum output scale i.e. linear or dB scale used within the handle.
 *
 * @param [in] handle       A handle to the range doppler spectrum processing object
 * @param [out] output_scale_type  Linear or dB are the possible options.
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_doppler_map_get_output_scale_type(const ifx_Range_Doppler_Map_Handle_t handle,
        ifx_Scale_Type_t* output_scale_type);

/**
 * @brief This function facilitates to update range window used within range doppler spectrum handle.
 *
 * For example, if range window type or its scale needs to be modified, one can update by passing the
 * new window type or attenuation scale in window configuration structure defined by \ref ifx_Window_Config_t.
 *
 * @param config             Window settings with new gain value or window type but with same length
 *                           as passed during range doppler handle creation
 * @param handle             A handle to the range doppler spectrum processing object
 *
 * @return ifx_Error_t       Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_update_range_window(const ifx_Window_Config_t* config,
        ifx_Range_Doppler_Map_Handle_t handle);
/**
 * @brief This function facilitates to update doppler window used within range doppler spectrum handle.
 *
 * For example, if doppler window type or its scale needs to be modified, one can update by passing the
 * new window type or attenuation scale in window configuration structure defined by \ref ifx_Window_Config_t.
 *
 * @param config             Window settings with new gain value or window type but with same length
 *                           as passed during range doppler handle creation
 * @param handle             A handle to the range doppler spectrum processing object
 *
 * @return ifx_Error_t       Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_doppler_map_update_doppler_window(const ifx_Window_Config_t* config,
        ifx_Range_Doppler_Map_Handle_t handle);

/**
 * @brief Returns information about range axis (each column) of range-doppler map output specified by the type
 * \ref ifx_Math_Axis_Spec_t. The units of the values are m (meters).
 *
 * @param [in] handle        A handle to the range doppler spectrum processing object
 * @param [out] range_axis_spec_m  Structure of type \ref ifx_Math_Axis_Spec_t
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_doppler_map_get_range_axis_info(const ifx_Range_Doppler_Map_Handle_t handle,
        ifx_Math_Axis_Spec_t* range_axis_spec_m);

/**
 * @brief Returns information about speed axis (each row) of range-doppler map output specified by the type
 * \ref ifx_Math_Axis_Spec_t. The units of the values are m/s (meters/second).
 *
 * @param [in] handle        A handle to the range doppler spectrum processing object
 * @param [out] speed_axis_spec_mps  Structure of type \ref ifx_Math_Axis_Spec_t
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_doppler_map_get_speed_axis_info(const ifx_Range_Doppler_Map_Handle_t handle,
        ifx_Math_Axis_Spec_t* speed_axis_spec_mps);

#ifdef IFX_HAVE_GENERICS
/**
 * @brief Performs signal processing on a real/complex IQ time domain input data (including mean removal, windowing,
 * zero padding, FFT transform) for both range and doppler dimensions and produces a real amplitude spectrum as output.
 * Signal processing chain for range doppler spectrum is following;
 * Raw Data --> Range FFT --> Matrix transpose --> Doppler FFT --> FFT Shift --> Absolute --> Thresholding --> Scaling
 *
 * This method is used for both real input as well as complex input raw ADC data. It internally calculates R2C or
 * C2C FFT transform based on the \ref ifx_FFT_Type_t configured in the \ref ifx_Range_Doppler_Map_Handle_t handle.
 *
 * @param [in] handle   A handle to the range doppler spectrum processing object.
 * @param [in] input    The complex (\ref ifx_Matrix_C_t) or real (\ref ifx_Matrix_R_t) time domain
 *                      input data matrix, with rows as chirps and columns as samples per chirp.
 * @param [out] output  Output is always a real matrix defined by i.e. \ref ifx_Matrix_R_t,
 *                      containing absolute amplitude spectrum in linear or dB scale.
 *
 * @return ifx_Error_t  Error code based on the type of error detected.
 *
 */
#define ifx_range_doppler_map_run(handle, input, output) \
    ifx_matrix_gen(input, ifx_range_doppler_map_run_r, ifx_range_doppler_map_run_c)(handle, input, output)
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_RANGE_DOPPLER_MAP_H */

