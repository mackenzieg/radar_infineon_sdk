#include "dsp.hpp"

#include "ifxRadar_Error.h"

#include <math.h>

dsp::dsp(radar_config* radar_config) : m_radar_config(radar_config), num_frames_per_fft(NUM_FFT_POINTS / radar_config->get_device_config()->num_chirps_per_frame)
{
    float temp_bin = (range_interest / m_radar_config->get_device_metrics()->m_value_per_bin);

    important_bin = (uint32_t) (temp_bin + 0.5f);

    ofstream metrics_file;
    metrics_file.open ("metrics.txt");

    metrics_file << "range_resolution: " << m_radar_config->get_device_metrics()->m_range_resolution << endl;
    metrics_file << "range_interest: " << range_interest << endl;
    metrics_file << "bin_interest: " << important_bin << endl;

    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        metrics_file <<  ((float)i) * (1/ ((float)m_radar_config->get_device_metrics()->m_frame_rate)) ;
        if (i < NUM_FFT_POINTS - 1)
        {
            metrics_file << ", ";
        }
    }
    metrics_file << endl;

    metrics_file.close();

    int range = 4;

    if (((int)important_bin) - range < 0)
    {
        min_bin = 0;
    } else {
        min_bin = important_bin - ((uint32_t)range);
    }
    max_bin = important_bin + range;
    delta_bin = max_bin - min_bin + 1;

    mti_buffer_length = m_radar_config->get_device_metrics()->m_frame_rate * 4;

    m_mti_test_handle = new mti(m_radar_config, mti_buffer_length, min_bin, max_bin);
    fft_handle = new fft_circular[delta_bin];

    data_file.open ("data.txt");

    this->create_spectrum_handle();
    this->create_mti_handle();
    this->create_doppler_fft_handle();
}

dsp::~dsp()
{
    delete m_mti_test_handle;
    delete[] fft_handle;
    this->destroy_spectrum_handle();
    this->destroy_mti_handle();
    this->destroy_doppler_fft_handle();

    data_file.close();
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

void dsp::create_doppler_fft_handle()
{
    ifx_fft_create(FFT_TYPE_C2C, m_radar_config->get_device_config()->num_chirps_per_frame, (ifx_FFT_Size_t) (m_radar_config->get_device_config()->num_chirps_per_frame * 2), &(this->m_doppler_fft.doppler_fft_handle));

    ifx_vector_create_c(m_radar_config->get_device_config()->num_chirps_per_frame * 2, &(this->m_doppler_fft.chirp_fft_result));

    ifx_vector_create_c(m_radar_config->get_device_config()->num_chirps_per_frame, &(this->m_doppler_fft.doppler_data));
}

void dsp::destroy_doppler_fft_handle()
{
    ifx_vector_destroy_c(&(this->m_doppler_fft.chirp_fft_result));
    ifx_vector_destroy_c(&(this->m_doppler_fft.doppler_data));
    ifx_fft_destroy(this->m_doppler_fft.doppler_fft_handle);
}

void dsp::run(ifx_Frame_t frame)
{
    this->run_count += 1;

    float value_per_bin = m_radar_config->get_device_metrics()->m_value_per_bin;

    float max_range = m_radar_config->get_device_metrics()->m_maximum_range;
    float min_range = m_radar_config->get_device_metrics()->m_minimum_range;

    ifx_Vector_R_t chirp_fft_result_abs;
    ifx_Error_t ret = ifx_vector_create_r(m_radar_config->get_device_config()->num_chirps_per_frame * 2, &chirp_fft_result_abs);

    /* Create doppler window */
    ifx_Window_Config_t doppler_fft_window_config;
    //doppler_fft_window_config.type = WINDOW_CHEBYSHEV;
    doppler_fft_window_config.type = WINDOW_HANN;
    doppler_fft_window_config.size = NUM_FFT_POINTS;
    doppler_fft_window_config.at_dB = 60.0f;

    ifx_Vector_R_t doppler_fft_window;
    ifx_vector_create_r(doppler_fft_window_config.size, &doppler_fft_window);

    ifx_window_init(&doppler_fft_window_config, &doppler_fft_window);

    float doppler_window_scale = create_scale(&doppler_fft_window);

    /* Create peak search */

    ifx_Peak_Search_Config_t fine_peak_search_config = {
            .value_per_bin = value_per_bin,
            .search_zone_start = min_range,
            .search_zone_end = max_range,
            .threshold_factor = 1.5f,
            .threshold_offset = 0,
            .max_num_peaks = 1
    };

    ifx_Peak_Search_Handle_t peak_search;

    ret = ifx_peak_search_create(&fine_peak_search_config,
                                 &peak_search);

    ifx_Peak_Search_Result_t fine_peak_result = {0};

    ifx_Matrix_R_t rx_data = frame.rx_data[0];
    /*************************************************************************************/
    /*                                    Range Processing                               */
    /*************************************************************************************/
    ret = ifx_range_spectrum_run_r(this->m_range_spectrum.range_spectrum_handle, &rx_data, &(this->m_range_spectrum.fft_spectrum_result));

    ret = ifx_range_spectrum_get_fft_transformed_matrix(this->m_range_spectrum.range_spectrum_handle, &(this->m_range_spectrum.frame_fft_half_result));

    /**************************** Remove static object (MTI filter) ***********************/
    ifx_vector_copy_r(&(this->m_range_spectrum.fft_spectrum_result), 0,
                      this->m_range_spectrum.fft_spectrum_result.length / 2, &(this->m_mti.mti_result));

    ret = ifx_mti_run(this->m_mti.mti_handle, &(this->m_mti.mti_result));

    //m_mti_test_handle->train_average(&(this->m_range_spectrum.frame_fft_half_result));

    // Give MTI filter time to train against clutter
    if (this->run_count <= mti_buffer_length)
    {
        return;
    } else if(this->run_count == mti_buffer_length + 1) {
        cout << "Completed Calibration" << endl;
    }

    ret = ifx_peak_search_run(peak_search, &(this->m_mti.mti_result), &fine_peak_result);

    uint32_t x = 0;
    for (uint32_t i = min_bin; i <= max_bin; ++i, ++x)
    {
        ifx_Complex_t element;
        ifx_matrix_get_element_c(&(this->m_range_spectrum.frame_fft_half_result), 0, i, &element);
        fft_handle[x].sample(element);
    }

    fftw_complex* result = fft_handle[0].get_result();
    if (result == nullptr)
    {
        return;
    }

    for (uint32_t x = 0; x < NUM_FFT_POINTS; ++x)
    {
        integrated[x][REAL] = 0.0f;
        integrated[x][IMAG] = 0.0f;

        uint32_t curr_bin = min_bin;
        for (uint32_t i = 0; i < delta_bin; ++i, ++ curr_bin)
        {
            fftw_complex* signal = fft_handle[i].get_signal();

            integrated[x][REAL] += signal[x][REAL];
            integrated[x][IMAG] += signal[x][IMAG];
        }
    }

    for (uint32_t i = 0; i < NUM_FFT_POINTS; ++i)
    {
        cout << integrated[i][REAL];
        if (i < NUM_FFT_POINTS - 1)
        {
            cout << std::setprecision(40) << ", ";
        }
    }
    cout << endl;

    uint32_t curr_bin = min_bin;
    for (uint32_t i = 0; i < delta_bin; ++i, ++ curr_bin)
    {

        fftw_complex* result = fft_handle[i].get_result();
        if (result == nullptr)
        {
            return;
        }
        fftw_complex* signal = fft_handle[i].get_signal();

        data_file << "curr_bin: " << curr_bin << endl;
        data_file << "real: " << curr_bin << endl;

        for (int i = 0; i < NUM_FFT_POINTS; ++i)
        {
            float abs = signal[i][REAL];
            //float abs = signal[i][REAL];
            data_file << std::setprecision(40) << abs;

            if (i < NUM_FFT_POINTS - 1)
            {
                data_file << ", ";
            }
        }
        data_file << endl;
        data_file << "imag: " << curr_bin << endl;

        for (int i = 0; i < NUM_FFT_POINTS; ++i)
        {
            //float abs = sqrt(signal[i][REAL] * signal[i][REAL] + signal[i][IMAG] * signal[i][IMAG]);
            float abs = signal[i][IMAG];

            data_file << std::setprecision(40) << abs;

            if (i < NUM_FFT_POINTS - 1)
            {
                data_file << ", ";
            }
        }
        data_file << endl;
    }

//    double slow_time_sample_time = ((double)m_radar_config->get_device_config()->frame_period_us) / pow(10 , 6);
//    double freq_per_bin = 1 / (slow_time_sample_time * NUM_FFT_POINTS);
//
//    uint32_t slow_time_min_bin = 1;
//    uint32_t slow_time_max_bin = ceil(1.0 / freq_per_bin);
//
//    for (int i = 0; i < delta_bin; ++i)
//    {
//        fftw_complex* result = fft_handle[i].get_result();
//
//        float max = 0.0f;
//        uint32_t max_index = 0;
//        for (int i = slow_time_min_bin; i <= slow_time_max_bin; ++i)
//        {
//            float abs = sqrt(result[i][REAL] * result[i][REAL] + result[i][IMAG] * result[i][IMAG]);
//            if (abs >= max)
//            {
//                max = abs;
//                max_index = i;
//            }
//        }
//        cout << (max_index * freq_per_bin) << endl;
//    }
    cout << "-------------------------------" << endl;

    ret = ifx_peak_search_destroy(peak_search);
    ret = ifx_vector_destroy_r(&doppler_fft_window);

    time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    cout << time_stamp << endl;

    cout << "-------------------------------" << endl;
}

void dsp::print_complex(fftw_complex* signal, ofstream &location)
{
    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        float abs = sqrt(signal[i][REAL] * signal[i][REAL] + signal[i][IMAG] * signal[i][IMAG]);
        location << std::setprecision(40) << abs << " ";
    }
    location << endl;
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
