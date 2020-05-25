#include "dsp.hpp"

dsp::dsp(radar_config* radar_config) : m_radar_config(radar_config)
{
    //ctor
}

dsp::~dsp()
{
    //dtor
}

void dsp::create_spectrum_handle()
{
    if (ifx_range_spectrum_create(m_radar_config->get_range_spectrum_config(), &(this->m_range_spectrum.range_spectrum_handle)))
    {
        // TODO error check
    }

    if (ifx_range_spectrum_set_mode(this->m_range_spectrum.range_spectrum_handle, m_radar_config->get_device_metrics()->m_range_spectrum_mode))
    {
        // TODO error check
    }

    if (ifx_vector_create_r(m_radar_config->get_device_metrics()->m_range_fft_size, this->m_range_spectrum.fft_spectrum_result))
    {
        // TODO error check
    }

    if (ifx_matrix_create_c(m_radar_config->get_device_config()->num_samples_per_chirp,
                            m_radar_config->get_device_metrics()->m_range_fft_size / 2,
                            this->m_range_spectrum.frame_fft_half_result))
    {

    }
}

void dsp::create_mti_handle() {
    if (ifx_mti_create(m_radar_config->get_device_metrics()->m_mti_weght,
                       m_radar_config->get_device_metrics()->m_range_fft_size / 2, &(this->m_mti.mti_handle))) {
        // TODO Error check
    }

    if (ifx_vector_create_r(m_radar_config->get_device_metrics()->m_range_fft_size / 2, this->m_mti.mti_result)) {


    }
}
