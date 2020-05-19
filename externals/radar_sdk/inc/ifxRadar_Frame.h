
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
 * @file ifxRadar_Frame.h
 *
 * @brief This file defines the radar frame type.
 */


#ifndef IFX_RADAR_FRAME_H
#define IFX_RADAR_FRAME_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/
#include <stdint.h>
#include "ifxRadar_Matrix.h"

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
 * @brief This structure holds a complete frame of time domain data.
 *
 * When time domain data is acquired by a radar sensor device, it is copied into an instance of
 * this structure. The structure contains one matrix for each enabled RX antenna.
 */
typedef struct
{
    uint8_t num_rx;           /**< The number of rx matrices in this instance (same as the number
                                   of RX antennas enabled in the radar device) */
    ifx_Matrix_R_t* rx_data;  /**< This is an array of data matrices. It contains num_rx elements. */
} ifx_Frame_t;

/**
 * @brief This function creates a frame structure for time domain data acquisition
 *
 * This function initializes a data structure (of type \ref ifx_Frame_t) that can hold a time domain
 * data frame according to the dimensions provided as parameters. These parameters can be obtained for
 * instance by parsing through a pre-recorded file. If a device is connected then the
 * \ref ifx_device_create_frame_from_device_handle can be used instead of this function, as that function
 * reads the dimensions from configured the device handle.
 *
 * @param[in]  num_antennas             Number of rx antennas available in the device
 * @param[in]  num_chirps_per_frame     Number of chirps configured in a frame
 * @param[in]  num_samples_per_chirp    Number of chirps configured in a frame
 * @param[in]  frame                    The frame structure to be initialized. The caller must create this structure,
 *                                      this function will initialize it and allocate internal data for data storage.
 *
 *
 * @return If this function succeeds IFX_OK is returned and the *frame* is initialized properly. If
 *         not enough memory is available, IFX_ERROR_MEMORY_ALLOCATION_FAILED is returned.
 */
ifx_Error_t ifx_device_create_frame(uint8_t num_antennas,
                                    uint32_t num_chirps_per_frame,
                                    uint32_t num_samples_per_chirp,
                                    ifx_Frame_t* frame);


/**
 * @brief This function frees the memory allocated for the frame structure.
 *
 * This function frees the memory associated with the provided frame structure and uninitializes
 * it. This function must be called when the frame is not used any longer to avoid memory leaks. *frame*
 * must not be used any more after this function was called.
 *
 * @param[in]  frame  The frame structure to be deallocated.
 *
 * @return This function is supposed to return IFX_OK.
 */
ifx_Error_t ifx_device_destroy_frame(ifx_Frame_t* frame);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
