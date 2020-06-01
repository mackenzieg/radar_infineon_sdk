#include "dsp.hpp"

#include "ifxRadar_Error.h"

#include <math.h>

dsp::dsp(radar_config* radar_config) : m_radar_config(radar_config)
{
    this->create_spectrum_handle();
    this->create_mti_handle();
}

dsp::~dsp()
{
    this->destroy_spectrum_handle();
    this->destroy_mti_handle();
}

void dsp::create_spectrum_handle()
{
    ifx_Error_t err = ifx_range_spectrum_create(m_radar_config->get_range_spectrum_config(), &(this->m_range_spectrum.range_spectrum_handle));

    err = ifx_range_spectrum_set_mode(this->m_range_spectrum.range_spectrum_handle, this->m_radar_config->get_device_metrics()->m_range_spectrum_mode);


    if (ifx_vector_create_r((uint32_t) m_radar_config->get_device_metrics()->m_range_fft_size, &(this->m_range_spectrum.fft_spectrum_result)))
    {
        // TODO error check
    }

    if (ifx_matrix_create_c(m_radar_config->get_device_config()->num_chirps_per_frame,
                            m_radar_config->get_device_metrics()->m_range_fft_size / 2,
                            &(this->m_range_spectrum.frame_fft_half_result)))
    {

    }
}

void dsp::destroy_spectrum_handle()
{
    if (ifx_range_spectrum_destroy(this->m_range_spectrum.range_spectrum_handle))
    {
        // TODO error check
    }

    if (ifx_vector_destroy_r(&(this->m_range_spectrum.fft_spectrum_result)))
    {

    }

    if (ifx_matrix_destroy_c(&(this->m_range_spectrum.frame_fft_half_result)))
    {

    }
}

void dsp::create_mti_handle() {
    if (ifx_mti_create(m_radar_config->get_device_metrics()->m_mti_weight,
                       m_radar_config->get_device_metrics()->m_range_fft_size / 2, &(this->m_mti.mti_handle))) {
        // TODO Error check
    }

    if (ifx_vector_create_r(m_radar_config->get_device_metrics()->m_range_fft_size / 2, &(this->m_mti.mti_result))) {


    }
}

void dsp::destroy_mti_handle()
{
    if (ifx_mti_destroy(this->m_mti.mti_handle))
    {

    }

    if (ifx_vector_destroy_r(&(this->m_mti.mti_result)))
    {

    }
}


void dsp::run(ifx_Frame_t frame)
{
    ifx_Matrix_R_t rx_data = frame.rx_data[0];
    /*************************************************************************************/
    /*                                    Range Processing                               */
    /*************************************************************************************/
    ifx_Error_t ret = ifx_range_spectrum_run_r(this->m_range_spectrum.range_spectrum_handle, &rx_data, &(this->m_range_spectrum.fft_spectrum_result));

    ret = ifx_range_spectrum_get_fft_transformed_matrix(this->m_range_spectrum.range_spectrum_handle, &(this->m_range_spectrum.frame_fft_half_result));

    /**************************** Remove static object (MTI filter) ***********************/
    ifx_vector_copy_r(&(this->m_range_spectrum.fft_spectrum_result), 0,
                      this->m_range_spectrum.fft_spectrum_result.length / 2, &(this->m_mti.mti_result));

    ret = ifx_mti_run(this->m_mti.mti_handle, &(this->m_mti.mti_result));

    float value_per_bin = m_radar_config->get_device_metrics()->m_value_per_bin;

    float max_range = m_radar_config->get_device_metrics()->m_maximum_range;
    float min_range = m_radar_config->get_device_metrics()->m_minimum_range;

    /* Create Doppler FFT */
    uint32_t doppler_fft_size = m_radar_config->get_device_config()->num_chirps_per_frame * 2;

    ifx_FFT_Handle_t doppler_fft_handle;
    ret = ifx_fft_create(FFT_TYPE_C2C, m_radar_config->get_device_config()->num_chirps_per_frame, (ifx_FFT_Size_t) doppler_fft_size, &doppler_fft_handle);

    ifx_Vector_C_t chirp_fft_result;
    ifx_vector_create_c(doppler_fft_size, &chirp_fft_result);

    ifx_Vector_R_t chirp_fft_result_abs;
    ifx_vector_create_r(doppler_fft_size, &chirp_fft_result_abs);

    /* Create doppler window */
    ifx_Window_Config_t doppler_fft_window_config;
    doppler_fft_window_config.type = WINDOW_CHEBYSHEV;
    doppler_fft_window_config.size = m_radar_config->get_device_config()->num_chirps_per_frame;
    doppler_fft_window_config.at_dB = 60.0f;

    ifx_Vector_R_t doppler_fft_window;
    ret = ifx_vector_create_r(doppler_fft_window_config.size, &doppler_fft_window);

    ifx_window_init(&doppler_fft_window_config, &doppler_fft_window);

    float doppler_window_scale = create_scale(&doppler_fft_window);

    /* Create peak search */

    ifx_Peak_Search_Config_t fine_peak_search_config = {.value_per_bin = value_per_bin,
            .search_zone_start = min_range,
            .search_zone_end = max_range,
            .threshold_factor = 1.5f,
            .threshold_offset = 0,
            .max_num_peaks = 15};

    ifx_Peak_Search_Handle_t peak_search;

    ret = ifx_peak_search_create(&fine_peak_search_config,
                                 &peak_search);

    ifx_Peak_Search_Result_t fine_peak_result = {0};

    ret = ifx_peak_search_run(peak_search, &(this->m_mti.mti_result), &fine_peak_result);

    ifx_Vector_C_t doppler_data;

    ret = ifx_vector_create_c(m_radar_config->get_device_config()->num_chirps_per_frame, &doppler_data);

    ifx_Vector_C_t doppler_data_remove_mean;

    ret = ifx_vector_create_c(m_radar_config->get_device_config()->num_chirps_per_frame, &doppler_data_remove_mean);

    for (int i = 0; i < fine_peak_result.peak_count; ++i)
    {
        uint32_t pidx = fine_peak_result.index[i];

        for (uint32_t midx = 0; midx < this->m_range_spectrum.frame_fft_half_result.rows; ++midx)
        {
            ifx_Complex_t element;
            ifx_matrix_get_element_c(&(this->m_range_spectrum.frame_fft_half_result), midx, pidx, &element);
            doppler_data.data[midx] = element;
        }

        /* Remove mean */
        ifx_Complex_t mean = {0};
        ifx_math_get_mean_c(&doppler_data, &mean);

        ifx_math_subtract_scalar_c(&doppler_data, mean, &doppler_data_remove_mean);

        /*
         * Doppler windowing
         * 1. Multiply Scale
         * 2. Multiply Window
         */
        for (uint32_t i = 0; i < doppler_data.length; ++i)
        {
            doppler_data.data[i] = ifx_complex_mul_scalar(doppler_data_remove_mean.data[i], doppler_fft_window.data[i] * doppler_window_scale);
        }

        ret = ifx_fft_run_c(doppler_fft_handle, &doppler_data_remove_mean, &chirp_fft_result);

        fft_shift(&chirp_fft_result);

        ifx_math_vector_abs_c(&chirp_fft_result, &chirp_fft_result_abs);

        // Convert 100ps units to s
        double freq_per_bin = ((double)m_radar_config->get_device_config()->chirp_to_chirp_time_100ps) / pow(10 , 10);
        freq_per_bin = (1 / freq_per_bin) / (m_radar_config->get_device_config()->num_chirps_per_frame);

        int x = 0;
    }

    ret = ifx_peak_search_destroy(peak_search);
    ifx_vector_destroy_c(&doppler_data);
    ifx_vector_destroy_c(&doppler_data_remove_mean);
    ifx_vector_destroy_c(&chirp_fft_result);
    ifx_vector_destroy_r(&doppler_fft_window);
    ifx_fft_destroy(doppler_fft_handle);
}

float dsp::create_scale(ifx_Vector_R_t* win)
{
    ifx_Float_t fnorm = 0;

    for (uint32_t i = 0; i < win->length; ++i)
    {
        fnorm += win->data[i];
    }

    if (fnorm != 0)
    {
        fnorm = 1.0f / fnorm;

        return fnorm;
    }

    return 1.0f;
}

void dsp::fft_shift(ifx_Vector_C_t* vector)
{
    ifx_Complex_t temp = {0};
    uint32_t idx = vector->length / 2;

    for (uint32_t i = 0; i < vector->length / 2; ++i, ++idx)
    {
        temp = vector->data[i];
        vector->data[i] = vector->data[idx];
        vector->data[idx] = temp;
    }
}
