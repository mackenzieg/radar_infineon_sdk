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
 * @file ifxRadar_Vector.h
 *
 * @brief This file configures operations such as creation and destruction of
 *        vectors, print vector samples onto a file
 *
 */


#ifndef IFX_RADAR_VECTOR_H
#define IFX_RADAR_VECTOR_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include <stdio.h>
#include "ifxRadar_Types.h"
#include "ifxRadar_Complex.h"
#include "ifxRadar_Error.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
==============================================================================
   2. MACROS
==============================================================================
*/
#ifdef IFX_HAVE_GENERICS
#define ifx_vector_gen(v, funcr, funcc)  ifx_type_gen(v, ifx_Vector_R_t*,funcr, ifx_Vector_C_t*, funcc)
#define ifx_vector_gen2(v1, v2, func_rr, func_rc, func_cr, func_cc)\
    ifx_type_gen2(v1, v2, ifx_Vector_R_t*, ifx_Vector_C_t*,  func_rr, func_rc, func_cr, func_cc)
#endif /* IFX_HAVE_GENERICS */
/*
==============================================================================
   3. TYPES
==============================================================================
*/

/**
 *
 * @brief Defines the structure for one dimensional Real data array used within API.
 *        Data length is fixed in this vector i.e. vector neither grows nor shrinks.
 *
 */
typedef struct
{
    uint32_t     length; /**< Number of floating point elements in the array */
    ifx_Float_t* data;   /**< Pointer to floating point memory containing data values */

} ifx_Vector_R_t;

/**
 *
 * @brief Defines the structure for one dimensional Complex data array used within API.
 *        Data length is fixed in this vector i.e. vector neither grows nor shrinks.
 *
 */
typedef struct
{
    uint32_t       length; /**< Number of complex elements in the array */
    ifx_Complex_t* data;   /**< Pointer to floating point memory containing data values */

} ifx_Vector_C_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/


/**
 * @brief Allocates memory for a real data array for a specified number of
 *        elements and initializes it to zero
 *
 * @param[in,out] vector  Pointer to the memory containing array defined by \ref ifx_Vector_R_t
 * @param[in]     length  Number of elements in the array
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_vector_create_r(uint32_t length, ifx_Vector_R_t* vector);

/**
 * @brief Allocates memory for a complex data array for a specified number of
 *          elements and initializes it to zero
 *
 * @param[in,out] vector  Pointer to the memory containing array defined by \ref ifx_Vector_C_t
 * @param[in]     length  Number of elements in the array
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_vector_create_c(uint32_t length, ifx_Vector_C_t* vector);

/**
 * @brief Frees the memory allocated for a real data array for a specified number of
 *        elements
 *
 * @param[in,out] vector  Pointer to the memory containing array defined by \ref ifx_Vector_R_t
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_vector_destroy_r(ifx_Vector_R_t* vector);

/**
 * @brief Frees the memory allocated for a complex data array for a specified number of
 *          elements
 *
 * @param[in,out] vector  Pointer to the memory containing array defined by \ref ifx_Vector_C_t
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_vector_destroy_c(ifx_Vector_C_t* vector);

/**
 * @brief Prints the samples of real data array onto a file
 *
 * @param[in,out] f       Pointer to the file that stores the samples of the array
 * @param[in]     vector  Pointer to the memory containing array defined by \ref ifx_Vector_R_t
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_vector_print_r(FILE* f, ifx_Vector_R_t* vector);

/**
 * @brief Prints the samples of complex data array onto a file
 *
 * @param[in,out] f       Pointer to the file that stores the samples of the array
 * @param[in]     vector  Pointer to the memory containing array defined by \ref ifx_Vector_C_t
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_vector_print_c(FILE* f, ifx_Vector_C_t* vector);

/**
 *@brief Copies a real vector content into another real vector within the given boundaries.
 *
 * @param from          Pointer to the real vector to copy from.
 * @param index         The start index the copy begins at.
 * @param num_elements  Number of elements to copy.
 * @param to            Pointer to the real vector to copy to.
 * @return ifx_Error_t  Error Code
 */
ifx_Error_t ifx_vector_copy_r(const ifx_Vector_R_t* from, uint32_t index, uint32_t num_elements, ifx_Vector_R_t* to);

/**
 *@brief Copies a complex vector content into another complex vector within the given boundaries.
 *
 * @param [in] from          Pointer to the complex vector to copy from.
 * @param [in] index         The start index the copy begins at.
 * @param [in] num_elements  Number of elements to copy.
 * @param [out] to           Pointer to the complex vector to copy to.
 * @return ifx_Error_t  Error Code
 */
ifx_Error_t ifx_vector_copy_c(const ifx_Vector_C_t* from, uint32_t index, uint32_t num_elements, ifx_Vector_C_t* to);

/**
 * @brief Performs rotation on a real vector elements by factor and elements shifted out are fed back to the
 * vector like a circular ring.
 * Input and output memories should be of same type and size, else error code is returned.
 *
 * @param [in] input   Real array on which shift operation needs to be performed
 * @param [in] factor  Rotation factor (+ive for rotate clockwise, -ive not supported yet)
 *                     elements shifted by this factor in a circular ring manner
 * @param [out] output Rotated vector by a given rotation factor
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_vector_rotate_r(const ifx_Vector_R_t* input, uint32_t factor, ifx_Vector_R_t* output);

/**
 * @brief Performs rotation on a complex vector elements by factor and elements shifted out are fed back to the
 * vector like a circular ring.
 * Input and output memories should be of same type and size, else error code is returned.
 *
 * @param [in] input   Complex array on which shift operation needs to be performed
 * @param [in] factor  Rotation factor (+ive for rotate clockwise, -ive not supported yet),
 *                     elements shifted by this factor in a circular ring manner
 * @param [out] output Rotated vector by a given rotation factor
 * @return ifx_Error_t Error code
 */
ifx_Error_t ifx_vector_rotate_c(const ifx_Vector_C_t* input, uint32_t factor, ifx_Vector_C_t* output);

ifx_Error_t ifx_vector_set_element_r(ifx_Vector_R_t* input, uint32_t index, ifx_Float_t value);
ifx_Error_t ifx_vector_set_element_c(ifx_Vector_C_t* input, uint32_t index, ifx_Complex_t value);

#ifdef IFX_HAVE_GENERICS
#define ifx_vector_create(l,v)      ifx_vector_gen(v, ifx_vector_create_r, ifx_vector_create_c)(l,v)
#define ifx_vector_destroy(v)       ifx_vector_gen(v, ifx_vector_destroy_r, ifx_vector_destroy_c)(v)
#define ifx_vector_print(v)         ifx_vector_gen(v, ifx_vector_print_r, ifx_vector_print_c)(v)
#define ifx_vector_rotate(i,f,o)    ifx_vector_gen(i, ifx_vector_rotate_r, ifx_vector_rotate_c)(i,f,o)
#define ifx_vector_copy(from, index, num_elements, to)\
    ifx_vector_gen(from, ifx_vector_copy_r, ifx_vector_copy_c)(from, index, num_elements, to)
#define ifx_vector_set_element(v, i, val)\
    ifx_vector_gen(v, ifx_vector_set_element_r, ifx_vector_set_element_c)(v, i, val)
#endif /* IFX_HAVE_GENERICS */

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif  // IFX_RADAR_VECTOR_H
