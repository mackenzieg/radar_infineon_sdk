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
 * @file ifxRadar_FFT.h
 *
 * @brief This file defines the API to calculate fft signal and spectrum.
 *
 *
 */

#ifndef IFX_RADAR_FFT_H
#define IFX_RADAR_FFT_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

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
 * @brief Supported FFT sizes.
 *
 */
typedef enum
{
    FFT_SIZE_16     = 16U,
    FFT_SIZE_32     = 32U,
    FFT_SIZE_64     = 64U,
    FFT_SIZE_128    = 128U,
    FFT_SIZE_256    = 256U,
    FFT_SIZE_512    = 512U,
    FFT_SIZE_1024   = 1024U

} ifx_FFT_Size_t;

/**
 * @brief Supported FFT Types.
 *
 */
typedef enum
{
    FFT_TYPE_R2C = 1U,           /**< Input is real and FFT output is complex */
    FFT_TYPE_C2C = 2U            /**< Input is complex and FFT output is complex */

} ifx_FFT_Type_t;


/**
 *
 * @brief Forward declaration structure for FFT processing module.
 *
 */
typedef struct ifx_FFT_s* ifx_FFT_Handle_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief Aim of this method is to create a FFT handle(object), based on the input parameters
 * e.g. FFT type (\ref ifx_FFT_Type_t) and FFT size (\ref ifx_FFT_Size_t). This handle contains
 * the complete twiddle table required for computing the FFT. During this handle construction,
 * two cases have been taken into the consideration, regarding the input of the FFT as output is
 * always complex.
 * 1. Real input FFT
 * 2. Complex input FFT
 * In case 1, a special handle has been used to make FFT operation twice faster and memory efficient.
 * Input parameter fft_input_len is used to set the zero padding length in the FFT handle because
 * for input data length less than the FFT size, zero padding is required. Thus, if user wants
 * feed the input of different length than this parameter, he/she needs to destroy the FFT handle and
 * create a new one.
 * Error is returned if any of the parameter is not in the defined range of these enums
 * (\ref ifx_FFT_Type_t and \ref ifx_FFT_Size_t)
 *
 * @param [in] fft_type FFT type is defined by \ref ifx_FFT_Type_t. Type is defined based on FFT input nature,
 * i.e. real or complex.
 * @param [in] fft_input_len Size of the input (time domain) data samples, in units of sample count. Should be
 * non-zero and not greater than FFT size
 * @param [in] fft_size The FFT parameters are defined by structure @ref ifx_FFT_Size_t. Must be power of 2.
 * @param [in] handle   An ifx_FFT_Handle_t handle, containing the required memories and
 *                      settings internally used by FFT library.
 * @return ifx_Error_t Error code based on the type of error detected.
 */
ifx_Error_t ifx_fft_create(const ifx_FFT_Type_t fft_type,
                           const uint32_t fft_input_len,
                           const ifx_FFT_Size_t fft_size,
                           ifx_FFT_Handle_t* handle);

/**
 * @brief Performs destruction of FFT handle (object) to clear internal states used for a particular FFT size.
 *
 * @param [in] handle  An ifx_FFT_Handle_t handle, containing the internal state memories to be released.
 *
 * @return ifx_Error_t Error code
 */
ifx_Error_t  ifx_fft_destroy(ifx_FFT_Handle_t handle);

/**
 * @brief Performs FFT transform on a real/complex input (I/Q) and produces a complex output IQ FFT signal.
 *
 * This method is used for both real input as well as complex input FFT. It internally calculates R2C or
 * C2C FFT transform based on the FFT handle passed as a first argument. User has
 * to create memory for the output (at least equal to the configured FFT size in the handle).
 * If input length is not matched as it is in the handle (set during the FFT handle creation),
 * this method returns error. If input length is less than FFT size, this method performs zeros padding
 * before calculating FFT transform.
 * There is no type safety for the input parameter, so user has to ensure it is passing the right type
 * of input i.e. \ref ifx_Vector_R_t or \ref ifx_Vector_C_t.
 *
 *
 * @param [in] handle  The FFT object to be used, for R2C or C2C FFT transform
 * @param [in] input   \ref ifx_Vector_R_t or \ref ifx_Vector_C_t pointer passed as
 * input vector.
 * @param [out] output FFT output is always in complex IQ format i.e. \ref ifx_Vector_C_t
 * @return ifx_Error_t Error code
 */

/**
 * @brief Performs FFT transform on a real input (I or Q) and produces a complex
 *        output IQ FFT signal
 *
 * @param [in] handle  The FFT object to be used
 * @param [in] input   Pointer to a floating point data memory containing real values
 * @param [out] output FFT output in complex IQ format
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_run_r(ifx_FFT_Handle_t handle,
                          const ifx_Vector_R_t* input,
                          ifx_Vector_C_t* output);

/**
 * @brief Performs FFT transform on a complex input IQ and produces a complex
 *        output IQ FFT signal
 *
 * @param [in] handle  The FFT object to be used
 * @param [in] input   Real valued data array on which FFT needs to be performed
 * @param [out] output FFT output in complex IQ format
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_run_c(ifx_FFT_Handle_t handle,
                          const ifx_Vector_C_t* input,
                          ifx_Vector_C_t* output);

/**
 * @brief Performs shift on a FFT amplitude spectrum (real values) to bring DC bin in the center of spectrum,
 * positive bins on right side and negative bins on left side.
 *
 *Input and output memories should be of same type and size
 * @param [in] input   Real array on which shift operation needs to be performed
 * @param [out] output DC bin shifted to the center of the output array
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_shift_r(const ifx_Vector_R_t* input,
                            ifx_Vector_R_t* output);

/**
 * @brief Performs shift on a FFT spectrum complex to bring DC bin in the center of spectrum,
 * positive bins on right side and negative bins on left side.
 *
 *Input and output memories should be of same type and size
 * @param [in] input   Complex array on which shift operation needs to be performed
 * @param [out] output DC bin shifted to the center of the output array
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_shift_c(const ifx_Vector_C_t* input,
                            ifx_Vector_C_t* output);

/**
 * @brief Returns the current FFT size configured in the FFT handle
 *
 * @param handle     The FFT object to be used
 * @param fft_size   Returns FFT size given in enum \ref ifx_FFT_Size_t
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_get_fft_size(const ifx_FFT_Handle_t handle,
                                 ifx_FFT_Size_t* fft_size);

/**
 * @brief Returns the current FFT type (R2C or C2C) configured in the FFT handle
 *
 * @param handle     The FFT object to be used
 * @param fft_type   Returns FFT type given in enum \ref ifx_FFT_Type_t
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_get_fft_type(const ifx_FFT_Handle_t handle,
                                 ifx_FFT_Type_t* fft_type);

/**
 * @brief Returns the zero padding length currently configured in the FFT handle based on
 * fft_input_length
 *
 * @param handle     The FFT object to be used
 * @param zero_pad_length   Zero or positive number of samples to be filled in the FFT input
 * before performing FFT.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_fft_get_zero_pad_length(const ifx_FFT_Handle_t handle,
                                        uint32_t* zero_pad_length);

#ifdef IFX_HAVE_GENERICS
#define ifx_fft_shift(i,o)    ifx_vector_gen(i, ifx_fft_shift_r, ifx_fft_shift_c)(i,o)

#define ifx_fft_run(handle, input, output) \
    ifx_vector_gen(input, ifx_fft_run_r, ifx_fft_run_c)(handle, input, output)
#endif /* IFX_HAVE_GENERICS */

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_FFT_H */

