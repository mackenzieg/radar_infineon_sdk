#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "ifxRadarSDK.h"
#include "ifxRadar_Error.h"
#include "radar.hpp"

/*
==============================================================================
   LOCAL DEFINITIONS
==============================================================================
*/

//These are some of the hardcoded values for the ifx_PresenceSensing_Config_t to be used in this application

#define IFX_ADC_SAMPLERATE_HZ           (1000000U)  /**< Samplerate of the ADC in Hz ==> [100k - 2M]
                                                        @internal The app needs to be tested with other possible sample rates as well,
                                                        e.g., for higher bandwidths @endinternal */

#define IFX_RANGE_FFT_WINDOW_TYPE       (2U)        /**< Windowing Function applied on range fft input data. See \ref ifx_Window_Type_t. */
#define IFX_RANGE_FFT_WINDOW_AT_DB      (0.0f)      /**< Attenuation in dB, this parameter is only needed if windowing type is chebyshev */
#define IFX_DOPPLER_FFT_WINDOW_TYPE     (3U)        /**< Windowing Function applied on doppler fft input data. See \ref ifx_Window_Type_t. */
#define IFX_DOPPLER_FFT_WINDOW_AT_DB    (60.0f)     /**< Attenuation in dB, this parameter is only needed if windowing type is chebyshev */

#define IFX_RANGE_SPECTRUM_MODE     (3U)        /**< mode of calculation of range spectrum.
                                                    0 = SINGLE CHIRP MODE , 1 = COHERENT INTEGRATION MODE (DEFAULT), 2 = MAXIMUM ENERGY MODE ,
                                                          3 = MAX ENERGY RANGE BIN (see \ref ifx_Range_Spectrum_Mode_t) */

#define IFX_THRESHOLD_FACTOR_PRESENCE_PEAK          (3.0f)      /**< Decides threshold factor param in ifx_Peak_Search_Config_t for configuring the
                                                                   "presence_peak" peak search module that is active in the PRESENCE state [internal param] */

#define IFX_THRESHOLD_FACTOR_ABSENCE_PEAK           (4.0f)      /**< Decides threshold factor param in ifx_Peak_Search_Config_t for configuring the
                                                                   "absence_peak" peak search module that is active in the ABSENCE state [internal param] */

#define IFX_THRESHOLD_FACTOR_ABSENCE_FINE_PEAK      (1.5f)      /**< Decides threshold factor param in ifx_Peak_Search_Config_t for configuring the
                                                                   "absence_fine_peak" peak search module that is active in the ABSENCE state and decides the
                                                                         range bins on which the slow time FFT needs to be performed [internal param] */
bool running = true;

void signal_handle(int sig)
{
    if (sig != SIGINT)
        return;

    running = false;
}

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handle);

	printf("Running Radar SDK version: %s\n", ifx_radar_sdk_get_version_string());

    device_config dc;

    radar radar(dc);
    printf("Pulling frame from device\n");
    radar.pull_frame();

    ifx_Error_t ret = IFX_OK;
	while (running)
    {
        ret = radar.pull_frame();
        // No data avaliable data to pull
        if (ret == IFX_ERROR_FIFO_OVERFLOW)
        {
            continue;
        }

        ifx_Frame_t* frame = radar.get_frame();

        printf("Num of active rx: %d", frame->num_rx);
    }

    printf("Closing connection\n");

    return 0;
}
