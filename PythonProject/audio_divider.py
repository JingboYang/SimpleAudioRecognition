from __future__ import division
import struct
import math
import numpy as np


# Apparently need 
BYTE_PER_SAMPLE = 2                         # 16 bits signal
SAMPLE_RATE = 16000                         # 16K sampling rate
BYTE_RATE = SAMPLE_RATE * BYTE_PER_SAMPLE   # byte rate

VAD_CHUNK = SAMPLE_RATE * 32 / 1000


def rms(chunk):

    s = 0.0
    for c in chunk:
        s += c * c
    
    #print(s)
    result = math.sqrt(s / len(chunk))
    return result


def divide_audio(arr, threshold=1500, silence_count=4):

    num_iters = int(len(arr) / VAD_CHUNK)

    non_speech_count = 0
    is_speech = False
    speech_start = float('inf')
    speech_end = float('inf')

    speech_ranges = []

    for i in range(num_iters):
        #print(i)
        start_index = int(i * VAD_CHUNK)
        end_index = int((i + 1) * VAD_CHUNK)

        rms_power = rms(arr[start_index:end_index])
        #print(rms_power * rms_power)
        #if rms_power > 400:
        if rms_power > threshold:
            non_speech_count = 0
            if is_speech == False:
                speech_start = start_index
                speech_end = end_index
                is_speech = True
            else:
                speech_end = end_index
        else:
            if is_speech == False:
                pass
            else:
                non_speech_count += 1
                if non_speech_count > silence_count:
                    is_speech = False
                    speech_ranges.append((speech_start, speech_end))
                    
                    print('-----')
                    print(speech_start, speech_start / SAMPLE_RATE)
                    print(speech_end, speech_end / SAMPLE_RATE)
                    print('----')
                    
    if non_speech_count <= silence_count:
        speech_ranges.append((speech_start, end_index))

    speech_segs = []
    for s in speech_ranges:
        speech_segs.append(np.array(arr[s[0]:s[1]]))

    return speech_ranges, speech_segs


def plot_vad(audio_arr, speech_ranges, jupyter=True):

    import matplotlib.pyplot as plt

    plt.plot(audio_arr, linewidth=0.2)
    
    for s in speech_ranges:
        plt.axvspan(s[0], s[1], facecolor='#2ca02c', alpha=0.5)

    if not jupyter:
        plt.show()


if __name__ == '__main__':
    #audio_arr = open_audio('audio_lib/phrase/open_the_door6.raw')
    audio_arr = open_audio('audio_lib/phrase/open_door_4.raw')
    
    speech_ranges, speech_segs = divide_audio(audio_arr)

    plot_vad(audio_arr, speech_ranges)