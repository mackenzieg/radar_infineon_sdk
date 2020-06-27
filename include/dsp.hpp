#ifndef DSP_HPP
#define DSP_HPP

#define MTI_FILTER_TRAIN_FRAMES 5

#include "ifxRadar_RangeSpectrum.h"
#include "ifxRadar_MTI.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Matrix.h"
#include "ifxRadar_Frame.h"
#include "ifxRadar_PeakSearch.h"

#include "mti.hpp"
#include "radar_config.hpp"

#include "json.hpp"
#include "fft_circular.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <chrono>
using namespace std;

#include <string>

class dsp
{
    public:
        dsp(radar_config *radar_config);
        virtual ~dsp();

        json run(ifx_Frame_t frame);

    protected:

    private:
        typedef struct
        {
            ifx_Range_Spectrum_Handle_t range_spectrum_handle;

            ifx_Vector_R_t fft_spectrum_result;

            ifx_Matrix_C_t frame_fft_half_result;
        } range_spectrum_t;

        typedef struct
        {
            ifx_MTI_Handle_t mti_handle;

            ifx_Vector_R_t mti_result;
        } mti_t;

        typedef struct
        {
            ifx_FFT_Handle_t doppler_fft_handle;

            ifx_Vector_C_t doppler_data;

            ifx_Vector_C_t chirp_fft_result;
        } doppler_fft_t;

        range_spectrum_t m_range_spectrum;

        mti_t m_mti;

        uint32_t mti_buffer_length;
        mti* m_mti_test_handle;

        doppler_fft_t m_doppler_fft;

        radar_config* m_radar_config;

        unsigned long int time_stamp;

        long unsigned int run_count = 0;

        uint32_t min_bin;
        uint32_t max_bin;
        uint32_t delta_bin;
        uint32_t important_bin;

        /*
         * Doppler FFT related variables
         */
        fft_circular* fft_handle;

        string file_name;

        ofstream data_file;

        fftw_complex integrated[NUM_FFT_POINTS];

        float range_interest = 1.0f;

        int num_frames_per_fft;
        int curr_frames_sampled = 0;

        void create_spectrum_handle();
        void destroy_spectrum_handle();

        void create_mti_handle();
        void destroy_mti_handle();

        void create_range_doppler_handle();
        void destroy_range_doppler_handle();

        void create_doppler_fft_handle();
        void destroy_doppler_fft_handle();

        float create_scale(ifx_Vector_R_t* win);
        void fft_shift(ifx_Vector_C_t* vector);

        void insert_new_sample(ifx_Vector_C_t* new_sample);

        void print_complex(fftw_complex *signal, ofstream &location);
};

#endif // DSP_HPP
