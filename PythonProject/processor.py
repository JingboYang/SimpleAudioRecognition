from __future__ import division
import struct
import math
import matplotlib.pyplot as plt
import numpy as np                  # use numpy functions as little as possible (can use when we know it can 
                                    # be done conveninently on FPGA)
import pprint as pp

import librosa as rosa              # library to compare against

from custom_timer import Timer      # for performance evaluation


# http://haythamfayek.com/2016/04/21/speech-processing-for-machine-learning.html
# http://mirlab.org/jang/books/audioSignalProcessing/speechFeatureMfcc.asp?title=12-2%20MFCC                                    

# Apparently need 
BYTE_PER_SAMPLE = 2                         # 16 bits signal
SAMPLE_RATE = 16000                         # 16K sampling rate
BYTE_RATE = SAMPLE_RATE * BYTE_PER_SAMPLE   # byte rate

SHIFT_WINDOW = int(float(16) / 1000 * SAMPLE_RATE)   # shift at 2^4 for faster computation
WINDOW_SIZE = int(float(32) / 1000 * SAMPLE_RATE)    # window size also power of 2 for faster computation

PRE_EMPH_COEF = 0.97

HANNING_COEF = np.hanning(WINDOW_SIZE)
NUM_MEL_FILTERS = 22
NUM_MFCC = 22                               # this should be the same as one above
MEL_FILTER_BANK = []
DCT_COEF = []

LIFT = 1 + (NUM_MFCC / 2) * np.sin(np.pi * np.arange(NUM_MFCC) / NUM_MFCC)

# mel scale
# m = 2595 * log_10( 1 + f/700)
# we simply record these values and input them to FPGA
def generate_filter_bank():
    
    global MEL_FILTER_BANK

    low_freq_mel = 0
    high_freq_mel = (2595 * np.log10(1 + (SAMPLE_RATE / 2) / 700))  # Convert Hz to Mel
    mel_points = np.linspace(low_freq_mel, high_freq_mel, NUM_MEL_FILTERS + 2)  # Equally spaced in Mel scale
    hz_points = (700 * (10**(mel_points / 2595) - 1))  # Convert Mel to Hz
    bin = np.floor((WINDOW_SIZE + 1) * hz_points / SAMPLE_RATE)

    fbank = np.zeros((NUM_MEL_FILTERS, int(np.floor(WINDOW_SIZE / 2 + 1))))
    for m in range(1, NUM_MEL_FILTERS + 1):
        f_m_minus = int(bin[m - 1])   # left
        f_m = int(bin[m])             # center
        f_m_plus = int(bin[m + 1])    # right

        for k in range(f_m_minus, f_m):
            fbank[m - 1, k] = (k - bin[m - 1]) / (bin[m] - bin[m - 1])
        for k in range(f_m, f_m_plus):
            fbank[m - 1, k] = (bin[m + 1] - k) / (bin[m + 1] - bin[m])

    MEL_FILTER_BANK = np.array(fbank)
    return MEL_FILTER_BANK
    #print('shape of mel filter bank: ' + str(MEL_FILTER_BANK.shape))

    #print(MEL_FILTER_BANK)


def generate_dct_coeffs():
    
    global DCT_COEF

    coeffs = []
    for k in range(NUM_MFCC):
        coeff_temp = []
        for n in range(NUM_MFCC):
            temp = math.cos(math.pi / NUM_MFCC * (n + 0.5) * k)
            coeff_temp.append(temp)
        coeffs.append(coeff_temp)
    
    DCT_COEF = np.array(coeffs)
    print('shape of DCT coefficients: ' + str(DCT_COEF.shape))
    return DCT_COEF


def pre_emphasis(audio_chunk, coef=None):
    
    result = [audio_chunk[0]]

    if coef is None:
        pre_emph_coef = PRE_EMPH_COEF
    else:
        pre_emph_coef = coef

    for i in range(1, len(audio_chunk)):
        result.append(audio_chunk[i] - pre_emph_coef * result[-1])

    result = np.array(result).flatten()

    #print('after emphasis: ' + str(result.shape))

    return result


# https://docs.scipy.org/doc/numpy-1.13.0/reference/generated/numpy.hanning.html
# w[n] = 0.5 - 0.5cos(2*pi*n/(M-1)) for n in [0,M)
def hanning_window(audio_chunk):
    result = HANNING_COEF * audio_chunk

    #print('hanning coef: ' + str(HANNING_COEF.shape))
    #print('orig audio: ' + str(audio_chunk.shape))
    #print('after hanning: ' + str(result.shape))

    return result


def compute_fft(processed_audio):

    coeffs = np.fft.rfft(processed_audio, WINDOW_SIZE)
    #print('audio: ' + str(processed_audio.shape))
    #print('coeffs: ' + str(coeffs.shape))
    mags = np.absolute(coeffs)
    powers = (1.0 / WINDOW_SIZE) * (mags ** 2)

    '''
    print(fftfreq(snippet_fft))

    fft_results.append(snippet_fft[:40])        # we don't care about so many FFT terms

    plt.plot(snippet_fft, linewidth=0.1)
    plt.show()
    '''

    return powers

def process_mel(powers):
    
    results = np.dot(powers, np.transpose(MEL_FILTER_BANK))
    
    #print(results)

    #print('powers: ' + str(powers.shape))
    #print('mel bank: ' + str(MEL_FILTER_BANK.shape))
    #print('results: ' + str(len(results)))
    # apply some numerical stability stuff that we cannot do in FPGA
    results = 20 * np.log10(results)                    # x20 to convert to decibel notation

    #print(results)
    return results


def dct(values):

    #print('before dct: ' + str(values.shape))

    '''
    results = []
    for k in range(len(values)):
        s = 0
        for n in range(len(values)):
            print(str((k,n)))
            temp = values[n] * DCT_COEF[k][n]
            s += temp
        
        results.append(s)
    '''

    results = np.matmul(values, np.transpose(DCT_COEF))
    #print('after dct: ' + str(results.shape))

    return results


def re_scaling(mfcc):
    
    mfcc_final = mfcc * LIFT

    #mfcc_final = mfcc

    #pp.pprint(mfcc_final)
    #raw_input("Press Enter to continue...")

    return mfcc_final


def workflow(audio_arr):
    
    #print('==> Generating mel filter bank')
    with Timer('generate_filter_bank') as timer:
        generate_filter_bank()
    
    #print('==> Generating DCT coefficients')
    with Timer('generate_dct_coefficients') as timer:
        generate_dct_coeffs()

    '''
    print('==> Loading audio')
    with Timer('open_audio') as timer:
        audio_arr = open_audio(filename)
    '''

    max_ratio = 1.0
    last = int(len(audio_arr) * max_ratio)

    num_windows = int((last - WINDOW_SIZE))
    last = int(len(audio_arr) * max_ratio)

    num_windows = int((last - WINDOW_SIZE) / SHIFT_WINDOW) + 1
    print('Total of ' + str(num_windows) + ' windows')

    times = {   'pre_emphasis': 0,
                'hanning': 0,
                'fft': 0,
                'mel': 0,
                'dct': 0,
                'rescaling': 0}

    all_mfcc_scaled = []
    all_mfcc_no_scale = []
    portion = 0.25
    end_point = -1
    for i in range(0, last - SHIFT_WINDOW, SHIFT_WINDOW):
        if float(i) / last > portion:
            print('Processed ' + str(round(float(i) / last * 100, 2)) + '%')
            portion += 0.1

        # forget about the last little bit if it doesn't fit
        if i + WINDOW_SIZE > last:
            break

        cur_audio = audio_arr[i : i + WINDOW_SIZE]
        end_point = i + WINDOW_SIZE

        #pp.pprint(cur_audio)
        #raw_input("Press Enter to continue...")

        with Timer('pre_emphasis', False) as timer:
            pre_emph_audio = pre_emphasis(cur_audio)
            #print(len(pre_emph_audio))
            times['pre_emphasis'] += timer.so_far()

        with Timer('pre_emphasis', False) as timer:
            processed_audio = hanning_window(pre_emph_audio)
            #print(len(processed_audio))
            #print(processed_audio)
            #exit()
            times['hanning'] += timer.so_far()

        with Timer('fft', False) as timer:
            fft_powers = compute_fft(processed_audio)
            #fft_powers = compute_fft(np.array(cur_audio).flatten())             # for no pre-processing
            #print(len(fft_powers))
            #print(fft_powers)
            times['fft'] += timer.so_far()

        with Timer('mel', False) as timer:
            processed_mel = process_mel(fft_powers)
            #print(len(processed_mel))
            #print(processed_mel)
            times['mel'] += timer.so_far()

        with Timer('dct', False) as timer:
            mfcc = dct(processed_mel)
            #print(len(mfcc))
            #print(mfcc)
            times['dct'] += timer.so_far()

        with Timer('rescaling', False) as timer:
            final_mfcc = re_scaling(mfcc)
            #final_mfcc = mfcc
            #final_mfcc = mfcc                                                   # no re-scalling
            #print(len(final_mfcc))
            #print(final_mfcc)
            times['rescaling'] += timer.so_far()

        #break
        all_mfcc_scaled.append(final_mfcc)
        all_mfcc_no_scale.append(mfcc)

    print('==> Done')

    #pp.pprint(times)

    all_mfcc_scaled = np.array(all_mfcc_scaled)
    #print(all_mfcc_scaled.shape)
    assert all_mfcc_scaled.shape == (num_windows, NUM_MFCC)

    all_mfcc_no_scale = np.array(all_mfcc_no_scale)
    #print(all_mfcc_no_scale.shape)
    assert all_mfcc_no_scale.shape == (num_windows, NUM_MFCC)

    return all_mfcc_scaled, all_mfcc_no_scale, audio_arr[:end_point]


def compute_using_librosa(audio):
    mfcc = rosa.feature.mfcc(y=audio.flatten(), sr=SAMPLE_RATE, n_mfcc=NUM_MFCC, hop_length=SHIFT_WINDOW, n_fft=WINDOW_SIZE)
    mfcc = np.transpose(mfcc)           # seems like librosa have it the other way for plotting purpose

    last = int((len(audio) - WINDOW_SIZE) / SHIFT_WINDOW) + 1   # to align with our engine result. throw out last few
    mfcc = mfcc[:last]

    print('librosa mfcc shape ' + str(mfcc.shape))
    return mfcc


def im_mfcc(mfcc, ax):
    ax.set_xlim(0, len(mfcc))
    plt.imshow((np.transpose(mfcc) / np.linalg.norm(mfcc)), interpolation='spline16',aspect='auto', cmap=plt.get_cmap('BrBG'))
    #plt.clim(-0.075, 0.1)


def plot_mfcc(title, all_mfcc_scaled, all_mfcc_no_scale, rosa_mfcc, audio):
    
    #print('shape of all mfcc is ' + str(all_mfcc.shape))

    fig = plt.figure()

    ax = fig.add_subplot(411)
    ax.set_xlim(0,len(audio))
    ax.plot(audio, linewidth=0.75)
    ax.set_title(title)

    ax = fig.add_subplot(412)
    im_mfcc(all_mfcc_scaled, ax)
    ax.set_ylabel('MFCC (scaled)')

    ax = fig.add_subplot(413)
    im_mfcc(all_mfcc_no_scale, ax)
    ax.set_ylabel('MFCC (not scaled)')
    
    ax = fig.add_subplot(414)
    im_mfcc(rosa_mfcc, ax)
    ax.set_ylabel('MFCC from librosa')

    #plt.colorbar(orientation='horizontal', pad=0.2)
    
    plt.show()
    

if __name__ == '__main__':
       
    np.set_printoptions(threshold=np.nan)
    generate_filter_bank()
    generate_dct_coeffs()
    
    exit()

    c = [None, None, None, None]
    h = [None, None, None, None]
    c[0], h[0] = print_2d_c_arr_as_1d(MEL_FILTER_BANK, 'MEL_FILTER_BANK')
    c[1], h[1] = print_2d_c_arr_as_1d(DCT_COEF, 'DCT_COEF')
    c[2], h[2] = print_1d_c_arr(np.hanning(WINDOW_SIZE), 'HANNING_COEF')
    c[3], h[3] = print_1d_c_arr(LIFT, 'LIFT')


    for t in c:
        print(t)
        print('')

    for t in h:
        print(t)
        print('')

    
    exit()
    
    filename = 'open_the_door.raw'
    title = filename.split('.')[0].replace('_', ' ')
    title = 'Audio and MFCC for Command \"' + title + '\"'

    all_mfcc_scaled, all_mfcc_no_scale, audio = workflow(filename)
    librosa_mfcc = compute_using_librosa(audio)
    plot_mfcc(title, all_mfcc_scaled, all_mfcc_no_scale, librosa_mfcc, audio)
    
