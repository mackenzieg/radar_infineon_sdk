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
 * @file ifxRadar_Math.h
 *
 * @brief This file defines the API of mathematical operations such as
 *        mean calculation, scalar removal from an array, element wise
 *        addition, subtraction and multiplication of arrays, scalar
 *        multiplication of an array, absolute value calculation, maximum
 *        value in an array.
 *
 *
 */

#ifndef IFX_RADAR_MATH_H
#define IFX_RADAR_MATH_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include "ifxRadar_Types.h"
#include "ifxRadar_Complex.h"

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

#include "ifxRadar_Vector.h"

/**
 * @brief Supported Scale Types.
 *
 */
typedef enum
{
    SCALE_TYPE_LINEAR = 0U,            /**< By default scale is linear for all modules */
    SCALE_TYPE_DECIBEL_10LOG = 10U,     /**< Scale is in dB = 10xlog10() */
    SCALE_TYPE_DECIBEL_20LOG = 20U      /**< Scale is in dB = 20xlog10() */
} ifx_Scale_Type_t;

/**
 * @brief This typedef contains semantics of an axis that represents a physical quantity.
 * The axis can be along a vector (e.g., Frequency axis of a vector representing FFT values)
 * or even along one of the dimensions of a matrix (e.g., Range axis or the Speed
 * axis of a matrix representing a range-doppler map).
 * Structures of this type can be used to readily describe / visualize / plot a physical
 * quantity without additional calculations in a GUI.
 */
typedef struct
{
    /** Minimum value depicted in the axis. (can be negative)*/
    ifx_Float_t min_value;
    /** Maximum value depicted in the axis.*/
    ifx_Float_t max_value;
    /** Value bin represented by each step (sample) in the axis*/
    ifx_Float_t value_bin_per_step;

} ifx_Math_Axis_Spec_t;


/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief Computes the arithmetic mean from a real valued array for a given array length
 *
 * @param [in]  input  Pointer to a data memory defined by \ref ifx_Vector_R_t,
 *                     from which mean is calculated
 * @param [out] result Calculated arithmetic mean value for a given input data.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_get_mean_r(const ifx_Vector_R_t* input,
                                ifx_Float_t* result);

/**
 * @brief  Computes the arithmetic mean from a complex valued array for a given array length
 *
 * @param [in]  input  Pointer to a data memory defined by \ref ifx_Vector_C_t,
 *                     from which mean is calculated
 * @param [out] result Calculated arithmetic mean value for real and imaginary
 *                     parts of \ref ifx_Vector_C_t.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_get_mean_c(const ifx_Vector_C_t* input,
                                ifx_Complex_t* result);

/**
 * @brief  Removes a scalar value from each sample of a real valued array
 *
 * @param [in]  input        Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                           from which the given scalar is to be subtracted
 * @param [in]  scalar_value Floating point scalar
 * @param [out] output       Pointer to data memory containing the result of scalar subtraction.
 *                           In-place operation may be achieved by passing the same pointer for
 *                           both input and output.
 * @return ifx_Error_t       Error code
 */
ifx_Error_t ifx_math_subtract_scalar_r(const ifx_Vector_R_t* input,
                                       ifx_Float_t scalar_value,
                                       ifx_Vector_R_t* output);

/**
 * @brief  Removes a scalar value from each sample of a complex valued array
 *
 * @param [in]  input        Pointer to data memory defined by \ref ifx_Vector_C_t,
 *                           from which the given scalar is to be subtracted
 * @param [in]  scalar_value Floating point scalar
 * @param [out] output       Pointer to data memory containing the result of scalar subtraction.
 *                           In-place operation may be achieved by passing the same pointer for
 *                           both input and output.
 * @return ifx_Error_t       Error code
 */
ifx_Error_t ifx_math_subtract_scalar_c(const ifx_Vector_C_t* input,
                                       const ifx_Complex_t scalar_value,
                                       ifx_Vector_C_t* output);

/**
 * @brief  Computes absolute array from Complex input data. Absolute = sqrt(real^2 + imag^2)
 *
 * @param [in]  input  Pointer to data memory defined by \ref ifx_Vector_C_t,
 *                     on which Absolute operation is to be performed
 * @param [out] output Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                     containing the absolute values
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_vector_abs_c(const ifx_Vector_C_t* input,
                                  ifx_Vector_R_t* output);

/**
 * @brief  Computes absolute array (modulus) from Real input data. Absolute = |data|
 *
 * @param [in]  input  Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                     on which Absolute operation is to be performed
 * @param [out] output Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                     containing the absolute values
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_vector_abs_r(const ifx_Vector_R_t* input,
                                  ifx_Vector_R_t* output);

/**
 * @brief  Computes element wise addition of two real vectors and stores it
 *         in result vector. This function allows in-place calculation, that
 *         means a input vector can also be the output vector at the same time
 *         if the input vector can be overwritten.
 *
 * @param [in]  v1     Pointer to operand 1, a real vector of type
 *                     \ref ifx_Vector_R_t.
 * @param [in]  v2     Pointer to operand 2, a real vector of type
 *                     \ref ifx_Vector_R_t.
 * @param [out] result Pointer to output vector the sum is stored to. Can be
 *                     one of the input vectors v1 or v2.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_add_r(const ifx_Vector_R_t* v1,
                           const ifx_Vector_R_t* v2,
                           ifx_Vector_R_t* result);

/**
 * @brief  Computes element wise addition of two complex vectors and stores it
 *         in result vector. This function allows in-place calculation, that
 *         means a input vector can also be the output vector at the same time
 *         if the input vector can be overwritten.
 *
 * @param [in]  v1     Pointer to operand 1, a real vector of type
 *                     \ref ifx_Vector_C_t.
 * @param [in]  v2     Pointer to operand 2, a real vector of type
 *                     \ref ifx_Vector_C_t.
 * @param [out] result Pointer to output vector the sum is stored to. Can be
 *                     one of the input vectors v1 or v2.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_add_c(const ifx_Vector_C_t* v1,
                           const ifx_Vector_C_t* v2,
                           ifx_Vector_C_t* result);
/**
 * @brief  Computes element wise real vector subtraction result = v1 - v2.
 *
 * @param [in]  v1     Pointer to left hand operand \ref ifx_Vector_R_t,
 *                     on which element wise subtraction is applied to.
 * @param [in]  v2     Pointer to right hand operand \ref ifx_Vector_R_t,
 *                     which elements are subtracted from v1.
 *
 * @param [out] result Pointer to result vector \ref ifx_Vector_R_t,
 *                     containing the results of element wise subtraction.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_subtract_r(const ifx_Vector_R_t* v1,
                                const ifx_Vector_R_t* v2,
                                ifx_Vector_R_t* result);

/**
 * @brief  Computes element wise complex vector subtraction result = v1 - v2.
 *
 * @param [in]  v1     Pointer to left hand operand \ref ifx_Vector_C_t,
 *                     on which element wise subtraction is applied to.
 * @param [in]  v2     Pointer to right hand operand \ref ifx_Vector_C_t,
 *                     which elements are subtracted from v1.
 *
 * @param [out] result Pointer to result vector \ref ifx_Vector_C_t,
 *                     containing the results of element wise subtraction.
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_subtract_c(const ifx_Vector_C_t* v1,
                                const ifx_Vector_C_t* v2,
                                ifx_Vector_C_t* result);

/**
 * @brief  Computes multiplication of a given real array by a scalar
 *
 * @param [in, out]  vector Pointer to data memory defined by \ref ifx_Vector_R_t
 * @param [in] factor Value by which each element in input array gets multiplied
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_multiply_scalar_r(ifx_Vector_R_t* vector,
                                       ifx_Float_t factor);

/**
 * @brief  Computes multiplication of a given complex array by a scalar
 *
 * @param [in, out]  vector Pointer to data memory defined by \ref ifx_Vector_C_t
 * @param [in] factor Value by which each element in input array gets multiplied
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_multiply_scalar_c(ifx_Vector_C_t* vector,
                                       ifx_Float_t factor);

/**
 * @brief  Computes element wise multiplication between real input and output data
 *
 * @param [in]  input  Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                     on which element wise multiplication is to be performed
 * @param [out] output Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                     containing the results of element wise multiplication
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_multiply_r(ifx_Vector_R_t* input,
                                ifx_Vector_R_t* output);

/**
 * @brief  Computes element wise multiplication between complex input and output data
 *
 * @param [in]  input  Pointer to data memory defined by \ref ifx_Vector_C_t,
 *                     on which element wise multiplication is to be performed
 * @param [out] output Pointer to data memory defined by \ref ifx_Vector_C_t,
 *                     containing the results of element wise multiplication
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_math_multiply_c(ifx_Vector_C_t* input,
                                ifx_Vector_C_t* output);

ifx_Error_t ifx_math_multiply_rc(ifx_Vector_R_t* input,
                                 ifx_Vector_C_t* output);
/**
 * @brief  Computes the maximum value in a real array along with the corresponding index
 *
 * @param [in]  input   Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                      whose maximum value is to be computed
 * @param [out] max_idx Index corresponding to the maximum value
 * @param [out] max_val Maximum value in the given array
 * @return ifx_Error_t  Error code
 */
ifx_Error_t ifx_math_find_max(const ifx_Vector_R_t* input,
                              uint32_t* max_idx,
                              ifx_Float_t* max_val);

/**
 * @brief  Computes the L1 Norm in an array
 *
 * @param [in]  input   Pointer to data memory defined by \ref ifx_Vector_R_t,
 *                      whose L1 Norm is to be computed
 * @param [out] l1_norm Calculated L1 Norm for the input array
 * @return ifx_Error_t  Error code
 */
ifx_Error_t ifx_math_calc_l1norm(const ifx_Vector_R_t* input,
                                 ifx_Float_t* l1_norm);

/**
 *
 * @param input
 * @param sum
 * @return
 */
ifx_Error_t ifx_math_vector_sum_r(const ifx_Vector_R_t* input, ifx_Float_t* sum);

/**
 *
 * @param input
 * @param sum
 * @return
 */
ifx_Error_t ifx_math_vector_sum_c(const ifx_Vector_C_t* input, ifx_Complex_t* sum);


/**
 * @brief  Operate on \ref ifx_Vector_R_t arrays, to clip values less than given threshold.
 *
 * if input(n) < threshold
 *    output(n) = clip_value
 * else
 *    output(n) = input(n)
 *
 * @param [in]  input     Pointer to data memory defined by \ref ifx_Vector_R_t
 * @param [in]  threshold Real value, must be greater than or equal to zero
 * @param [in]  clip_value Real value, to be set if input values are greater than threshold
 * @param [out] output    Pointer to output memory, could be same as input for in-place operation
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_vector_clip_lt_threshold_r(const ifx_Vector_R_t* input,
        const ifx_Float_t threshold,
        const ifx_Float_t clip_value,
        ifx_Vector_R_t* output);

/**
 * @brief  Operate on \ref ifx_Vector_R_t arrays, to clip values greater than given threshold.
 *
 * if input(n) > threshold
 *    output(n) = clip_value
 * else
 *    output(n) = input(n)
 *
 * @param [in]  input     Pointer to data memory defined by \ref ifx_Vector_R_t
 * @param [in]  threshold Real value, must be greater than or equal to zero
 * @param [in]  clip_value Real value, to be set if input values are greater than threshold
 * @param [out] output    Pointer to output memory, could be same as input for in-place operation
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_vector_clip_gt_threshold_r(const ifx_Vector_R_t* input,
        const ifx_Float_t threshold,
        const ifx_Float_t clip_value,
        ifx_Vector_R_t* output);

/**
 * @brief  Operate on real value vectors, to perform log base 10 from standard math.
 * If any of the input real values are negative, the output would be a NaN as per the C99 standard.
 *
 * @param [in]  input     Vector of real values as an input
 * @param [out] output    Vector of real values as output, could be same as input for in-place operation
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_vector_log10_r(const ifx_Vector_R_t* input,
                                    ifx_Vector_R_t* output);

/**
 * @brief  Operate on real scalar, to convert from linear to dB scale.
 * If any of the input real values are negative, the output would be a NaN as per the C99 standard.
 *
 * @param [in]  input     Real value as input
 * @param [in]  scale     For voltage this should be 20 i.e. 20xlog10() and for power 10 i.e. 10xlog10()
 *                        However, this is a generic math function, so scale can be any desired non zero float value
 * @param [out] output    Pointer to location where a Real value is updated as output.
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_linear_to_db(const ifx_Float_t input,
                                  const ifx_Float_t scale,
                                  ifx_Float_t* output);

/**
 * @brief  Operate on real vector arrays, to convert from linear to dB scale.
 *
 * @param [in]  input     Vector of real values as an input
 * @param [in]  scale     For voltage this should be 20 i.e. 20xlog10() and for power 10 i.e. 10xlog10()
 *                        However, this is a generic math function, so scale can be any desired non zero float value
 * @param [out] output    Vector of real values as output, could be same as input for in-place operation
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_vector_linear_to_db(const ifx_Vector_R_t* input,
        const ifx_Float_t scale,
        ifx_Vector_R_t* output);

/**
 * @brief  Operate on real scalar, to convert from dB to Linear scale.
 *
 * @param [in]  input     Real value as input
 * @param [in]  scale     For voltage this should be 20 i.e. 10^(A/20) and for power 10 i.e. 10^(A/10)
 * @param [out] output    Real value as output
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_db_to_linear(const ifx_Float_t input,
                                  const ifx_Float_t scale,
                                  ifx_Float_t* output);

/**
 * @brief  Operate on real vector arrays, to convert from dB to linear scale.
 *
 * @param [in]  input     Vector of real values as an input
 * @param [in]  scale     For voltage this should be 20 i.e. 10^(A/20) and for power 10 i.e. 10^(A/10)
 * @param [out] output    Vector of real values as output, could be same as input for in-place operation
 * @return ifx_Error_t    Error code
 */
ifx_Error_t ifx_math_vector_db_to_linear(const ifx_Vector_R_t* input,
        const ifx_Float_t scale,
        ifx_Vector_R_t* output);

ifx_Error_t ifx_math_abs_r(ifx_Float_t input, ifx_Float_t* output);
ifx_Error_t ifx_math_abs_c(ifx_Complex_t input, ifx_Float_t* output);

#ifdef IFX_HAVE_GENERICS
#define ifx_math_mean(input, result) \
    ifx_vector_gen(input, ifx_math_get_mean_r, ifx_math_get_mean_c)(input, result)

#define ifx_math_substract_scalar(input, scalar_value, output) \
    ifx_vector_gen(input, ifx_math_subtract_scalar_r, ifx_math_subtract_scalar_c)(input, scalar_value, output)

#define ifx_math_vector_abs(input, output) \
    ifx_vector_gen(input,ifx_math_vector_abs_r, ifx_math_vector_abs_c)(input, output)

#define ifx_math_log10(input, output) \
    ifx_vector_gen(input, ifx_math_log10_r, ifx_math_log10_c)(input, output)

#define ifx_math_add(v1, v2, result) \
    ifx_vector_gen(v1, ifx_math_add_r, ifx_math_add_c)(v1, v2, result)

#define ifx_math_subtract(lh, rh) \
    ifx_vector_gen(lh, ifx_math_subtract_r, ifx_math_subtract_c)(input, output)

#define ifx_math_multiply_scalar(vector, factor) \
    ifx_vector_gen(vector, ifx_math_multiply_scalar_r, ifx_math_multiply_scalar_c)(vector, factor)

#define ifx_math_multiply(input, output) \
    ifx_vector_gen2(input, output, ifx_math_multiply_r, ifx_math_multiply_rc, NULL, ifx_math_multiply_c)(input, output)

#define ifx_math_vector_sum(input, sum) \
    ifx_vector_gen(input, ifx_math_vector_sum_r, ifx_math_vector_sum_c)(input, output)

#define ifx_math_abs(input, output) \
    ifx_real_cmplx_gen(input, ifx_math_abs_r, ifx_math_abs_c)(input, output)
#endif /* IFX_HAVE_GENERICS */

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_MATH_H */
