#include "fft_circular.hpp"

#define REAL 0
#define IMAG 1

fft_circular::fft_circular()
{
    plan = fftw_plan_dft_1d(NUM_FFT_POINTS,
                                    signal,
                                    result,
                                    FFTW_FORWARD,
                                    FFTW_ESTIMATE);
}

fft_circular::~fft_circular()
{
    fftw_destroy_plan(plan);
}

void fft_circular::sample(ifx_Complex_t element)
{
    signal[sample_count][REAL] = element[REAL];
    signal[sample_count][IMAG] = element[IMAG];
    ++sample_count;

    if (sample_count == NUM_FFT_POINTS)
    {
        sample_count %= NUM_FFT_POINTS;

        fftw_execute(plan);

        result_valid = true;
    }
}

fftw_complex fft_circular::get_result()
{
    if (result_valid)
    {
        return result;
    }
    return nullptr;
}
