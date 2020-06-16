#include "fft_circular.hpp"

#define REAL 0
#define IMAG 1

fft_circular::fft_circular()
{
    signal = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NUM_FFT_POINTS);
    result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NUM_FFT_POINTS);
    plan = fftw_plan_dft_1d(NUM_FFT_POINTS,
                                    signal,
                                    result,
                                    FFTW_FORWARD,
                                    FFTW_ESTIMATE);
}

fft_circular::~fft_circular()
{
    fftw_destroy_plan(plan);

    fftw_free(signal);
    fftw_free(result);
}

void fft_circular::sample(ifx_Complex_t element)
{
    result_valid = false;

    signal[sample_count][REAL] = element.data[REAL];
    signal[sample_count][IMAG] = element.data[IMAG];
    ++sample_count;

    if (sample_count == NUM_FFT_POINTS)
    {
        sample_count %= NUM_FFT_POINTS;

        double avg_real = 0.0;
        double avg_imag = 0.0;
        for (int i = 0; i < NUM_FFT_POINTS; ++i)
        {
            avg_real += signal[i][REAL];
            avg_imag += signal[i][IMAG];
        }
        avg_real /= NUM_FFT_POINTS;
        avg_imag /= NUM_FFT_POINTS;

        for (int i = 0; i < NUM_FFT_POINTS; ++i)
        {
            signal[i][REAL] -= avg_real;
            signal[i][IMAG] -= avg_imag;
        }

        fftw_execute(plan);

        result_valid = true;
    }
}

fftw_complex* fft_circular::get_signal()
{
    return signal;
}

fftw_complex* fft_circular::get_result()
{
    if (result_valid)
    {
        return result;
    }
    return nullptr;
}
