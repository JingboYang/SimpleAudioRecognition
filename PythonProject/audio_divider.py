from __future__ import division
import struct
import math
import numpy as np
import matplotlib.pyplot as plt


# Apparently need 
BYTE_PER_SAMPLE = 2                         # 16 bits signal
SAMPLE_RATE = 16000                         # 16K sampling rate
BYTE_RATE = SAMPLE_RATE * BYTE_PER_SAMPLE   # byte rate

VAD_CHUNK = SAMPLE_RATE * 32 / 1000


def open_audio(filename):

    with open(filename, 'rb') as f:
        arr = f.read()
    
    result = []
    for i in range(0, len(arr), BYTE_PER_SAMPLE):
        unpacked = struct.unpack('<h', arr[i:i+BYTE_PER_SAMPLE])
        result.append(unpacked)

        #print(unpacked)

    #plt.plot(result, linewidth=0.1)
    #plt.show()

    result = np.array(result)

    print('length of audio is ' + str(len(result)))

    return result


def rms(chunk):

    s = 0.0
    for c in chunk:
        s += c * c
    
    #print(s)
    result = math.sqrt(s / len(chunk))
    return result


def divide_audio(arr):

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
        if rms_power > 1500:
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
                if non_speech_count > 4:
                    is_speech = False
                    speech_ranges.append((speech_start, speech_end))
                    
                    print('-----')
                    print(speech_start)
                    print(speech_start / SAMPLE_RATE)
                    print(speech_end)
                    print(speech_end / SAMPLE_RATE)
                    print('----')
                    
    if non_speech_count <= 4:
        speech_ranges.append((speech_start, end_index))

    speech_segs = []
    for s in speech_ranges:
        speech_segs.append(np.array(arr[s[0]:s[1]]))

    return speech_ranges, speech_segs


if __name__ == '__main__':
    #audio_arr = open_audio('audio_lib/phrase/open_the_door6.raw')
    audio_arr = open_audio('audio_lib/phrase/open_door_4.raw')
    
    speech_ranges, speech_segs = divide_audio(audio_arr)

    plt.plot(audio_arr, linewidth=0.2)
    print(speech_ranges)
    for s in speech_ranges:
        plt.axvspan(s[0], s[1], facecolor='#2ca02c', alpha=0.5)

    plt.show()