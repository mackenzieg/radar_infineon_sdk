#ifndef RADAR_CONFIG_H
#define RADAR_CONFIG_H

#include "ifxRadar_DeviceConfig.h"
#include "ifxRadar_FFT.h"
#include "ifxRadar_Window.h"
#include "ifxRadar_RangeSpectrum.h"

#include "json.hpp"
using json = nlohmann::json;

typedef struct
{
    float m_range_resolution;   /**< The range resolution is the distance between two consecutive
                                     bins of the range transform. Note that even though zero
                                     padding before the range transform seems to increase this
                                     resolution, the true resolution does not change but depends
                                     only from the acquisition parameters. Zero padding is just
                                     interpolation! */
    float m_maximum_range;      /**< The bins of the range transform represent the range
                                     between 0m and this value. (If the time domain input data it
                                     is the range-maximum_range_m ... maximum_range_m.) */

    float m_minimum_range;      /**< The bins of the range transform represent the range
                                     between 0m and this value. (If the time domain input data it
                                     is the range-maximum_range_m ... maximum_range_m.) */

    float m_speed_resolution;   /**< The speed resolution is the distance between two consecutive
                                     bins of the Doppler transform. Note that even though zero
                                     padding before the speed transform seems to increase this
                                     resolution, the true resolution does not change but depends
                                     only from the acquisition parameters. Zero padding is just
                                     interpolation! */
    float m_maximum_speed;      /**< The bins of the Doppler transform represent the speed values
                                     between -maximum_speed_m_s and maximum_speed_m_s. */

    float m_frame_rate;

    uint32_t m_adc_samplerate_hz;

    uint8_t m_bgt_tx_power;

    uint8_t m_rx_antenna_number;

    int8_t m_if_gain_db;

    uint32_t m_fmcw_center_frequency_khz;

    ifx_Window_Type_t m_range_fft_window_type;

    ifx_FFT_Size_t m_range_fft_size;

    float m_range_fft_window_alpha;

    ifx_Range_Spectrum_Mode_t m_range_spectrum_mode;
    float m_threshold_factor_presence_peak;
    float m_threshold_factor_absence_peak;
    float m_threshold_factor_absence_fine_peak;

    float m_mti_weight;

    float m_value_per_bin;
} device_metrics_t;

class radar_config
{
    public:
        radar_config();
        virtual ~radar_config();

        device_metrics_t* get_device_metrics();
        ifx_Device_Config_t* get_device_config();
        ifx_Range_Spectrum_Config_t* get_range_spectrum_config();

        json create_json();

    protected:

    private:
        void compute_metrics();

        // Metrics for device
        device_metrics_t m_device_metrics;
        // SDK Device config structure
        ifx_Device_Config_t m_device_config;

        ifx_Range_Spectrum_Config_t m_range_spectrum_config;
};

#endif // RADAR_CONFIG_H
