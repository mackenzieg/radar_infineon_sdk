#include "dsp.hpp"

#include "ifxRadar_Error.h"

#include <math.h>

#include <vector>

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

json dsp::run(ifx_Frame_t frame)
{
    time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this->run_count += 1;

    ifx_Matrix_R_t rx_data = frame.rx_data[0];
    ifx_Matrix_R_t rx_data1 = frame.rx_data[1];
    ifx_Matrix_R_t rx_data2 = frame.rx_data[2];

    json data;

    data["frame"] = std::vector<float>(rx_data.columns);


    for (int i = 0; i < rx_data.columns; ++i)
    {
//        float element;
//        float element1;
//        float element2;
//        ifx_matrix_get_element_r(&rx_data, 0, i, &element);
//        ifx_matrix_get_element_r(&rx_data1, 0, i, &element1);
//        ifx_matrix_get_element_r(&rx_data2, 0, i, &element2);
//        data["frame"][i] = element + element1 + element2;


        float element;
        ifx_matrix_get_element_r(&rx_data, 0, i, &element);

        data["frame"][i] = element;
    }


    return data;
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
