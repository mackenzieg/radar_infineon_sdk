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
 * @file ifxRadar_Mem.h
 *
 * @brief This file defines the API for memory allocation and deallocation
 *        as well as aligned allocation and aligned deallocation.
 *
 *
 */
#ifndef IFX_RADAR_MEM_H
#define IFX_RADAR_MEM_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**
 * @brief Allocates memory of defined size.
 * @param size Number of bytes to be allocated.
 * @return Pointer to the allocated memory if successful
 *         otherwise it returns NULL.
 */
void* ifx_mem_alloc(size_t size);

/**
 * @brief Allocates memory for an array of defined number of elements
 *        with specified element size and initializes each byte to zero.
 * @param count Number of elements to be allocated.
 * @param element_size Size of one element of the array.
 * @return Pointer to the allocated memory if successful
 *         otherwise it returns NULL.
 */
void* ifx_mem_calloc(size_t count, size_t element_size);

/**
 * @brief Allocates memory of defined size with specified alignment.
 *        The size must be a multiple of alignment.
 * @param size Number of bytes to be allocated.
 * @param alignment The number the allocated memory is aligned to.
 * @return Pointer to the allocated memory if successful
 *         otherwise it returns NULL.
 */
void* ifx_mem_aligned_alloc(size_t size, size_t alignment);

/**
 * @brief Deallocates the memory which has been allocated by \ref ifx_mem_alloc
 *        or \ref ifx_mem_calloc. Do not use this to deallocate memory allocated
 *        with \ref ifx_mem_aligned_alloc.
 *
 * @param mem Pointer to the memory to be deallocated.
 */
void ifx_mem_free(void* mem);

/**
 * @brief Deallocates the memory which has been allocated by \ref ifx_mem_aligned_alloc.
 *        Do not use this to deallocate memory allocated allocated with \ref ifx_mem_alloc
 *        or \ref ifx_mem_calloc.
 *
 * @param mem Pointer to the memory to be deallocated.
 */
void ifx_mem_aligned_free(void* mem);

/**
 * @brief  This function returns the number of the total allocated
 *         memory in bytes. Internal tracking must be enabled with TRACK_MEMORY_ALLOCATION
 *         defined to enable this function.
 * @return Total number of bytes allocated. If tracking is not enabled
 *         this function returns -1.
 */
int ifx_mem_get_total_alloc_size();

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_MEM_H */
