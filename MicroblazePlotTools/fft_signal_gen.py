from __future__ import division

import matplotlib.pyplot as plt
import numpy as np
import math
import pprint as pp

NUM_INPUT = 512             # Number of input points
INPUT_OFFSET = 8000         # "offset" from start of a "cycle"
FREQUENCY = 16000           # sampling rate

# magnitude for 1Kx2pi, 2Kx2pi, 3Kx2pi ....
COMPONENT_POWERS = [0,0,1,1]

array = []
for i in range(NUM_INPUT):
    t = (i + INPUT_OFFSET) / FREQUENCY
    
    val = 0
    for j, c in enumerate(COMPONENT_POWERS):
        val += c * math.sin(math.pi * 2 * (j + 1) * 1000 * t)
    array.append(val)

array = np.array(array)
array = array * (2 ** 16) - 2 ** 15

print ('--- For C Code ---')
output = "["
for i, a in enumerate(array):
    output += str(int(a)).ljust(8) + ' '
    if (i != len(array) - 1):
        output += ', '
    
    if ((i + 1) % 5 == 0):
        output += '\n'
output += ']'
print(output)
print('--- END for C Code')


fft_result = np.fft.rfft(array)

print('---- REAL ----')
pp.pprint(fft_result.real)

print('---- COMPLEX ----')
pp.pprint(fft_result.imag)

print('---- MAG ** 2 ----')
power = np.power(np.absolute(fft_result), 2)
pp.pprint(power)


#### Plotting ####

plt.plot(array, color='b')
plt.show()

plt.plot(fft_result.real, color='g', linestyle='-.')
plt.show()

plt.plot(fft_result.imag, color='y', linestyle=':')
plt.show()

plt.plot(power, color='m', linestyle='--', marker='o', markersize=3)
plt.show()

