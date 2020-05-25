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
 * @file ifxRadar_Matrix.h
 *
 * @brief This file defines the API to create and destroy matrices
 *
 */


#ifndef IFX_RADAR_MATRIX_H
#define IFX_RADAR_MATRIX_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/
#include <stdint.h>
#include "ifxRadar_Complex.h"
#include "ifxRadar_Vector.h"
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

/**
 *
 * @brief Defines the structure for one dimensional floating point data array used within API.
 *        Data length is fixed in this matrix i.e. matrix neither grows nor shrinks.
 *        The data is arranged sequentially in a row-major order, i.e., all elements of a given row are
 *        placed in successive memory locations, as depicted in the illustrations.
 *
 * @image html Matrix_memory_map_wiki.png "Illustration of row-major order" width=400cm
 *
 * @image html Matrix_memory_map_wiki_C.png "Illustration of accessing a matrix in row-major order in C (starting from index 0)" width=400cm
 *
 * The above 2 images have been sourced from <a href="https://en.wikipedia.org/wiki/Row-_and_column-major_order">here</a>.
 *
 * @image html Matrix_memory_map.png "Illustration showing memory arrangement of matrix data in ifx_Matrix_R_t " width=700cm
 *
 */
typedef struct
{
    uint32_t rows;       /**< Number of rows in the matrix */
    uint32_t columns;       /**< Number of columns in the matrix */
    ifx_Float_t* data;  /**< Pointer to floating point memory containing data values */
} ifx_Matrix_R_t;

/**
 *
 * @brief Defines the structure for one dimensional Complex data array used within API.
 *        Data length is fixed in this matrix i.e. matrix neither grows nor shrinks.
 *        The data is arranged sequentially in a row-major order, i.e., all elements of a given row are
 *        placed in successive memory locations, as depicted in the illustrations.
 *
 * @image html Matrix_memory_map_wiki.png "Illustration of row-major order" width=400cm
 *
 * @image html Matrix_memory_map_wiki_C.png "Illustration of accessing a matrix in row-major order in C (starting from index 0)" width=400cm
 *
 * The above 2 images have been sourced from <a href="https://en.wikipedia.org/wiki/Row-_and_column-major_order">here</a>.
 *
 * @image html Matrix_memory_map.png "Illustration showing memory arrangement of matrix data in ifx_Matrix_C_t " width=700cm
 *
 */
typedef struct
{
    uint32_t rows;           /**< Number of rows in the matrix */
    uint32_t columns;           /**< Number of columns in the matrix */
    ifx_Complex_t* data;    /**< Pointer to floating point memory containing data values */
} ifx_Matrix_C_t;


/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

#ifdef IFX_HAVE_GENERICS
#define ifx_matrix_gen(matrix, funcr, funcc) \
    ifx_type_gen(matrix, ifx_Matrix_R_t*, funcr, ifx_Matrix_C_t*, funcc)

#define ifx_matrix_create(rows, columns, matrix) \
    ifx_matrix_gen(matrix, ifx_matrix_create_r, ifx_matrix_create_c)(rows, columns, matrix)

#define ifx_matrix_destroy(matrix) \
    ifx_matrix_gen(matrix, ifx_matrix_destroy_r, ifx_matrix_destroy_c)(matrix)

#define ifx_matrix_copy(from, to, from_row, num_rows, from_column, num_columns) \
    ifx_matrix_gen(from, ifx_matrix_copy_r, ifx_matrix_copy_c)(from, to, from_row, num_rows, from_column, num_columns)

#define ifx_matrix_clone(from, to) \
    ifx_matrix_gen(from, ifx_matrix_clone_r, ifx_matrix_clone_c)(from, to)

#define ifx_matrix_set_element(matrix, row, column, val) \
    ifx_matrix_gen(matrix, ifx_matrix_set_element_r, ifx_matrix_set_element_c)(matrix, row, column, val)

#define ifx_matrix_get_element(matrix, row, column, value) \
    ifx_matrix_gen(matrix, ifx_matrix_get_element_r, ifx_matrix_get_element_c)(matrix, row, column, value)

#define ifx_matrix_set_row(matrix, row_index, row_values, count) \
    ifx_matrix_gen(matrix, ifx_matrix_set_row_r, ifx_matrix_set_row_c)(matrix, row_index, row_values, count)

#define ifx_matrix_set_row_vector(matrix, row_index, row_values) \
    ifx_matrix_gen(matrix, ifx_matrix_set_row_vector_r, ifx_matrix_set_row_vector_c)(matrix, row_index, row_values)

#define ifx_matrix_get_row_vector(matrix, row_index, row_values) \
    ifx_matrix_gen(matrix, ifx_matrix_get_row_vector_r, ifx_matrix_get_row_vector_c)(matrix, row_index, row_values)

#define ifx_matrix_transpose(matrix, transposed) \
    ifx_matrix_gen(matrix, ifx_matrix_transpose_r, ifx_matrix_transpose_c)(matrix, transposed)
#endif

/**
 * @brief Allocates memory for a real matrix with a specified number of
 *        rows and columns and initializes it to zero.
 *        The data is arranged sequentially row-wise, i.e., all elements of a given row are
 *        placed in successive memory locations, as depicted in the illustration.
 *
 * @image html Matrix_memory_map.png "Illustration showing memory arrangement of matrix data in ifx_Matrix_R_t " width=700cm
 *
 * see \ref ifx_Matrix_R_t for more details.
 *
 *
 * @param [in]      rows          Number of rows in the matrix
 * @param [in]      columns       Number of columns in the matrix
 * @param [in,out]  matrix        Pointer to an allocated and populated, but empty
 *                                matrix instance defined by \ref ifx_Matrix_R_t
 * @return          ifx_Error_t   Error code
 */
ifx_Error_t ifx_matrix_create_r(const uint32_t rows,
                                const uint32_t columns,
                                ifx_Matrix_R_t* matrix);

/**
 * @brief Allocates memory for a complex matrix with a specified number of
 *        rows and columns and initializes it to zero
 *        The data is arranged sequentially row-wise, i.e., all elements of a given row are
 *        placed in successive memory locations, as depicted in the illustration.
 *
 * @image html Matrix_memory_map.png "Illustration showing memory arrangement of matrix data in ifx_Matrix_C_t " width=700cm
 *
 * see \ref ifx_Matrix_C_t for more details.
 *
 * @param [in]     rows           Number of rows in the matrix
 * @param [in]     columns        Number of columns in the matrix
 * @param [in,out] matrix         Pointer to an allocated and populated, but empty
 *                                matrix instance defined by \ref ifx_Matrix_C_t
 * @return         ifx_Error_t    Error code
 */
ifx_Error_t ifx_matrix_create_c(const uint32_t rows,
                                const uint32_t columns,
                                ifx_Matrix_C_t* matrix);

/**
 * @brief Frees memory for a real matrix defined by \ref ifx_matrix_create_r
 *        and sets the length of the matrix to zero
 *
 * @param [in,out] matrix    Pointer to an allocated matrix instance defined
 *                           by \ref ifx_Matrix_R_t
 * @return ifx_Error_t       Error code
 */
ifx_Error_t ifx_matrix_destroy_r(ifx_Matrix_R_t* matrix);

/**
 * @brief Frees memory for a complex matrix defined by \ref ifx_matrix_create_c
 *        and sets the length of the matrix to zero
 *
 * @param [in,out] matrix    Pointer to an allocated matrix instance defined
 *                           by \ref ifx_Matrix_C_t
 * @return ifx_Error_t       Error code
 */
ifx_Error_t ifx_matrix_destroy_c(ifx_Matrix_C_t* matrix);

ifx_Error_t ifx_matrix_copy_r(const ifx_Matrix_R_t* from, ifx_Matrix_R_t* to,
                              uint32_t from_row, uint32_t num_rows,
                              uint32_t from_column, uint32_t num_columns);

ifx_Error_t ifx_matrix_copy_c(const ifx_Matrix_C_t* from, ifx_Matrix_C_t* to,
                              uint32_t from_row, uint32_t num_rows,
                              uint32_t from_column, uint32_t num_columns);
/**
 * @brief Copies the elements from a real source matrix to a real destination matrix
 * The condition is that both source and destination matrix instance should be allocated
 * and populated (pointers inside the matrix instance should not be null) using the
 * \ref ifx_matrix_create_r . No memory allocation is done in this function.
 *
 * @param [in]      from     Pointer to an allocated and populated source
 *                           matrix instance defined by \ref ifx_Matrix_R_t
 * @param [in,out]  to       Pointer to an allocated and populated, yet empty
 *                           (data values of the matrix are still zero) destination
 *                           matrix instance defined by \ref ifx_Matrix_R_t
 * @return ifx_Error_t       Error code
 */
ifx_Error_t ifx_matrix_clone_r(const ifx_Matrix_R_t* from, ifx_Matrix_R_t* to);

/**
 * @brief Copies the elements from a complex source matrix to a complex destination matrix
 * The condition is that both source and destination matrix instance should be allocated
 * and populated (pointers inside the matrix instance should not be null) using the
 * \ref ifx_matrix_create_c . No memory allocation is done in this function.
 *
 * @param [in]       from   Pointer to an allocated and populated source
 *                          matrix instance defined by \ref ifx_Matrix_C_t
 * @param [in,out]   to     Pointer to an allocated and populated, yet empty
 *                          (data values of the matrix are still zero) destination
 *                          matrix instance defined by \ref ifx_Matrix_C_t
 * @return ifx_Error_t      Error code
 */
ifx_Error_t ifx_matrix_clone_c(const ifx_Matrix_C_t* from, ifx_Matrix_C_t* to);

/**
 * @brief Sets a user defined value at a given row,column location in a real matrix
 *
 * @param [in,out] matrix  Pointer to an allocated and populated real valued
 *                         matrix instance defined by \ref ifx_Matrix_R_t
 * @param [in]     row     Row location where the value is to be set
 * @param [in]     column  Column location where the value is to be set
 * @param [in]     value   User defined value defined by \ref ifx_Float_t
 * @return ifx_Error_t     Error code
 */
ifx_Error_t ifx_matrix_set_element_r(ifx_Matrix_R_t* matrix, uint32_t row, uint32_t column, ifx_Float_t value);

/**
 * @brief Sets a user defined value at a given row,column location in a complex matrix
 *
 * @param [in,out] matrix  Pointer to an allocated and populated complex valued
 *                         matrix instance defined by \ref ifx_Matrix_C_t
 * @param [in]     row     Row location where the value is to be set
 * @param [in]     column  Column location where the value is to be set
 * @param [in]     value   User defined value defined by \ref ifx_Complex_t
 * @return ifx_Error_t     Error code
 */
ifx_Error_t ifx_matrix_set_element_c(ifx_Matrix_C_t* matrix, uint32_t row, uint32_t column, ifx_Complex_t value);

/**
 * @brief Obtains a user defined value set at a given row,column location in a real matrix
 *
 * @param [in]  matrix  Pointer to an allocated and populated real valued
 *                      matrix instance defined by \ref ifx_Matrix_R_t
 * @param [in]  row     Row location where the value can be obtained
 * @param [in]  column  Column location where the value can be obtained
 * @param [out] value   Pointer to a user defined value defined by \ref ifx_Float_t
 * @return ifx_Error_t  Error code
 */
ifx_Error_t ifx_matrix_get_element_r(const ifx_Matrix_R_t* matrix, uint32_t row, uint32_t column, ifx_Float_t* value);

/**
 * @brief Obtains a user defined value set at a given row,column location in a complex matrix
 *
 * @param [in]  matrix  Pointer to an allocated and populated complex valued
 *                      matrix instance defined by \ref ifx_Matrix_C_t
 * @param [in]  row     Row location where the value can be obtained
 * @param [in]  column  Column location where the value can be obtained
 * @param [out] value   Pointer to a user defined value defined by \ref ifx_Complex_t
 * @return ifx_Error_t  Error code
 */
ifx_Error_t ifx_matrix_get_element_c(const ifx_Matrix_C_t* matrix, uint32_t row, uint32_t column, ifx_Complex_t* value);

/**
 * @brief Allocates a user defined sequence of real values to a user defined row index in a real matrix
 * The count of the input real values should not be greater than the number of columns in the matrix.
 *
 * @param [in,out] matrix        Pointer to an allocated and populated real valued
 *                               matrix instance defined by \ref ifx_Matrix_R_t
 * @param [in]     row_index     Row number that is to be filled by the user defined vector
 * @param [in]     row_values    Pointer to user defined vector defined by \ref ifx_Float_t
 * @param [in]     count         Number of elements in the user defined vector
 * @return ifx_Error_t           Error Code
 */
ifx_Error_t ifx_matrix_set_row_r(ifx_Matrix_R_t* matrix, uint32_t row_index, const ifx_Float_t* row_values, uint32_t count);

/**
 * @brief Allocates a user defined sequence of complex numbers values to a user defined row index in a complex matrix
 * The count of the input complex numbers should not be greater than the number of columns in the matrix.
 *
 * @param [in,out] matrix        Pointer to an allocated and populated complex valued
 *                               matrix instance defined by \ref ifx_Matrix_C_t
 * @param [in]     row_index     Row number that is to be filled by the user defined vector
 * @param [in]     row_values    Pointer to User defined vector defined by \ref ifx_Complex_t
 * @param [in]     count         Number of elements in the user defined vector
 * @return ifx_Error_t           Error Code
 */
ifx_Error_t ifx_matrix_set_row_c(ifx_Matrix_C_t* matrix, uint32_t row_index, const ifx_Complex_t* row_values, uint32_t count);

/**
 * @brief Allocates a user defined vector of real values (ifx_Vector_R_t) to a user defined row index in a real matrix
 * The length of the input real vector should not be greater than the number of columns in the matrix.
 *
 * @param [in,out] matrix        Pointer to an allocated and populated real valued
 *                               matrix instance defined by \ref ifx_Matrix_R_t
 * @param [in]     row_index     Row number that is to be filled by the user defined vector
 * @param [in]     row_values    Pointer to user defined vector defined by \ref ifx_Vector_R_t
 * @return ifx_Error_t           Error Code
 */
ifx_Error_t ifx_matrix_set_row_vector_r(ifx_Matrix_R_t* matrix, uint32_t row_index, const ifx_Vector_R_t* row_values);

/**
 * @brief Allocates a user defined complex vector (ifx_Vector_C_t) to a user defined row index in a complex matrix
 * The length of the input complex vector should not be greater than the number of columns in the matrix.
 *
 * @param [in,out] matrix        Pointer to an allocated and populated complex valued
 *                               matrix instance defined by \ref ifx_Matrix_C_t
 * @param [in]     row_index     Row number that is to be filled by the user defined vector
 * @param [in]     row_values    Pointer to user defined vector defined by \ref ifx_Vector_C_t
 * @return ifx_Error_t           Error Code
 */
ifx_Error_t ifx_matrix_set_row_vector_c(ifx_Matrix_C_t* matrix, uint32_t row_index, const ifx_Vector_C_t* row_values);

/**
 * @brief This function returns a complex vector pointing to defined row of the given complex matrix.
 *
 * @param [in]  matrix       The complex matrix, from which one row would be pointed to by the output vector.
 * @param [in]  row_index    The row of the matrix the vector will point to.
 * @param [out] row_values   The complex vector which data pointer is pointing to the given row in the given matrix.
 *
 * @return ifx_Error_t Error Code
 */
ifx_Error_t ifx_matrix_get_row_vector_c(const ifx_Matrix_C_t* matrix, uint32_t row_index, ifx_Vector_C_t* row_values);

/**
 * @brief This function returns a real vector pointing to defined row of the given real matrix.
 *
 * @param [in]  matrix       The real matrix, from which one row would be pointed to by the output vector.
 * @param [in]  row_index    The row of the matrix the vector will point to.
 * @param [out] row_values   The real vector which data pointer is pointing to the given row in the given matrix.
 *
 * @return ifx_Error_t Error Code
 */
ifx_Error_t ifx_matrix_get_row_vector_r(const ifx_Matrix_R_t* matrix, uint32_t row_index, ifx_Vector_R_t* row_values);

/**
 * @brief Transposes a given real matrix and saves it in the given output matrix. This function doesn't support in-place
 *        transposing therefore it is not allowed input and output matrix pointing to the same matrix.
 *        This will result in IFX_ERROR_IN_PLACE_CALCULATION_NOT_SUPPORTED.
 *
 * @param [in] matrix       Real matrix to transpose.
 * @param [out] transposed  Real matrix the transposed matrix will be stored in must be allocated and of the same size as
 *                     the given input matrix.
 *
 * @return ifx_Error_t Error Code
 */
ifx_Error_t ifx_matrix_transpose_r(const ifx_Matrix_R_t* matrix, ifx_Matrix_R_t* transposed);

/**
 * @brief Transposes a given complex matrix and saves it in the given output matrix. This function doesn't support
 *        in-place transposing therefore it is not allowed input and output matrix pointing to the same matrix.
 *        This will result in IFX_ERROR_IN_PLACE_CALCULATION_NOT_SUPPORTED.
 *
 * @param [in]  matrix       Complex matrix to transpose.
 * @param [out] transposed   Complex matrix the transposed matrix will be stored in must be allocated and of the same size as
 *                     the given input matrix.
 *
 * @return ifx_Error_t Error Code
 */
ifx_Error_t ifx_matrix_transpose_c(const ifx_Matrix_C_t* matrix, ifx_Matrix_C_t* transposed);

/**
 * @brief Adds two real matrices and saves the result of addition into given output matrix.
 *
 * This function supports in-place addition operation, therefore it is allowed one of the input and
 * output matrix pointing to the same matrix.
 *
 * Math operation implemented by this method: Z = X + Y
 *
 * @param [in]  matrix_l     Real matrix as left argument (X in above equation).
 * @param [in]  matrix_r     Real matrix as right argument (Y in above equation).
 *
 * @param [out] result       Real matrix where the result of addition will be stored (Z in above equation).
 *                           It must be allocated and of the same dimensions as the given input matrix. Can be in-place.
 *
 * @return ifx_Error_t       Error if dimensions do not match or NULL is passed as matrix pointer.
 *
 */
ifx_Error_t ifx_matrix_add_r(const ifx_Matrix_R_t* matrix_l,
                             const ifx_Matrix_R_t* matrix_r,
                             ifx_Matrix_R_t* result);

/**
 * @brief Adds two complex matrices and saves the result of addition into given output matrix.
 *
 * This function supports in-place addition operation, therefore it is allowed one of the input and
 * output matrix pointing to the same matrix.
 *
 * Math operation implemented by this method: Z = X + Y
 *
 * @param [in]  matrix_l     Complex matrix as left argument (X in above equation).
 * @param [in]  matrix_r     Complex matrix as right argument (Y in above equation).
 *
 * @param [out] result       Complex matrix where the result of addition will be stored (Z in above equation).
 *                           It must be allocated and of the same dimensions as the given input matrix. Can be in-place.
 *
 * @return ifx_Error_t       Error if dimensions do not match or NULL is passed as matrix pointer.
 *
 */
ifx_Error_t ifx_matrix_add_c(const ifx_Matrix_C_t* matrix_l,
                             const ifx_Matrix_C_t* matrix_r,
                             ifx_Matrix_C_t* result);

/**
 * @brief Subtracts two real matrices and saves the result of subtraction into given output matrix.
 *
 * This function supports in-place subtraction operation, therefore it is allowed one of the input and
 * output matrix pointing to the same matrix.
 *
 * Math operation implemented by this method: Z = X - Y
 *
 * @param [in]  matrix_l     Real matrix as left argument (X in above equation).
 * @param [in]  matrix_r     Real matrix as right argument (Y in above equation).
 *
 * @param [out] result       Real matrix where the result of subtraction will be stored (Z in above equation).
 *                           It must be allocated and of the same dimensions as the given input matrix. Can be in-place.
 *
 * @return ifx_Error_t       Error if dimensions do not match or NULL is passed as matrix pointer.
 *
 */
ifx_Error_t ifx_matrix_subtract_r(const ifx_Matrix_R_t* matrix_l,
                                  const ifx_Matrix_R_t* matrix_r,
                                  ifx_Matrix_R_t* result);

/**
 * @brief Subtracts two complex matrices and saves the result of subtraction into given output matrix.
 *
 * This function supports in-place subtraction operation, therefore it is allowed one of the input and
 * output matrix pointing to the same matrix.
 *
 * Math operation implemented by this method: Z = X - Y
 *
 * @param [in]  matrix_l     Complex matrix as left argument (X in above equation).
 * @param [in]  matrix_r     Complex matrix as right argument (Y in above equation).
 *
 * @param [out] result       Complex matrix where the result of subtraction will be stored (Z in above equation).
 *                           It must be allocated and of the same dimensions as the given input matrix. Can be in-place.
 *
 * @return ifx_Error_t       Error if dimensions do not match or NULL is passed as matrix pointer.
 *
 */
ifx_Error_t ifx_matrix_subtract_c(const ifx_Matrix_C_t* matrix_l,
                                  const ifx_Matrix_C_t* matrix_r,
                                  ifx_Matrix_C_t* result);

/**
 * @brief Apply linear real value scaling to a real matrix.
 *
 * This function supports in-place scaling operation, therefore it is allowed one of the input and
 * output matrix pointing to the same matrix.
 *
 * Math operation implemented by this method: Z = a * X
 *
 * @param [in]  scale        Real floating point vlaues of scalar (a in above equation).
 * @param [in]  input        Real matrix on which scaling is to be applied (X in above equation).
 *
 * @param [out] output       Real matrix where the result of scaling will be stored (Z in above equation).
 *                           It must be allocated and of the same dimensions as the given input matrix. Can be in-place.
 *
 * @return ifx_Error_t       Error if dimensions do not match or NULL is passed as matrix pointer.
 *
 */
ifx_Error_t ifx_matrix_multply_scalar_r(const ifx_Float_t scale,
                                        const ifx_Matrix_R_t* input,
                                        ifx_Matrix_R_t* output);

/**
 * @brief Apply linear real value scaling to a complex matrix.
 *
 * This function supports in-place scaling operation, therefore it is allowed one of the input and
 * output matrix pointing to the same matrix.
 *
 * Math operation implemented by this method: Z = a * X
 *
 * @param [in]  scale        Real floating point vlaues of scalar (a in above equation).
 * @param [in]  input        Complex matrix on which scaling is to be applied (X in above equation).
 *
 * @param [out] output       Complex matrix where the result of scaling will be stored (Z in above equation).
 *                           It must be allocated and of the same dimensions as the given input matrix. Can be in-place.
 *
 * @return ifx_Error_t       Error if dimensions do not match or NULL is passed as matrix pointer.
 *
 */
ifx_Error_t ifx_matrix_multply_scalar_c(const ifx_Float_t scale,
                                        const ifx_Matrix_C_t* input,
                                        ifx_Matrix_C_t* output);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif  // IFX_RADAR_MATRIX_H
