import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter

def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y

contents = open("cmake-build-debug/pre-fft.txt", "r").read().rstrip().split(' ')

y_vals = [float(i) for i in contents]
x_vals = range(len(y_vals))

plt.plot(x_vals, y_vals, label='un1filtered')

sample_rate = 64
order = 10
cutoff = 2

y_vals = butter_lowpass_filter(y_vals, cutoff, sample_rate, order)

plt.plot(x_vals, y_vals, label='filtered')

plt.legend()
plt.show()
