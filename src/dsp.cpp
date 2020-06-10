#include "dsp.hpp"

#include "ifxRadar_Error.h"

#include <math.h>

dsp::dsp(radar_config* radar_config) : m_radar_config(radar_config), num_frames_per_fft(NUM_FFT_POINTS / radar_config->get_device_config()->num_chirps_per_frame)
{
    uint32_t bin_number = (uint32_t) (0.5f / m_radar_config->get_device_metrics()->m_value_per_bin);
    m_mti_test_handle = new mti(m_radar_config, NUM_FFT_POINTS, bin_number - 1, bin_number + 1);

    this->create_spectrum_handle();
    this->create_mti_handle();
    this->create_doppler_fft_handle();
}

dsp::~dsp()
{
    delete m_mti_test_handle;
    this->destroy_spectrum_handle();
    this->destroy_mti_handle();
    this->destroy_doppler_fft_handle();
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

#include <iostream>
#include <fstream>
#include <iomanip>

#include <chrono>
using namespace std;


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

    // Give MTI filter time to train against clutter
    if (this->run_count <= MTI_FILTER_TRAIN_FRAMES)
    {
        return;
    }

    ret = ifx_peak_search_run(peak_search, &(this->m_mti.mti_result), &fine_peak_result);

    // bin number for 0.5 m
    uint32_t bin_number = (uint32_t) (0.5f / m_radar_config->get_device_metrics()->m_value_per_bin);
    //bin_number = 5;
    //uint32_t bin_number = last_peak_detected;
//    if (last_peak_detected == -1)
//    {
//        last_peak_detected = fine_peak_result.index[0];
//    }

    //uint32_t bin_number = fine_peak_result.index[0];
//
//    ifx_Complex_t avg;
//    avg.data[REAL] = 0.0f;
//    avg.data[IMAG] = 0.0f;
//    for (uint32_t midx = 0; midx < this->m_range_spectrum.frame_fft_half_result.rows; ++midx)
//    {
//        ifx_Complex_t element;
//        ifx_matrix_get_element_c(&(this->m_range_spectrum.frame_fft_half_result), midx, bin_number, &element);
//        avg.data[REAL] += element.data[REAL];
//        avg.data[IMAG] += element.data[IMAG];
//    }
//
//    avg.data[REAL] /= this->m_range_spectrum.frame_fft_half_result.rows;
//    avg.data[IMAG] /= this->m_range_spectrum.frame_fft_half_result.rows;

    m_mti_test_handle->train_average(&(this->m_range_spectrum.frame_fft_half_result));

    ifx_Complex_t element;
    ifx_matrix_get_element_c(&(this->m_range_spectrum.frame_fft_half_result), 0, bin_number, &element);

    signal[curr_frames_sampled][REAL] = element.data[REAL];
    signal[curr_frames_sampled][IMAG] = element.data[IMAG];

    if (curr_frames_sampled != NUM_FFT_POINTS - 1)
    {
        ++curr_frames_sampled;
        return;
    }

    curr_frames_sampled = 0;


    // Windowing
//    for (int i = 0; i < NUM_FFT_POINTS; ++i)
//    {
//        signal[i][REAL] *= doppler_fft_window.data[i] * doppler_window_scale;
//        signal[i][IMAG] *= doppler_fft_window.data[i] * doppler_window_scale;
//    }

    double avg_real = 0.0;
    double avg_imag = 0.0;
    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        avg_real += signal[i][REAL];
        avg_imag += signal[i][IMAG];
    }
    avg_real /= NUM_FFT_POINTS;
    avg_imag /= NUM_FFT_POINTS;

    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        signal[i][REAL] -= avg_real;
        signal[i][IMAG] -= avg_imag;
    }

    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        float abs = sqrt(signal[i][REAL] * signal[i][REAL] + signal[i][IMAG] * signal[i][IMAG]);
        //float abs = signal[i][REAL];
        cout << std::setprecision(40) << abs << " ";
    }

    cout << endl;

    ofstream myfile;
    myfile.open("pre-fft.txt");

    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        float abs = sqrt(signal[i][REAL] * signal[i][REAL] + signal[i][IMAG] * signal[i][IMAG]);
        myfile << std::setprecision(40) << abs << " ";
    }

    myfile << endl;

    myfile.close();

    fftw_plan plan = fftw_plan_dft_1d(NUM_FFT_POINTS,
                                      signal,
                                      result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);

    fftw_execute(plan);

    fftw_destroy_plan(plan);

    double freq_per_bin = ((double)m_radar_config->get_device_config()->frame_period_us) / pow(10 , 6);
    freq_per_bin = (1 / freq_per_bin) / (NUM_FFT_POINTS);

    myfile.open("fft.txt");

    for (int i = 0; i < NUM_FFT_POINTS; ++i)
    {
        float abs = sqrt(result[i][REAL] * result[i][REAL] + result[i][IMAG] * result[i][IMAG]);
        myfile << std::setprecision(40) << abs << " ";
        cout << std::setprecision(40) << abs << " ";
    }

    myfile << endl;
    cout << endl;

    myfile.close();

    ret = ifx_peak_search_destroy(peak_search);
    ret = ifx_vector_destroy_r(&doppler_fft_window);

    time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    //cout << time_stamp << endl;
}

void dsp::insert_new_sample(ifx_Vector_C_t* new_sample)
{
    int samples_per_frame = new_sample->length;

    int fft_to_sample_ratio = NUM_FFT_POINTS / samples_per_frame;

    int x = 0;
    for (int i = (1 * samples_per_frame); i < NUM_FFT_POINTS; ++i, ++x)
    {
        signal[x][REAL] = signal[i][REAL];
        signal[x][IMAG] = signal[i][IMAG];
    }

    x = 0;
    for (int i = ((fft_to_sample_ratio - 1) * samples_per_frame - 1); i < NUM_FFT_POINTS; ++i, ++x)
    {
        signal[i][REAL] = new_sample->data[x].data[REAL];
        signal[i][IMAG] = new_sample->data[x].data[IMAG];
    }
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
