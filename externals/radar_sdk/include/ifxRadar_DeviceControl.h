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
 * @file ifxRadar_DeviceControl.h
 *
 * @brief This file defines the API for simple radar device control operations.
 */


#ifndef IFX_RADAR_DEVICE_CONTROL_H
#define IFX_RADAR_DEVICE_CONTROL_H
/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include <stdint.h>
#include "ifxRadar_DeviceConfig.h"
#include "ifxRadar_Frame.h"

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
 * @brief This type represents a radar device.
 *
 * When the connection to a radar device is established @ref ifx_device_create creates a handle
 * of this type, which must be passed to all future device related function calls.
 */
typedef struct ifx_Device_Controller_s* ifx_Device_Handle_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief This function connects to a radar sensor device and starts acquisition of time domain data.
 *
 * This function searches for a BGT60TR13AIP sensor device connected to the host machine and
 * connects to the first found sensor device. After connection the board is configured according to
 * the parameters provided through *device_config* and acquisition of time domain data is started.
 *
 * A handle to the sensor device is returned through *handle*. That handle must be passed to all
 * following device related function calls to identify the device.
 *
 * @param[in]  device_config  This struct contains the parameters for data acquisition. The device
 *                            in configured according to these parameters.
 * @param[out] handle         A handle to the connected sensor device is returned through this
 *                            parameter.
 *
 * @return If this function succeeds IFX_OK is returned and a handle to the connected device is
 *         returned. If the function fails one of the following error codes is returned:
 *         - @ref IFX_ERROR_NO_DEVICE
 *         - @ref IFX_ERROR_DEVICE_BUSY
 *         - @ref IFX_ERROR_COMMUNICATION_ERROR
 *         - @ref IFX_ERROR_NUM_SAMPLES_OUT_OF_RANGE
 *         - @ref IFX_ERROR_ANTENNA_COMBINATION_NOT_ALLOWED
 *         - @ref IFX_ERROR_IF_GAIN_OUT_OF_RANGE
 *         - @ref IFX_ERROR_SAMPLERATE_OUT_OF_RANGE
 *         - @ref IFX_ERROR_RF_OUT_OF_RANGE
 *         - @ref IFX_ERROR_TX_POWER_OUT_OF_RANGE
 *         - @ref IFX_ERROR_CHIRP_RATE_OUT_OF_RANGE
 *         - @ref IFX_ERROR_FRAME_RATE_OUT_OF_RANGE
 *         - @ref IFX_ERROR_NUM_CHIRPS_NOT_ALLOWED
 *         - @ref IFX_ERROR_FRAME_SIZE_NOT_SUPPORTED
 */
ifx_Error_t ifx_device_create(ifx_Device_Config_t* device_config, ifx_Device_Handle_t* handle);

/**
 * @brief Use to intialize structures used for frame creation without real Device i.e. without commlib commands.
 *
 * @param[in]  device_config  This struct contains the parameters for data acquisition. The device
 *                            in configured according to these parameters.
 * @param[out] handle         A handle to the connected sensor device is returned through this
 *                            parameter.
 *
 * @return This function always returns IFX_OK.
 */
ifx_Error_t ifx_device_create_dummy(ifx_Device_Config_t* device_config, ifx_Device_Handle_t* handle);

/**
 * @brief This function closes the connection to the radar sensor device.
 *
 * This function stops acquisition of time domain data, closes the connection to the device and
 * destroys the handle. The handle becomes invalid and must not be used any more after this
 * function was called.
 *
 * @param[in]  handle  The handle to the device that should be closed.
 *
 * @return This function always returns IFX_OK.
 */
ifx_Error_t ifx_device_destroy(ifx_Device_Handle_t handle);

/**
 * @brief This function free the memory occupied by device handle.

 * @param[in]  handle  The handle to the device that should be closed.
 *
 * @return This function always returns IFX_OK.
 */
ifx_Error_t ifx_device_destroy_dummy(ifx_Device_Handle_t handle);

/**
 * @brief This function creates a frame structure for time domain data acquisition using information
 * from a \ref ifx_Device_Handle_t
 *
 * This function checks the current configuration of the specified sensor device and initializes a
 * data structure (of type \ref ifx_Frame_t) that can hold a time domain data frame according
 * acquired through that device.
 *
 * @param[in]  handle  A handle to the device that is used to acquire the time domain data frames.
 *                     The device must be initialized before calling this function.
 * @param[in]  frame   The frame structure to be initialized. The caller must create this structure,
 *                     this function will initialize it and allocate internal data for data storage.
 *
 * @return If this function succeeds IFX_OK is returned and the *frame* is initialized properly. If
 *         not enough memory is available, IFX_ERROR_MEMORY_ALLOCATION_FAILED is returned.
 */
ifx_Error_t ifx_device_create_frame_from_device_handle(ifx_Device_Handle_t handle, ifx_Frame_t* frame);

/**
 * @brief This function retrieves the next frame of time domain data from a radar device.
 *
 * This function retrieves the next complete frame of time domain data from the connected device.
 * The samples from all chirps and all enabled RX antennas will be copied to the provided data
 * structure *frame*. It is assumed that *frame* was allocated beforehand with
 * \ref ifx_device_create_frame.
 *
 * @param[in]  handle  A handle to the device to retrieve the time domain data from.
 * @param[out] frame   The frame structure where the time domain data should be copied to.
 *
 * @return If this function succeeds and time domain data is available, IFX_OK is returned and the
 *         data is copied to *frame*. If no data is available the @ref IFX_ERROR_TIMEOUT is
 *         returned. If data frames are acquired faster than they are retrieved by this function
 *         an internal buffer overflow on the sensor device will occur. In that case
 *         @ref IFX_ERROR_FIFO_OVERFLOW is returned.
 */
ifx_Error_t ifx_device_get_next_frame(ifx_Device_Handle_t handle, ifx_Frame_t* frame);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_DEVICE_CONTROL_H */

