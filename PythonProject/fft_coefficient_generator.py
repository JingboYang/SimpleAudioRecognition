import numpy as np
import pprint as pp

from processor import print_2d_c_arr_as_1d
from processor import print_2d_c_arr
from processor import print_1d_c_arr

MAX_K = 257
MAX_X = 512

n_vals = np.arange(0, MAX_X)

all_cos_vals = np.zeros((MAX_K, MAX_X))
all_sin_vals = np.zeros((MAX_K, MAX_X))


for k in np.arange(0, MAX_K):
    cos_vals = np.cos(2*np.pi*k*n_vals/MAX_X)
    all_cos_vals[k] = cos_vals

    sin_vals = np.sin(2*np.pi*k*n_vals/MAX_X)
    all_sin_vals[k] = sin_vals

#pp.pprint(np.power(all_cos_vals, 2) + np.power(all_sin_vals, 2))

# now do some random test

x_values = np.random.rand(MAX_X) * 100

np_fft = np.fft.rfft(x_values, MAX_X)

#pp.pprint(np_fft.real)

### according to wikipedia ###

real_part = []
comp_part = []

for k in range(MAX_K):
    
    r = 0
    c = 0
    for n in range(MAX_X):
        r += x_values[n] * all_cos_vals[k][n]
        c -= x_values[n] * all_sin_vals[k][n]

    real_part.append(r)
    comp_part.append(c)

real_part = np.matmul(x_values, np.transpose(all_cos_vals))

real_part = np.array(real_part)
comp_part = np.array(comp_part)

np_power = np.sqrt(np.power(np_fft.real, 2) + np.power(np_fft.imag, 2))
custom_power = np.sqrt(np.power(real_part, 2) + np.power(comp_part, 2))

'''

print('difference is ')
diff = custom_power - np_power
pp.pprint(diff)
pp.pprint(custom_power)

print ('ratio is ')
ratio = diff / np_power
#pp.pprint(ratio)

print('for example')
print(custom_power[128])
print(ratio[128])

print(custom_power[-1])
print(ratio[-1])

print(custom_power[0])
print(ratio[0])


#pp.pprint(real_part - np_fft.real)
#pp.pprint(comp_part - np_fft.imag)

exit()
'''

c = [None, None, None]
h = [None, None, None]

c[0], h[0] = print_1d_c_arr(x_values, 'x')
c[1], h[1] = print_2d_c_arr_as_1d(all_cos_vals, 'COS_COEFFS')
c[2], h[2] = print_2d_c_arr_as_1d(all_sin_vals, 'SIN_COEFFS')

for t in c:
    print(t)
    print('')

for t in h:
    print(t)
    print('')