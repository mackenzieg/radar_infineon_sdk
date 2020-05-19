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
 * @file ifxRadar_Error.h
 *
 * @brief This file defines error codes and error handling macros.
 *
 *
 */


#ifndef IFX_RADAR_ERROR_H
#define IFX_RADAR_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum ifx_Error
{
    IFX_OK = 0,
    IFX_ERROR = 0x00010000,
    IFX_ERROR_ARGUMENT_NULL,
    IFX_ERROR_ARGUMENT_INVALID,
    IFX_ERROR_ARGUMENT_OUT_OF_BOUNDS,
    IFX_ERROR_ARGUMENT_INVALID_EXPECTED_REAL,
    IFX_ERROR_ARGUMENT_INVALID_EXPECTED_COMPLEX,
    IFX_ERROR_DIMENSION_WRONG,
    IFX_ERROR_DIMENSION_MISMATCH,
    IFX_ERROR_MEMORY_ALLOCATION_FAILED,
    IFX_ERROR_IN_PLACE_CALCULATION_NOT_SUPPORTED,

    // Device related errors:
    IFX_ERROR_NO_DEVICE,                        /**< Device handle creation failed because no
                                                     device compatible to Radar SDK was found. */
    IFX_ERROR_DEVICE_BUSY,                      /**< The connected device is busy and cannot
                                                     perform the requested action. This can happen
                                                     during device handle creation when the device
                                                     is in an undefined state. It is recommended to
                                                     unplug and replug the device. */
    IFX_ERROR_COMMUNICATION_ERROR,              /**< The communication between host computer and
                                                     device is disturbed. This error is also
                                                     returned when the device sends an unexpected
                                                     error code. */
    IFX_ERROR_NUM_SAMPLES_OUT_OF_RANGE,         /**< The device does not support the requested
                                                     number of samples, because the requested
                                                     number is too high. */
    IFX_ERROR_ANTENNA_COMBINATION_NOT_ALLOWED,  /**< The device does not support the requested
                                                     combination of RX antennas to be enabled.
                                                     This error typically occurs when a
                                                     non-existing antenna is requested to be
                                                     enabled. */
    IFX_ERROR_IF_GAIN_OUT_OF_RANGE,             /**< The device does not support the requested IF
                                                     gain, because the requested gain is either too
                                                     high or too low. */
    IFX_ERROR_SAMPLERATE_OUT_OF_RANGE,          /**< The device does not support the requested
                                                     sampling rate, because the requested rate is
                                                     either too high or too low. */
    IFX_ERROR_RF_OUT_OF_RANGE,                  /**< The requested FMCW start and end frequency are
                                                     not in the supported RF range of the device. */
    IFX_ERROR_TX_POWER_OUT_OF_RANGE,            /**< The device does not support the requested TX
                                                     power, because the requested value is
                                                     too high. */
    IFX_ERROR_CHIRP_RATE_OUT_OF_RANGE,          /**< The requested chirp-to-chirp time cannot be
                                                     applied. This typically happens when the
                                                     requested time is shorter than the chirp
                                                     duration resulting from the specified sampling
                                                     rate and number of samples. */
    IFX_ERROR_FRAME_RATE_OUT_OF_RANGE,          /**< The requested frame period cannot be applied.
                                                     This typically happens when the requested
                                                     period is shorter than the frame duration
                                                     resulting from the specified sampling
                                                     rate, number of samples and chirp-to-chirp
                                                     time. */
    IFX_ERROR_NUM_CHIRPS_NOT_ALLOWED,           /**< The device does not support the requested
                                                     number of chirps per frame, because the
                                                     number is too high. */
    IFX_ERROR_FRAME_SIZE_NOT_SUPPORTED,         /**< The device does not support the frame size
                                                     resulting from specified number of chirps,
                                                     number of samples and number of antennas. */
    IFX_ERROR_TIMEOUT,                          /**< The device did not acquire a complete time
                                                     domain data frame within the expected time.
                                                     */
    IFX_ERROR_FIFO_OVERFLOW,                     /**< The device stopped acquisition of time domain
                                                     data due to an internal buffer overflow. This
                                                     happens when time domain data is acquired
                                                     faster than it is read from the device. */

    //app related errors:
    IFX_ERROR_APP = 0x00020000                  /**< App errors are to be defined individually by the apps, starting
                                                     from this value. */
} ifx_Error_t;

#ifndef IFX_STDOUT
#define IFX_STDOUT stderr
#endif

#ifndef IFX_ERR_UNUSED
#define IFX_ERR_UNUSED(e) (void)(e)
#endif

#ifdef IFX_HAVE_LOGGING
#ifndef IFX_ERR_PRINT
#include <stdio.h>
#define IFX_ERR_PRINT(...)  fprintf(IFX_STDOUT, __VA_ARGS__)
#endif
#else
#define IFX_ERR_PRINT(...)
#endif

#ifndef IFX_ON_ERROR_HOOK
#define IFX_ON_ERROR_HOOK(e)   IFX_ERR_PRINT("\nError Code 0x%x (%d) in %s (%d)\n", e, e, __FILE__, __LINE__)
#endif

#define IFX_ERR_IS_ERROR(e) (e != IFX_OK)

#define IFX_ERR_CHECK(e) \
    do \
    { \
        if(IFX_ERR_IS_ERROR(e)) \
        { \
            {\
                IFX_ON_ERROR_HOOK(e); \
            } \
        } \
    } while(0)


#define IFX_ERR_CHECK_RETURN(e) \
    do \
    { \
        if(IFX_ERR_IS_ERROR(e)) \
        { \
            { \
                IFX_ON_ERROR_HOOK(e); \
            } \
            return e; \
        } \
    } while(0)

#define IFX_ERR_CHECK_BREAK(e)  \
    if(1) \
    { \
        if(IFX_ERR_IS_ERROR(e)) \
        { \
            { \
                IFX_ON_ERROR_HOOK(e); \
            } \
            break; \
        } \
    } else IFX_ERR_UNUSED(0)


#ifndef IFX_ERR_CHECK_RUNTIME
#define IFX_ERR_CHECK_RUNTIME(e) IFX_ERR_CHECK(e)

#define IFX_ERR_CHECK_CALL_RETURN(e, handle, destroy) \
    do \
    { \
        if(IFX_ERR_IS_ERROR(e)) \
        { \
            { \
                IFX_ON_ERROR_HOOK(e); \
            } \
            destroy(handle); \
            return e; \
        } \
    } while(0)
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_ERROR_H */
