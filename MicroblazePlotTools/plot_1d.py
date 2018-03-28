import os, sys
import matplotlib.pyplot as plt
import numpy as np

import struct
import librosa

from codec_convert import *


def save_audio(filename, content):

    with open(filename, 'wb') as f:

        for i in range(0, len(content)):
            #print(content[i])
            unpacked = struct.pack('<h', content[i])
            f.write(unpacked)



with open(os.path.join('.', sys.argv[1]), 'r') as f:
    content = f.read()
    
arr = np.array(eval(content))

stuff = []
uncapped = []
for a in arr:
    pass
    #if a > 65536:
    #    a = 0
    #stuff.append(convert_3(a))

    #temp = a / 32
    temp = a
    uncapped.append(temp)
    if temp > 32767:
        temp = 32767
    elif temp < -32767:
        temp = - 32767
        
    stuff.append(int(temp))
    #print(stuff[-1])


print(max(stuff))
print(max(uncapped))
print(sum(np.abs(stuff)) / len(stuff))

save_name = sys.argv[2] + '.raw'
save_audio(save_name, stuff)

plt.plot(stuff, linewidth=0.1)
plt.ylim(-2 ** 15, 2 ** 15)
plt.show()
