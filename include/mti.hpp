#ifndef TI_HPP
#define MTI_HPP

#include "ifxRadar_Matrix.h"
#include "ifxRadar_Vector.h"

class mti {
    public:
        mti(radar_config *radar_config, uint32_t buffer_length, uint32_t bin_min, uint32_t bin_max);
        virtual ~mti();

        void train(ifx_Matrix_C_t range_fft);

        void train_average(ifx_Matrix_C_t range_fft);
    protected:
    private:
        radar_config* radar_config;

        // Holds array of slow time buffers to hold rolling average
        ifx_Vector_C_t slow_time_buffer[];
        // Holds the current buffer location to insert into for rolling buffer
        uint32_t buff_location[];

        uint32_t buffer_length;
        uint32_t net_bins;
        uint32_t bin_min;
        uint32_t bin_max;
};


#endif //MTI_HPP
