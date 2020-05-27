#include "dsp.hpp"

#include "ifxRadar_Error.h"

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

    if (ifx_matrix_create_c(m_radar_config->get_device_config()->num_samples_per_chirp,
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
    if (ifx_range_spectrum_run_r(this->m_range_spectrum.range_spectrum_handle, &rx_data, &(this->m_range_spectrum.fft_spectrum_result)))
    {

    }

    if (ifx_range_spectrum_get_fft_transformed_matrix(this->m_range_spectrum.range_spectrum_handle, &(this->m_range_spectrum.frame_fft_half_result))) {

    }

    /**************************** Remove static object (MTI filter) ***********************/
    ifx_vector_copy_r(&(this->m_range_spectrum.fft_spectrum_result), 0,
                      this->m_range_spectrum.fft_spectrum_result.length / 2, &(this->m_mti.mti_result));

    ifx_mti_run(this->m_mti.mti_handle, &(this->m_mti.mti_result));

    for (int i = 0; i < this->m_mti.mti_result.length; ++i)
    {
        printf("%f, ", this->m_mti.mti_result.data[i]);
    }

    printf("\n");
}
