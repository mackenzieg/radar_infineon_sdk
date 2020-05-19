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
 * @file presence_sensing.h
 *
 * @brief This file defines the API to perform presence sensing use case.
 *
 *
 */

#ifndef IFX_RADAR_PRESENCESENSING_H
#define IFX_RADAR_PRESENCESENSING_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/

#include "ifxRadarSDK.h"
/*
==============================================================================
   2. DEFINITIONS
==============================================================================
*/

#define IFX_MIN_RANGE_RESOLUTION_M          (0.025f)    
#define IFX_MAX_RANGE_RESOLUTION_M          (1.00f)                                                          
                                                              
#define IFX_MIN_MAXIMUM_RANGE_M             (0.20f)
#define IFX_MAX_MAXIMUM_RANGE_M             (20.0f)      
                                                              
#define IFX_MIN_SPEED_RESOLUTION_M_S        (0.025f)
#define IFX_MAX_SPEED_RESOLUTION_M_S        (0.833f)      
                                                               
#define IFX_MIN_MAXIMUM_SPEED_M_S           (0.25f) 
#define IFX_MAX_MAXIMUM_SPEED_M_S           (25.0f)     /**< This impacts chirp to chirp time parameter of chip.
                                                         Valid range [0.25 - 25]m/s */
                                                               
#define IFX_MIN_FRAME_RATE_HZ               (0.016f)
#define IFX_MAX_FRAME_RATE_HZ               (100f)        /**< This is the frame acquisition rate for raw data.
                                                         valid range is [0.016 - 100]Hz */

#define IFX_MIN_BGT_TX_POWER                (1U)
#define IFX_MAX_BGT_TX_POWER                (31U)        /**< TX Power level to be used on the radar transceiver.
                                                         Valid range is [1 - 31] */
                                                         
#define IFX_MIN_RX_ANTENNA_NUMBER           (1) 
#define IFX_MAX_RX_ANTENNA_NUMBER           (3)         /**< Presence sensing uses only single Rx antenna. The ID number of 
                                                         the RX antenna e.g. 1 => Rx1; 2 => Rx2; 3 => Rx3 */
                                                         
#define IFX_MIN_IF_GAIN_DB                  (18)
#define IFX_MAX_IF_GAIN_DB                  (60)        /**< This is the amplification factor that is
                                                         applied to the IF signal before sampling.
                                                         Valid range is [18 - 60]dB */

#define IFX_MIN_MTI_WEIGHT                  (0.0f)
#define IFX_MAX_MTI_WEIGHT                  (1.0f)          

#define IFX_MIN_RANGE_HYST_PERCENT          (0U)
#define IFX_MAX_RANGE_HYST_PERCENT          (100U)

#define IFX_MIN_ABSENCE_CONF_COUNT          (1U)
#define IFX_MAX_ABSENCE_CONF_COUNT          (10U)

#define IFX_MIN_PRESENCE_CONF_COUNT         (1U)
#define IFX_MAX_PRESENCE_CONF_COUNT         (10U)

/*
==============================================================================
   3. TYPES
==============================================================================
*/
/**
 *
 * @brief Forward declaration structure for presence sensing
 *
 */

typedef enum
{
    PRESENCE = 0U,
    ABSENCE  = 1U
} Presence_State_t;

typedef void (*state_status_cb_t)(Presence_State_t cur_state);
typedef void (*state_change_cb_t)(Presence_State_t new_state);

/**
 * @brief This structure contains all relevant parameters for creating an instance of the presence
 * sensing module \ref ifx_PresenceSensing_Handle_t, and some parameters which determine the
 * operation of the presense sensing module.
 */
typedef struct
{
    ifx_FFT_Size_t range_fft_size;                       /**< The size of the range FFTs to be performed
                                                         on the input frame data for presense sensing*/
    ifx_FFT_Size_t doppler_fft_size;                     /**< The size of the FFTs to be performed on doppler domain
                                                         of the range spectrums obtained during presense sensing*/
    ifx_Window_Type_t range_fft_window_type;             /**< The type of window to be used for range FFT*/
    ifx_Float_t range_fft_window_alpha;                  /**< The scaling of the window to be used for range FFT*/
    ifx_Window_Type_t doppler_fft_window_type;           /**< The type of window to be used for doppler FFT*/
    ifx_Float_t doppler_fft_window_alpha;                /**< The scaling of the window to be used for doppler FFT*/
    ifx_Float_t mti_weight;                              /**< The weight if the \ref ifx_MTI_Handle_t used to remove static
                                                         targets during presence sensing*/
    ifx_Float_t minimum_detection_range_m;               /**< Targets below this distance are not detected by the presence sensing
                                                         algo*/
    ifx_Float_t maximum_detection_range_m;               /**< Targets above this distance are not detected by the presence sensing
                                                         algo */
    uint32_t range_hysteresis_percentage;                /**< Hysteresis for \ref maximum_detection_range_m while changing state from
                                                         presence to absence and vice versa.*/
    uint32_t presence_confirm_count;                     /**< Number of consecutive presence detections in consecutive frames
                                                         for making a definitive presence detection.*/
    uint32_t absence_confirm_count;                      /**< Number of consecutive absence detections in consecutive frames
                                                         for making a definitive presence detection.*/
    ifx_Device_Config_t device_config;                   /**< Configuration of the radar device to be used for
                                                         presense sensing*/                   
    state_status_cb_t state_status_cb;                   /**< Pointer to a callback function that would be called once
                                                         everytime the presence sensing state machine runs*/                   
    state_change_cb_t state_change_cb;                   /**< Pointer to a callback function that would be called once
                                                         everytime a state change happens in the presence sensing state machine*/ 
    ifx_Range_Spectrum_Mode_t range_spectrum_mode;       /**< Mode of calculation of range spectrum.
                                                         (see \ref ifx_Range_Spectrum_Mode_t)*/
    ifx_Float_t threshold_factor_presence_peak;          /**< Decides threshold factor param in \ref ifx_Peak_Search_Config_t for configuring the
                                                         "presence_peak" peak search module that is active in the PRESENCE state*/
    ifx_Float_t threshold_factor_absence_peak;           /**< Decides threshold factor param in \ref ifx_Peak_Search_Config_t for configuring the
                                                         "absence_peak" peak search module that is active in the ABSENCE state [internal param] */
    ifx_Float_t threshold_factor_absence_fine_peak;      /**< Decides threshold factor param in \ref ifx_Peak_Search_Config_t for configuring the
                                                         "absence_fine_peak" peak search module that is active in the ABSENCE state and decides the
                                                         range bins of the range spectrums on which the doppler FFT needs to be performed.*/
} ifx_PresenceSensing_Config_t;

/**
 * @brief This structure holds the metrics of the feature space used for presence detection.
 *
 * The presence detection algorithm analyzes the distance (range) and velocity (speed) of targets
 * in the field of view, so the time domain input data must be transformed to range/speed feature
 * space. Resolution and maximum values in this feature space depend on the time domain acquisition
 * parameters and the presence sensing algorithm calculates them from the device configuration. To
 * go the other way round and specify the metrics the function
 * @ref ifx_presencesensing_tranlate_metrics_to_config can be used to derive those acquisition parameters
 * from desired feature space metrics. This structure is used to specify the desired metrics to
 * that function.
 */
typedef struct
{
    float range_resolution_m;   /**< The range resolution is the distance between two consecutive
                                     bins of the range transform. Note that even though zero
                                     padding before the range transform seems to increase this
                                     resolution, the true resolution does not change but depends
                                     only from the acquisition parameters. Zero padding is just
                                     interpolation! */
    float maximum_range_m;      /**< The bins of the range transform represent the range
                                     between 0m and this value. (If the time domain input data it
                                     is the range-maximum_range_m ... maximum_range_m.) */
    float speed_resolution_m_s; /**< The speed resolution is the distance between two consecutive
                                     bins of the Doppler transform. Note that even though zero
                                     padding before the speed transform seems to increase this
                                     resolution, the true resolution does not change but depends
                                     only from the acquisition parameters. Zero padding is just
                                     interpolation! */
    float maximum_speed_m_s;    /**< The bins of the Doppler transform represent the speed values
                                     between -maximum_speed_m_s and maximum_speed_m_s. */
} ifx_PresenceSensing_Metrics_t;

typedef struct ifx_PresenceSensing_Handle ifx_PresenceSensing_Handle_t;

/**
 * This structure is used the return intermediate results of the presence sensing algorithm.
 * The allocation and deallocation of this structure is up to the user by calling \ref ifx_presencesensing_create_result
 * and \ref ifx_presencesensing_destroy_result.
 */
typedef struct {
    Presence_State_t cur_presence_state; /**< Current presence state */
    ifx_Matrix_R_t range_doppler_map;    /**< Range Doppler Map, the number of valid rows of this matrix
                                              is defined by the num_targets element of this structure.
                                              Presence sensing module does not calculate a
                                              full range doppler map, therefore num_targets must be
                                              consiedered when reading this matrix.*/
    ifx_Matrix_C_t range_spectrogram;    /**< Range Spectrogram*/
    uint32_t num_targets;                /**<*Number of targets found in current run.*/
} ifx_PresenceSensing_Result_t;
/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/
/**
 * Allocates memory for the internal structures and handles of the presence sensing module.
 * This function must be called after initializing a \ref ifx_PresenceSensing_Config_t
 * structure with the required config values. This function needs to be called once with a specific 
 * configuration before calling the run \ref ifx_presencesensing_run function. If the 
 * configuration of the presence sensing module needs to be changed, then the
 * currently active \ref ifx_PresenceSensing_Handle_t must be destroyed with a 
 * \ref ifx_presencesensing_destroy, and a new \ref ifx_PresenceSensing_Handle_t must be 
 * created by calling this function with the updated \ref ifx_PresenceSensing_Config_t.
 * 
 * @param config A pointer to an initialized presence sensing config structure.
 * @param handle Poiner to a handle of a presence sensing structure 
 * @return error code.
 */
ifx_Error_t ifx_presencesensing_create(const ifx_PresenceSensing_Config_t* config,
                                       ifx_PresenceSensing_Handle_t** handle);

/**
 * Frees allocated memories and destroys the internal handles created for the 
 * given presence sensing handle
 * 
 * @param handle Poiner to a handle of a presence sensing structure 
 * @return error code.
 */
ifx_Error_t ifx_presencesensing_destroy(ifx_PresenceSensing_Handle_t* handle);

/**
 * Performs a presense sensing operation on the given matrix consisting of 
 * raw IF frame data coming from the configured radar transceiver.
 * The algorithm oprates based on configration params used while creating the
 * \ref ifx_PresenceSensing_Handle_t being passed here, so this should be called only
 * after calling the \ref ifx_presencesensing_create once.
 * 
 * @param frame_data Matrix consisting of IF frame data from the radar transceiver. The number of rows
 * of the input matrix should match the number of chirps per frame and the number of columns
 * should match the number of samples per chirp expected by the
 * \ref ifx_PresenceSensing_Handle_t (given in the \ref ifx_Device_Config_t of the 
 * \ref ifx_PresenceSensing_Config_t used while creating the given presence sensing handle)
 * @param handle Poiner to a handle of a presence sensing structure 
 * @return error code.
 */
ifx_Error_t ifx_presencesensing_run(const ifx_Matrix_R_t* frame_data,
                                    ifx_PresenceSensing_Handle_t* handle);

/**
 * Allocates memory for the result structure of the presence sensing module.
 * This function must be called after the creation of the presence sensing handle
 * and must be feed with the allocated and defined handle. It is up to the user to
 * destroy the allocated result instances. If called twice without destroying the
 * result instance the functions leads to memory leaks.
 * @param handle The presence sensing handle must be allocated and defined.
 * @param result Conatins important intermediate results see \ref ifx_PresenceSensing_Result_t.
 * @return error code.
 */
ifx_Error_t ifx_presencesensing_create_result(ifx_PresenceSensing_Handle_t* handle,
                                              ifx_PresenceSensing_Result_t* result);

/**
 * Copies the intermediate results of the previous run into the result structure.
 * @param handle Handle to the presence sensing object.
 * @param result Pointer to \ref ifx_PresenceSensing_Result_t structure the result
 * will be stored in.
 * @return Error Code
 */
ifx_Error_t ifx_presencesensing_get_result(ifx_PresenceSensing_Handle_t* handle,
                                           ifx_PresenceSensing_Result_t* result);

/**
 * Frees allocated memory of the given result structure.
 * @param result Pointer to the \ref ifx_PresenceSensing_Result_t structure to free.
 * @return Error Code
 */
ifx_Error_t ifx_presencesensing_destroy_result(ifx_PresenceSensing_Result_t* result);

/**
 * @brief This function derives a device configuration from some given parameters which are read
 * from a config file, like the redar feature space metrics, adc sample rate, FMCW frame rate, etc.
 * @internal
 * 
 * This function needs to be moved to device_config.h and possibly defined in a new
 * device_config.c as this functionality may be needed across apps
 * 
 * @endinternal
 *
 * @param[in]  metrics              The desired radar feature space metrics to be used.
 * @param[in]  frame_rate           The desired FMCW frame rate 
 * @param[in]  adc_samplerate_hz    The desired sample rate for the chirps.
 * @param[in]  bgt_tx_power         The desired transmitter power level.
 * @param[in]  rx_antenna_number    The desired RX antenna to be used for presence sensing.
 * @param[in]  if_gain_dB           The desired the desired gain on the radar IF signal.
 * @param[out] device_config        The struct where the parameters calculated from the metrics are
 *                                  written to.
 */
ifx_Error_t ifx_presencesensing_get_device_config(const ifx_PresenceSensing_Metrics_t* metrics,
                                                  ifx_Float_t frame_rate,
                                                  uint32_t adc_samplerate_hz,
                                                  uint8_t bgt_tx_power,
                                                  uint8_t  rx_antenna_number,
                                                  int8_t if_gain_dB,
                                                  ifx_Device_Config_t* device_config);    

/**
 * @brief This function derives a device configuration from specified feature space metrics.
 *
 * This functions calculates FMCW frequency range, number of samples per chirp, number of chirps
 * per frame and chirp-to-chirp time needed to achieve the specified feature space metrics. Number
 * of samples per chirp and number of chirps per frame are rounded up to the next power of two,
 * because this is a usual constraint for range and Doppler transform. The resulting maximum range
 * and maximum speed may therefore be larger than specified.
 *
 * The calculated values are written to the members of the *device_config* struct, which must be
 * provided by the caller. Note that only those fields mentioned above are modified. Other fields
 * of the device_config are not related to the feature space metrics, so they stay unmodified. The
 * caller must take care for proper initialization of those fields before using the device_config
 * struct.
 * 
 * @internal
 * 
 * This function needs to be moved to device_config.h and possibly defined in a new
 * device_config.c as this functionality may be needed across apps
 * 
 * @endinternal
 *
 * @param[in]  metrics        The desired feature space metrics to be converted.
 * @param[out] device_config  The struct where the parameters calculated from the metrics are
 *                            written to.
 */
ifx_Error_t ifx_presencesensing_tranlate_metrics_to_config(const ifx_PresenceSensing_Metrics_t* metrics,
                                                           ifx_Device_Config_t* device_config);                                                                                       

#endif  // IFX_RADAR_PRESENCESENSING_H
