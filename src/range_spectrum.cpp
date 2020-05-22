#include "range_spectrum.hpp"

range_spectrum::range_spectrum(radar_config *radar_config) : m_radar_config(radar_config)
{
    if (ifx_range_spectrum_create(m_radar_config->get_range_spectrum_config(), &m_range_spectrum_handle))
    {
        // TODO error check
    }

    if (ifx_range_spectrum_set_mode(m_range_spectrum_handle, m_radar_config->get_device_metrics()->m_range_spectrum_mode))
    {
        // TODO error check
    }

    if (ifx_vector_create_r(radar_config->get_device_metrics()->m_range_fft_size, m_fft_spectrum_result))
    {
        // TODO error check
    }

    if (ifx_vector_create_c(radar_config->get_device_config()->num_samples_per_chirp,
                            radar_config->get_device_metrics()->m_range_fft_size / 2,
                            m_frame_fft_half_result))
    {
        // TODO error check
    }
}

range_spectrum::~range_spectrum()
{
    if (ifx_range_spectrum_destroy(m_range_spectrum_handle))
    {
        // TODO error check
    }

    if (ifx_vector_destroy_r(m_fft_spectrum_result))
    {

    }

    if (ifx_vector_destroy_c(m_frame_fft_half_result))
    {

    }
}
void range_spectrum::run(ifx_Matrix_R_t* frame_data)
{
    if (ifx_range_spectrum_run_r(m_range_spectrum_handle, frame_data, m_fft_spectrum_result))
    {

    }

    if (ifx_range_spectrum_get_fft_transformed_matrix(m_range_spectrum_handle, m_frame_fft_half_result))
    {

    }
}
