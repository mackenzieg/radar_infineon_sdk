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
 * @file ifxRadar_DeviceConfig.h
 *
 * @brief This file defines the radar device config.
 */


#ifndef IFX_RADAR_DEVICE_CONFIG_H
#define IFX_RADAR_DEVICE_CONFIG_H
/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
==============================================================================
   2. DEFINITIONS
==============================================================================
*/

#define DEVICE_RX_ANTENNA1      1 /**< This can be used to enable RX antenna 1 in the rx_antenna_mask field of ifx_DeviceConfig_t. */
#define DEVICE_RX_ANTENNA2      2 /**< This can be used to enable RX antenna 2 in the rx_antenna_mask field of ifx_DeviceConfig_t. */
#define DEVICE_RX_ANTENNA3      4 /**< This can be used to enable RX antenna 3 in the rx_antenna_mask field of ifx_DeviceConfig_t. */

/*
==============================================================================
   3. TYPES
==============================================================================
*/

/**
 * @brief This structure contains all relevant parameters for acquisition of time domain data.
 *
 * When a connection to sensor device is established, the device is configured according to the
 * parameters of this struct.
 */
typedef struct
{
    uint32_t num_samples_per_chirp;      /**< This is the number of samples acquired during each
                                              chirp of a frame. The duration of a single
                                              chirp depends on the number of samples and the
                                              sampling rate. */
    uint32_t num_chirps_per_frame;       /**< This is the number of chirps a single data frame
                                              consists of. */
    uint32_t adc_samplerate_hz;          /**< This is the sampling rate of the ADC used to acquire
                                              the samples during a chirp. The duration of a single
                                              chirp depends on the number of samples and the
                                              sampling rate. */
    uint64_t frame_period_us;            /**< This is the time period that elapses between the
                                              beginnings of two consecutive frames. The reciprocal
                                              of this parameter is the frame rate. */
    uint32_t lower_frequency_kHz;        /**< This is the start frequency of the FMCW frequency
                                              ramp. */
    uint32_t upper_frequency_kHz;        /**< This is the end frequency of the FMCW frequency
                                              ramp. */
    uint8_t bgt_tx_power;                /**< This value controls the power of the transmitted RX
                                              signal. This is an abstract value betwwen 0 and 31
                                              without any physical meaning. Refer to BGT60TR13AIP
                                              data sheet do learn more about the TX power
                                              BGT60TR13AIP is capable of. */
    uint8_t rx_antenna_mask;             /**< In this mask each bit represents one RX antenna of
                                              BGT60TR13AIP. If a bit is set the according RX
                                              antenna is enabled during the chirps and the signal
                                              received through that antenna is captured. */
    uint64_t chirp_to_chirp_time_100ps;  /**< This is the time period that elapses between the
                                              beginnings of two consecutive chirps in a frame. */
    int8_t if_gain_dB;                   /**< This is the amplification factor that is applied to
                                              the IF signal coming from the RF mixer before it is
                                              fed into the ADC. */
    uint64_t frame_end_delay_100ps;      /**< This parameter defines the delay after each frame in
                                              100 picosecond steps. In order to set this value
                                              frame_period_us must be set to 0, otherwise this value
                                              will be ignored. */
    uint64_t shape_end_delay_100ps;      /**< This parameter defines the delay after each shape in
                                              100 picosecond steps. In order to set this value
                                              chirp_to_chirp_time_100ps must be set to 0, otherwise
                                              this value will be ignored. */

} ifx_Device_Config_t;

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_DEVICE_CONFIG_H */

