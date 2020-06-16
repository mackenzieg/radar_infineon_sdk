#include "mti.hpp"

mti::mti(radar_config *radar_config, uint32_t buffer_length, uint32_t bin_min, uint32_t bin_max) : m_radar_config(radar_config),
                                                                                                   m_buffer_length(buffer_length),
                                                                                                   m_bin_min(bin_min),
                                                                                                   m_bin_max(bin_max)
{
    this->m_net_bins = bin_max - bin_min + 1;

    m_buff_location = new uint32_t[this->m_net_bins];
    m_slow_time_buffer = new ifx_Vector_C_t[this->m_net_bins];
    for (int i = 0; i < m_net_bins; ++i)
    {
        m_buff_location[i] = 0;
        ifx_vector_create_c(m_buffer_length, &m_slow_time_buffer[i]);
        for (int x = 0; x < m_buffer_length; ++x)
        {
            m_slow_time_buffer[i].data[x].data[REAL] = 0.0f;
            m_slow_time_buffer[i].data[x].data[IMAG] = 0.0f;
        }
    }
}

mti::~mti()
{
    delete[] m_buff_location;
    delete[] m_slow_time_buffer;
}

void mti::train(ifx_Matrix_C_t* range_fft)
{
    int buffer_index = 0;
    for (int i = m_bin_min; i <= m_bin_max; ++i, ++buffer_index)
    {
        ifx_Complex_t element;

        // Note zero to always take first chirp. This can be changed to include all chirps etc
        ifx_matrix_get_element_c(range_fft, 0, i, &element);

        // Use circular buffer to generate average
        m_slow_time_buffer[buffer_index].data[m_buff_location[buffer_index]].data[REAL] = element.data[REAL];
        m_slow_time_buffer[buffer_index].data[m_buff_location[buffer_index]].data[IMAG] = element.data[IMAG];

        m_buff_location[buffer_index] += 1;
        m_buff_location[buffer_index] %= m_buffer_length;
    }
}

#include <iostream>

void mti::train_average(ifx_Matrix_C_t* range_fft)
{
    this->train(range_fft);

    int buffer_index = 0;
    for (int i = m_bin_min; i <= m_bin_max; ++i, ++buffer_index)
    {
        float avg_real = 0.0f;
        float avg_imag = 0.0f;

        for (int x = 0; x < m_buffer_length; ++x)
        {
            avg_real += m_slow_time_buffer[buffer_index].data[x].data[REAL];
            avg_imag += m_slow_time_buffer[buffer_index].data[x].data[IMAG];
        }

        avg_real /= m_buffer_length;
        avg_imag /= m_buffer_length;

        //std::cout << avg_real << std::endl;

        ifx_Complex_t element;

        ifx_matrix_get_element_c(range_fft, 0, i, &element);

        element.data[REAL] -= avg_real;
        element.data[IMAG] -= avg_imag;

        ifx_matrix_set_element_c(range_fft, 0, i, element);
    }
}