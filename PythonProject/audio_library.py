import processor as mfcc_tool
import audio_basics as audio_tool
import audio_divider as vad_tool

import pprint as pp
import numpy as np
import os

from c_arr_printer import generate_c_lib


LIBRARY_PATH = 'audio_lib'

GROUPS = [['open', 'close', 'kai', 'guan', 'enter', 'light'], 
                ['deng', 'door', 'on', 'off']]

WORD_VALUE = {'open': '001', 'close': '002',
                'kai': '003', 'guan': '004',
                'enter': '005', 'light': '008',
                'door': '006', 'deng': '007',
                'on': '009', 'off': '00A'}


def get_name(filename):
    base = os.path.basename(filename)
    name = os.path.splitext(base)[0]

    return name
    

def get_ext(filename, count=1):
    base = os.path.basename(filename)

    if count == 1:
        exts = os.path.splitext(base)[-1]
    else:
        exts = os.path.splitext(base)[:-count]

    return exts


def get_word(string):
    w = string.strip().split('_')[0]
    #print(w)
    return w


def generate_one_library(filename, do_plotting=False):

    audio_array = audio_tool.open_audio(filename)
    
    for i in range(4000):
        audio_array = np.append(audio_array, 0)
        audio_array = np.insert(audio_array, 0, 0)

    speech_ranges, speech_segs = vad_tool.divide_audio(audio_array)

    print('Speech range: ' + str(speech_ranges) + ' Total Length: ' + str(len(audio_array)))
    if (len(speech_ranges)) != 1 or do_plotting:
        audio_tool.plot_audio([audio_array, speech_segs[0]])

    assert len(speech_ranges) == 1

    audio_array = speech_segs[0]
    audio_array = audio_tool.normalize_audio(audio_array)

    all_mfcc_scaled, all_mfcc_no_scale, audio_arr = mfcc_tool.workflow(audio_array)

    save_path = os.path.join(LIBRARY_PATH, get_name(filename) + '.mfcc')
    print('saving ' + save_path)
    np.savetxt(save_path, all_mfcc_scaled, delimiter=',')

    return all_mfcc_scaled


def generate_libraries(jupyter=False):

    if jupyter:
        global LIBRARY_PATH
        LIBRARY_PATH = os.path.join('..', 'audio_lib')
    onlyfiles = [f for f in os.listdir(LIBRARY_PATH) if os.path.isfile(os.path.join(LIBRARY_PATH, f))]

    mfcc_map = {}
    for f in onlyfiles:
        if get_ext(f) == '.raw':
            print('Generating ' + str(f))
            mfcc_map[get_name(f)] = generate_one_library(os.path.join(LIBRARY_PATH, f))
            #print(mfcc_map[get_name(f)])
    return mfcc_map



def load_one_library(filename):
    load_path = os.path.join(LIBRARY_PATH, get_name(filename) + '.mfcc')
    print('loading ' + load_path)
    all_mfcc_scaled = np.loadtxt(open(load_path, 'r'), delimiter=',')

    return all_mfcc_scaled


def load_libraries(jupyter=False):

    if jupyter:
        global LIBRARY_PATH
        LIBRARY_PATH = os.path.join('..', 'audio_lib')

    onlyfiles = [f for f in os.listdir(LIBRARY_PATH) if os.path.isfile(os.path.join(LIBRARY_PATH, f))]

    mfcc_map = {}
    for f in onlyfiles:
        if get_ext(f) == '.mfcc':
            
            all_mfcc_scaled = load_one_library(os.path.join(LIBRARY_PATH, f))
            #print(all_mfcc_scaled.shape)
            mfcc_map[get_name(f)] = all_mfcc_scaled
            
    return mfcc_map



def is_in_group(w1, w2):

    for g in GROUPS:
        if get_word(w1) in g:
            return (get_word(w2) in g)



def compare_libraries(in_group=True):
    mfcc_map = load_libraries()

    #for m in mfcc_map:
    #    pp.pprint(mfcc_map[m])
   
    import dynamic_time_wrap as dtw

    distance_map = {}
    for m1 in mfcc_map:
        print('Working on ' + m1)
        distance_map[m1] = {}
        for m2 in mfcc_map:
            distance = dtw.dtw_workflow(mfcc_map[m1][:, 3:], mfcc_map[m2][:, 3:])
            distance_map[m1][m2] = distance
    
    map_keys = list(distance_map.keys())
    map_keys.sort()

    for m in map_keys:
        s = [(k, distance_map[m][k]) for k in sorted(distance_map[m], key=distance_map[m].get, reverse=False)]
        #print('For ' + m)
        #pp.pprint(s)

        closest = -1
        good_count = 0
        ratio = 0
        dist = 0
        next_one = None
        very_bad = False

        useful_count = 0
        top_choices = []
        for i, (k, v) in enumerate(s):
            
            if (is_in_group(m, k)):
                useful_count += 1

                if (len(top_choices) < 6):
                    top_choices.append(k)
                #print(k, v)
                if useful_count == 2:
                    if get_word(k) != get_word(m):
                        print('Word ' + m + ' is very bad!')
                        print('Sounds similar to ' + k + ', with length ' + str(len(mfcc_map[k])) \
                                + '. Ref length is ' + str(len(mfcc_map[m]))\
                                )
                        #print(k, v)
                        very_bad = True
                        break
                    else:
                        closest = v
                        good_count += 1
                    #compare_mfcc(mfcc_map[k], mfcc_map[m])
                elif useful_count > 2:
                    if get_word(k) != get_word(m):
                        ratio = round((v / closest - 1) * 100, 2)
                        next_one = k
                        dist = v
                        break
                    else:
                        good_count += 1


        printout = 'Ref: ' + m.ljust(8) + '(' + str(len(mfcc_map[m])).ljust(2) + ')'
        printout += '\tGood Count: ' + str(good_count).ljust(2)
        if next_one is not None:
            printout += '\tNext One: ' + str(next_one).ljust(8) + '(' + str(len(mfcc_map[next_one])).ljust(2) + ')'
        printout += '\tDiff Ratio: ' + str(ratio).ljust(8)
        printout += '\tNext Dist: ' + str(dist).ljust(8)
        printout += '\tTop Choices: ' + str(top_choices)
        print(printout)

        if very_bad:
            pp.pprint(s)


def compare_libraries_top(in_group=True):
    mfcc_map = load_libraries()

    #for m in mfcc_map:
    #    pp.pprint(mfcc_map[m])
   
    import dynamic_time_wrap as dtw

    distance_map = {}
    for m1 in mfcc_map:
        print('Working on ' + m1)
        distance_map[m1] = {}
        for m2 in mfcc_map:
            distance = dtw.dtw_workflow(mfcc_map[m1][:, 3:16], mfcc_map[m2][:, 3:16])
            distance_map[m1][m2] = distance
    
    map_keys = list(distance_map.keys())
    map_keys.sort()

    print('Total number of samples: ' + str(len(map_keys)))
    incorrect = 0
    for m in map_keys:
        s = [(k, distance_map[m][k]) for k in sorted(distance_map[m], key=distance_map[m].get, reverse=False)]

        useful_count = 0
        top_choices = []
        for i, (k, v) in enumerate(s):
            
            if (is_in_group(m, k)):
                useful_count += 1

                if (len(top_choices) < 2 and useful_count != 1):
                    top_choices.append(k)

        votes = {}
        for t in top_choices:               
            w = get_word(t) 
            if w in votes:
                votes[w] += 1
            else:
                votes[w] = 1

        sorted_votes = [(v, votes[v]) for v in sorted(votes, key=votes.get, reverse=True)]
        printout = 'Ref: ' + m.ljust(8)
        printout += '\tBest Fit: ' + str(sorted_votes[0])
        printout += '\tTop Choices: ' + str(top_choices)
        print(printout)

        if sorted_votes[0][0] != get_word(m):
            incorrect += 1
    
    print('Total incorrect: ' + str(incorrect))


def compare_mfcc(mfcc_orig1, mfcc_orig2):
    import matplotlib.pyplot as plt
    import dynamic_time_wrap as dtw

    distance = dtw.dtw_workflow(mfcc_orig1[:, 3:], mfcc_orig2[:, 3:], True)
    for i in range(len(mfcc_orig1)):
        break
        print(list(mfcc_orig1[i]))
        print(list(mfcc_orig2[i]))
        print((np.linalg.norm(mfcc_orig1[i]), np.linalg.norm(mfcc_orig2[i])))
        print('--')

    mfcc1 = np.transpose(mfcc_orig1)
    mfcc2 = np.transpose(mfcc_orig2)
    assert len(mfcc1) == len(mfcc2)
    
    f, axs = plt.subplots(int((len(mfcc1) + 1) / 2), 2, sharey=True)

    for i in range(len(mfcc1)):
        ax = axs[int(i / 2)][i % 2]

        ax.plot(mfcc1[i])
        ax.plot(mfcc2[i])

    plt.show()


def view_word(w):
    import matplotlib.pyplot as plt

    onlyfiles = [f for f in os.listdir(LIBRARY_PATH) if os.path.isfile(os.path.join(LIBRARY_PATH, f))]

    audios = []
    longest = 0
    fnames= []
    for f in onlyfiles:
        #print(f)
        if get_ext(f) == '.raw' and get_word(get_name(f)) == w:
            audio_array = audio_tool.open_audio(os.path.join(LIBRARY_PATH, f))
            audios.append(audio_array)
            longest = max(longest, len(audio_array))
            fnames.append(f)

    f, axs = plt.subplots(len(audios), 1, sharey=True)

    for i, a in enumerate(audios):
        ax = axs[i]

        ax.plot(audios[i])
        ax.set_xlim((0, longest))
        ax.set_title(fnames[i])
    
    plt.show()


if __name__ == '__main__':
    
    #view_word('open')
    #view_word('enter')
    #view_word('guan')
    #view_word('kai')

    generate_libraries()
    mfcc_map = load_libraries()

    #np.set_printoptions(threshold=np.nan)

    generate_c_lib(mfcc_map)

    #compare_libraries()
    #compare_libraries_top()

    #compare_mfcc(mfcc_map['close_1'], mfcc_map['guan_5'])

    #generate_one_library(os.path.join(LIBRARY_PATH, 'open_1.raw'), True)

    '''
    import dynamic_time_wrap as dtw
    dist = dtw.dtw_workflow(mfcc_map['open_4'], mfcc_map['light_1'], True)
    print('Dist: ' + str(dist))
    dist = dtw.dtw_workflow(mfcc_map['open_4'], mfcc_map['enter_3'], True)
    print('Dist: ' + str(dist))
    dist = dtw.dtw_workflow(mfcc_map['open_4'], mfcc_map['open_3'], True)
    print('Dist: ' + str(dist))
    '''
    exit()