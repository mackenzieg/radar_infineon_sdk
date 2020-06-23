import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter

import numpy as np

plt.rcParams.update({'font.size': 10})

def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y


order = 10
cutoff = 2

use_filter = False

delta_bin = 8

metrics = open("metrics.txt").read().splitlines()
range_resolution = float(metrics[0].split(": ")[1])
range_interest = float(metrics[1].split(": ")[1])
bin_interest = float(metrics[2].split(": ")[1])
x_vals = np.asarray(metrics[3].split(", "), dtype=np.float64, order='C')

sample_rate = 1/(x_vals[1] - x_vals[0])

fig, plots = plt.subplots(int(delta_bin / 2), 2)

data = open("data.txt").read().splitlines()
i = 0
graph_num = 0
while (i < len(data)):
    curr_bin = int(data[i].split(": ")[1])
    real = np.asarray(data[i + 2].split(", "), dtype=np.float64, order='C')
    imag = np.asarray(data[i + 4].split(", "), dtype=np.float64, order='C')

    if (use_filter == True):
        real = butter_lowpass_filter(real, cutoff, sample_rate, order)
        imag = butter_lowpass_filter(imag, cutoff, sample_rate, order)

    plots[int(graph_num / 2), graph_num % 2].set_title(str(curr_bin * range_resolution) + "m Real")
    plots[int(graph_num / 2), graph_num % 2].plot(x_vals, real)

    #plots[graph_num, 1].set_title(str(curr_bin) + " imag", fontsize=20)
    #plots[graph_num, 1].plot(x_vals, real)

    i += 5
    graph_num += 1

    if (graph_num >= delta_bin):
        break

plt.legend()
plt.show()
