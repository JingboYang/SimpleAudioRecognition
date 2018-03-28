
import matplotlib.pyplot as plt
import librosa as rosa
import numpy as np

with open('temp', 'r') as f:
	content = f.read()

stuff = np.array(eval(content))

stuff2 = stuff[:96000]

#plt.plot(stuff, linewidth=0.1)
#plt.show()


#rosa.output.write_wav("lala.wav", stuff, 32000)

from audio_basics import save_audio

save_audio("lala.raw", stuff)


import ctypes
a = ctypes.c_uint(2 ** 32 - 1)

print(a)