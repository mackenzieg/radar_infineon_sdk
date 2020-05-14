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
 * @file ifxRadar_RangeSpectrum.h
 *
 * @brief Range spectrum is a 1D FFT transform of time domain data over chirps (also known as Fast time).
 * Here spectrum means, a vector of real absolute amplitude values calculated from complex FFT output.
 * Following signal processing blocks are used for range spectrum;
 *
 * Raw ADC data --> Mean Removal (optional) --> Windowing --> FFT -->  Absolute --> thresholding (Linear) --> Scale (Linear or dB)
 *
 * This ifxRadar_RangeSpectrum module defines an API to create & destroy Range spectrum object (handle) and
 * to compute range spectrums along with some helper setter/getter functions to modify few parameters without
 * destroying and recreating range spectrum handle.
 *
 * Range spectrum format without FFT shift:
 *
 * DC bin --> Positive Half --> Negative half
 *
 * Range spectrum format with FFT shift:
 *
 * Negative half --> DC bin --> Positive Half
 *
 */

#ifndef IFX_RADAR_RANGE_SPECTRUM_H
#define IFX_RADAR_RANGE_SPECTRUM_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include <stdint.h>

#include "ifxRadar_Error.h"
#include "ifxRadar_FFT.h"
#include "ifxRadar_PreprocessedFFT.h"
#include "ifxRadar_Matrix.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Window.h"
#include "ifxRadar_Math.h"

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
 * @brief This enumeration contains supported modes of range spectrum.
 * - Single Chirp
 * - Coherent Integration
 * - Max Energy Spectrum
 *
 */
typedef enum
{
    /** The range spectrum is calculated just for one specified chirp in a frame.
     *  @image html RadarSpectrumMode_SingleChirp.png "Flow - RANGE_SPECTRUM_MODE_SINGLE_CHIRP" width=600cm  */
    RANGE_SPECTRUM_MODE_SINGLE_CHIRP = 0U,

    /** The range spectrum is calculated as a coherent integration of all chirps in a frame.
     *  @image html RadarSpectrumMode_CoherentInteg.png "Flow - RANGE_SPECTRUM_MODE_COHERENT_INTEGRATION" width=600cm */
    RANGE_SPECTRUM_MODE_COHERENT_INTEGRATION = 1U,

    /** The range spectrum is calculated for one chirp that is identified to have the maximum energy.
     *  @image html RadarSpectrumMode_MaxEnergy.png "Flow - RANGE_SPECTRUM_MODE_MAX_ENERGY" width=600cm  */
    RANGE_SPECTRUM_MODE_MAX_ENERGY = 2U,

    /** The range spectrum will be calculated for every chirp, the maximum bin per column is considered. */
    RANGE_SPECTRUM_MODE_MAX_BIN = 3U
} ifx_Range_Spectrum_Mode_t;


typedef struct
{
    /** Threshold is in always linear scale, should be greater than 1-e6.
     * Range spectrum output values below this are set to 1-e6 (-120dB) */
    ifx_Float_t spect_threshold;

    /** Linear or dB scale for the output of range spectrum module */
    ifx_Scale_Type_t output_scale_type;

    /** Preprocessed FFT settings for range FFT e.g. mean removal, FFT settings */
    ifx_Preprocessed_FFT_Config_t fft_config;

    /** Non-zero positive number with an upper limit not defined yet */
    uint32_t num_of_chirps_per_frame;

    /** The chirp bandwidth is the difference between min freq and max freq in the chirp
     * This value is used to calculate the range bins in the range axis of the output range
     * spectrum. */
    uint32_t chirp_bandwidth_khz;

} ifx_Range_Spectrum_Config_t;

/**
 * @brief Refer to the struct type \ref ifx_Range_Spectrum_s
 */
typedef struct ifx_Range_Spectrum_s* ifx_Range_Spectrum_Handle_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief Aim of this method is to create a range spectrum handle (object), based on the input parameters
 * e.g. number of chirps, samples per chirp (defines zero padding), mean removal flag, Window settings,
 * FFT type (\ref ifx_FFT_Type_t) and FFT size (\ref ifx_FFT_Size_t). This handle also contains
 * a FFT handle (R2C or C2C) for computing the FFT, temporary memories to compute intermediate results etc.
 *
 * During this handle creation, two cases have been taken into the consideration, regarding nature of the
 * input from sensor and is defined by \ref ifx_FFT_Type_t,
 * 1. Real input data defined by FFT_TYPE_R2C
 * 2. Complex input data defined by FFT_TYPE_C2C
 * In case 1, a special handle has been used to make FFT operation twice faster and memory efficient.
 * However, it is important to use the correct spectrum run function \ref ifx_range_spectrum_run_r or
 * \ref ifx_range_spectrum_run_c with the correct handle created based on input data type \ref ifx_FFT_Type_t.
 *
 * Input parameter samples_per_chirp is used to set the zero padding length in the FFT handle because
 * for input data length less than the FFT size, zero padding is required. Thus, if user wants
 * feed the input length different to this parameter, he/she needs to destroy the Range Spectrum handle
 * (using \ref ifx_range_spectrum_destroy) and create a new one!
 * Error is returned if any of the parameter is not in the defined range of these enumerations
 * (\ref ifx_FFT_Type_t and \ref ifx_FFT_Size_t).
 *
 * Threshold:
 *  - Output of range spectrum module below this threshold is clipped to 1e-6 (-120dB).
 * Scale type:
 *  - convert output to dB scale or keep linear scale.
 * chirp_bandwidth_khz:
 *  - The chirp bandwidth is the difference between min freq and max freq in the chirp is used to calculate the semantics of the
 * range axis in the output range spectrum
 *
 * @param [in]  config   Contains configuration options for range spectrum module
 * @param [out] handle   A handle to the range spectrum processing object
 *
 * @return ifx_Error_t   Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_create(const ifx_Range_Spectrum_Config_t* config,
                                      ifx_Range_Spectrum_Handle_t* handle);

/**
 * @brief Performs destruction of range spectrum handle (object) to clear internal states and memories
 * used for range spectrum calculation.
 *
 * @param [in] handle    A handle to the range spectrum processing object
 *
 * @return ifx_Error_t   Error code based on the type of error detected
 */
ifx_Error_t  ifx_range_spectrum_destroy(ifx_Range_Spectrum_Handle_t handle);

#ifdef IFX_HAVE_GENERICS
/**
 * @brief Performs signal processing on a real/complex IQ raw ADC input data (including mean removal, windowing, zero padding, FFT transform)
 * and produces a real amplitude spectrum as output.
 * Range spectrum support three modes defined by ifx_Range_Spectrum_Mode_t as;
 * 1. Single Chirp
 * 2. Coherent Integration over Chirps
 * 3. Max Energy Chirp
 *
 * @param [in] handle   A handle \ref ifx_Range_Spectrum_Handle_t to the range spectrum processing object
 * @param [in] input    Real or complex matrix, with rows are chirps and columns as samples per chirp
 * @param [out] output  Output is always a real vector of absolute magnitude spectrum of FFT size
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
#define ifx_range_spectrum_run(handle, input, output) \
    ifx_matrix_gen(input, ifx_range_spectrum_run_r, ifx_range_spectrum_run_c)(handle, input, output)
#endif
/**
 * @brief Performs signal processing on a real input I or Q (e.g. mean removal, windowing, zero padding, FFT transform)
 * and produces a real amplitude spectrum of FFT size as output. Here, second half of the spectrum is of no use as
 * it is symmetric for real input FFT.
 *
 * @param [in] handle   A handle to the range spectrum processing object
 * @param [in] input    Real input matrix, with rows as chirps and columns as samples per chirp
 * @param [out] output  Output is always a real vector of absolute magnitude spectrum of FFT size
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_run_r(ifx_Range_Spectrum_Handle_t handle,
                                     const ifx_Matrix_R_t* input,
                                     ifx_Vector_R_t* output);

/**
 * @brief Performs signal processing on a complex input IQ (e.g. mean removal, windowing, zero padding, FFT transform)
 * and produces a real amplitude spectrum of FFT size as output. Since the input is complex, thus output spectrum
 * contains information over both positive and negative half of the spectrum.
 *
 * @param [in] handle   A handle to the range spectrum processing object
 * @param [in] input    Complex input matrix, with rows as chirps and columns as samples per chirp
 * @param [out] output  Output is always a real vector of absolute magnitude spectrum of FFT size
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_run_c(ifx_Range_Spectrum_Handle_t handle,
                                     const ifx_Matrix_C_t* input,
                                     ifx_Vector_R_t* output);
/**
 * @brief Configure at runtime, the range spectrum mode in the handle. This helps
 * not to create a new handle for new mode.
 *
 * @param [in] handle   A handle to the range spectrum processing object
 * @param [in] mode     Update the range spectrum mode defined by ifx_Range_Spectrum_Mode_t
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_set_mode(ifx_Range_Spectrum_Handle_t handle,
                                        const ifx_Range_Spectrum_Mode_t mode);

/**
 * @brief Retrieves the current mode used within the range spectrum handle.
 *
 * @param [in] handle   A handle to the range spectrum processing object
 * @param [out] mode    Range spectrum mode defined by ifx_Range_Spectrum_Mode_t to be
 *                      returned.
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_get_mode(const ifx_Range_Spectrum_Handle_t handle,
                                        ifx_Range_Spectrum_Mode_t* mode);

/**
 * @brief Configures runtime index in single chirp mode index in the handle. Chirps index
 * starts from zero to positive number.
 *
 * @param [in] handle   A handle to the range spectrum processing object
 * @param [in] index    Index is valid only for single chirp mode an non-negative number
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_set_single_chirp_mode_index(ifx_Range_Spectrum_Handle_t handle,
        const uint32_t index);

/**
 * @brief Retrieves the current single chirp mode index in the handle. Chirps index
 * starts from zero to positive number.
 *
 * @param [in] handle   A handle to the range spectrum processing object
 * @param [in] index    Index is valid only for single chirp mode an non-negative number
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_get_single_chirp_mode_index(const ifx_Range_Spectrum_Handle_t handle,
        uint32_t* index);

/**
 * @brief Copies the the range spectrum matrix from range spectrum handle to the specified output container.
 * Output matrix contains;
 * 1. Only single row containing FFT transform at the selected index in RANGE_SPECTRUM_MODE_SINGLE_CHIRP
 * 2. Fully populated matrix with FFT transforms in RANGE_SPECTRUM_MODE_COHERENT_INTEGRATION
 * 3. Only single row containing FFT transform at the Maximum Energy index in RANGE_SPECTRUM_MODE_MAX_ENERGY
 *
 * @param [in] handle   A handle to the range spectrum processing object.
 * @param [out] output  Matrix of complex values containing the FFT transform (IQ).
 * @return ifx_Error_t  Error code based on the type of error detected.
 */
ifx_Error_t ifx_range_spectrum_get_fft_transformed_matrix(const ifx_Range_Spectrum_Handle_t handle,
        ifx_Matrix_C_t* output);

/**
 * @brief Configure at runtime, the range spectrum output to linear or dB scale in the handle.
 *
 * @param [in] handle      A handle to the range spectrum processing object
 * @param [in] scale_type  Linear or dB are the possible options.
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_set_output_scale_type(ifx_Range_Spectrum_Handle_t handle,
        const ifx_Scale_Type_t scale_type);

/**
 * @brief Returns current range spectrum output scale i.e. linear or dB scale used within the handle.
 *
 * @param [in] handle       A handle to the range spectrum processing object
 * @param [out] scale_type  Linear or dB are the possible options.
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_get_output_scale_type(const ifx_Range_Spectrum_Handle_t handle,
        ifx_Scale_Type_t* scale_type);

/**
 * @brief Returns information about range axis of range spectrum output specifiec by the type
 * \ref ifx_Math_Axis_Spec_t
 *
 * @param [in] handle       A handle to the range spectrum processing object
 * @param [out] range_axis_spec_m  Structure of type \ref ifx_Math_Axis_Spec_t
 *
 * @return ifx_Error_t  Error code based on the type of error detected
 */
ifx_Error_t ifx_range_spectrum_get_range_axis_info(const ifx_Range_Spectrum_Handle_t handle,
        ifx_Math_Axis_Spec_t* range_axis_spec_m);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_RANGE_SPECTRUM_H */

