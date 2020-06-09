#include "mti.hpp"


mti::mti(radar_config *radar_config, uint32_t buffer_length, uint32_t bin_min, uint32_t bin_max) : radar_config(radar_config),
                                                                                                   buffer_length(buffer_length),
                                                                                                   bin_min(bin_min),
                                                                                                   bin_max(bin_max)
{
    this->net_bins = bin_max - bin_min + 1;

    buff_location = new uint32_t[this->net_bins];
    slow_time_buffer = new ifx_Vector_C_t[this->net_bins];
    for (int i = 0; i < this->net_bins; ++i)
    {
        buff_location[i] = 0;
        ifx_vector_create_c(buffer_length, &slow_time_buffer[i]);
        for (int x = 0; x < buffer_length; ++x)
        {
            slow_time_buffer[i].data[x].data[REAL] = 0.0f;
            slow_time_buffer[i].data[x].data[IMAG] = 0.0f;
        }
    }
}

virtual mti::~mti()
{

}

void mti::train(ifx_Matrix_C_t range_fft)
{
    int buffer_index = 0;
    for (int i = bin_min; i <= bin_max; ++i, ++buffer_index)
    {
        ifx_Complex_t element;

        // Note zero to always take first chirp. This can be changed to include all chirps etc
        ifx_matrix_get_element_c(range_fft, 0, i, &element);

        //Using circular byuf
        slow_time_buffer[buffer_index][buff_location[buffer_index]].data[REAL] = element.data[REAL];
        slow_time_buffer[buffer_index][buff_location[buffer_index]].data[IMAG] = element.data[IMAG];

        slow_time_buffer[i].data[x].data[REAL] = ;
        slow_time_buffer[i].data[x].data[IMAG] = 0.0f;

        ++buff_location[buffer_index];
        buff_location[buffer_index] %= buffer_length;
    }
}

void mti::train_average(ifx_Matrix_C_t range_fft)
{

}