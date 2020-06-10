#ifndef MTI_HPP
#define MTI_HPP

#include "ifxRadar_Matrix.h"
#include "ifxRadar_Vector.h"

#include "radar_config.hpp"

#define REAL 0
#define IMAG 1

class mti {
    public:
        mti(radar_config* radar_config, uint32_t buffer_length, uint32_t bin_min, uint32_t bin_max);
        virtual ~mti();

        void train_average(ifx_Matrix_C_t* range_fft);
    protected:
    private:
        radar_config* m_radar_config;

        // Holds array of slow time buffers to hold rolling average
        ifx_Vector_C_t* m_slow_time_buffer;
        // Holds the current buffer location to insert into for rolling buffer
        uint32_t* m_buff_location;

        uint32_t m_buffer_length;
        uint32_t m_net_bins;
        uint32_t m_bin_min;
        uint32_t m_bin_max;

        void train(ifx_Matrix_C_t* range_fft);
};


#endif //MTI_HPP
