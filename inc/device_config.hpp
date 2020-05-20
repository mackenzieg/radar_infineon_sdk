#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include "ifxRadar_DeviceConfig.h"

class device_config
{
    public:
        device_config();
        virtual ~device_config();

        ifx_Device_Config_t* get_device_config();

    protected:

    private:
        void compute_metrics();

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
        } device_metrics_t;

        // Metrics for device
        device_metrics_t m_device_metrics;
        // SDK Device config structure
        ifx_Device_Config_t m_device_config;
};

#endif // DEVICE_CONFIG_H
