from __future__ import division

import numpy as np
import math, os
import matplotlib.pyplot as plt
from scipy.stats import chisquare
import pprint as pp

from processor import workflow
from audio_basics import *
from audio_library import *
import dynamic_time_wrap as dtw


def get_del(array):
    arr1 = array[0:-2]
    arr2 = array[1:-1]

    result = arr2 - arr1

    return result


def plot_audios(audio1, audio2):
    
    plt.subplot(211)
    plt.plot(audio1)

    plt.subplot(212)
    plt.plot(audio2)

    plt.show()


# cosine similarity
# treat 
def compare_audios(f1, f2):
    mfcc1, _ , audio1 = workflow(open_audio(f1))
    mfcc2, _ , audio2 = workflow(open_audio(f2))

    m1 = mfcc1.flatten()
    m2 = mfcc2.flatten()

    m1 = m1.reshape(len(m1), 1)
    m2 = m2.reshape(len(m2), 1)

    #m1 = mfcc1
    #m2 = mfcc2

    distance= dtw.dtw_workflow(m1, m2)
    '''
    for i in range(mfcc1.shape[1]):

        v1 = mfcc1[:, i].reshape(len(mfcc1), 1)
        v2 = mfcc2[:, i].reshape(len(mfcc2), 1)
        distance = dtw.dtw_workflow(v1, v2)

        print ('Distance between two audio is '  + str(distance))
    
    plot_audios(audio1, audio2)
    '''


    return distance


if __name__ == '__main__':
    
    #f1 = 'open_the_door.raw'
    #f2 = 'close_the_door.raw'
    #f2 = 'open_the_door3.raw'

    #f1 = 'close_the_door.raw'
    #f2 = 'open_the_door3.raw'

    f1 = "audio_lib/open_1.raw"
    f2 = "audio_lib/open_4.raw"

    report = []
    onlyfiles = [f for f in os.listdir('audio_lib') if os.path.isfile(os.path.join('audio_lib', f))]
    for f1 in onlyfiles:
        report_temp = []
        for f2 in onlyfiles:
            if get_ext(f1) == '.raw' and get_ext(f2) == '.raw':
                f_1 = 'audio_lib' + '/' + f1
                f_2 = 'audio_lib' + '/' + f2

                #print(f_1)
                #print(f_2)
                #print(get_name(f_1) + ' => ' + get_name(f_2))
                val = compare_audios(f_1, f_2)
                #print('----')

                report_temp.append((val, f_1, f_2))
        
        report_temp.sort()
        report.append(report_temp)

    report.sort()
    pp.pprint(report)
