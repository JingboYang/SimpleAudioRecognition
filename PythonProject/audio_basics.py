import struct
import numpy as np

# Apparently need 
BYTE_PER_SAMPLE = 2                         # 16 bits signal
SAMPLE_RATE = 16000                         # 16K sampling rate
BYTE_RATE = SAMPLE_RATE * BYTE_PER_SAMPLE   # byte rate

# returns audio in integer array, not bytes
def open_audio(filename):

    with open(filename, 'rb') as f:
        arr = f.read()
    
    result = []
    for i in range(0, len(arr), BYTE_PER_SAMPLE):
        unpacked = struct.unpack('<h', arr[i:i+BYTE_PER_SAMPLE])
        
        result.append(unpacked)

    #plt.plot(result, linewidth=0.1)
    #plt.show()

    result = np.array(result).flatten()
    print(result.shape)
    print('length of audio is ' + str(len(result)))

    return result


def save_audio(filename, content):

    with open(filename, 'wb') as f:

        for i in range(0, len(content)):
            #print(content[i])
            unpacked = struct.pack('<h', content[i])
            f.write(unpacked)


def normalize_audio(audio_arr):

    max_val = 2 ** 15 * 0.95
    
    highest = max(audio_arr)
    lowest = abs(min(audio_arr))
    ratio = max_val / max(highest, lowest)

    audio_arr = audio_arr * ratio
    
    print('Ratio: ' + str(ratio))
    return audio_arr


def plot_audio(audio_arrs, jupyter=False):
    import matplotlib.pyplot as plt
    import numbers
    if isinstance(audio_arrs[0], numbers.Number):
        print('audio_arrs should be a list. If there is only 1 audio, use [audio_values]')
        exit(-1)

    if len(audio_arrs) > 1:
        f, axs = plt.subplots(len(audio_arrs), 1, sharey=True)
        longest = 0
        for a in audio_arrs:
            longest = max(longest, len(a))

        for i, a in enumerate(audio_arrs):
            ax = axs[i]

            ax.plot(a, linewidth=0.2)
            ax.set_xlim((0, longest))
            ax.set_ylim(-2**15, 2**15)
    else:
        f, ax = plt.subplots(len(audio_arrs), 1, sharey=True)
        ax.plot(audio_arrs[0],  linewidth=0.2)
        ax.set_ylim(-2**15, 2**15)

    if not jupyter:
        plt.show()

    

if __name__ == '__main__':

    audio = open_audio('audio_lib/sequences/open_door_0.raw')
    
    plot_audio(audio)
    
    exit()

    from c_arr_printer import *
    c, h = print_1d_c_arr(np.array(audio).flatten(), 'test_audio', 'int')

    print (c)
    print (' ')
    print(h)


    #plot_audio([audio, []])

    '''
    from processor import print_1d_c_arr

    length = 5
    sample_rate = 16000
    total_nums = length * sample_rate

    tile_num = int(total_nums / len(audio)) + 1
    #audio = np.tile(audio, tile_num)

    audio = audio.flatten()[:max(total_nums, len(audio))]

    for i in range(len(audio), total_nums):
        audio = np.append(audio, 0)


    audio = audio.astype(np.uint32)
    audio = (audio + 2 ** 15)

    print(max(audio))
    print(min(audio))
    print(len(audio))
    #assert len(audio) == total_nums

    import matplotlib.pyplot as plt
    plt.plot(audio)
    plt.show()


    c, h = print_1d_c_arr(audio, 'test_audio', 'int', per_line=16, ljust_size=8)

    print (c)
    print (' ')
    print(h)
    '''