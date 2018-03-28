import processor as mfcc_tool
import audio_divider as vad_tool
import dynamic_time_wrap as dtw_tool
import audio_library as lib_tool
import audio_basics as audio_tool

import os, sys
import pprint as pp


def workflow(filename):

    mfcc_libs = lib_tool.load_libraries()

    target_audio_arr = audio_tool.open_audio(filename)
    speech_ranges, speech_segs = vad_tool.divide_audio(target_audio_arr)
    
    pp.pprint(speech_ranges)

    words = []
    for speech in speech_segs:
        audio_array = audio_tool.normalize_audio(speech)

        #audio_tool.plot_audio([audio_array, []])

        target_mfcc = mfcc_tool.workflow(audio_array)[0]

        distances = []
        for m in mfcc_libs:
            distance = dtw_tool.dtw_workflow(mfcc_libs[m][:, 5:], target_mfcc[:, 5:])
            distances.append((distance, m))

        distances.sort()
        pp.pprint(distances)
        possible_word = distances[0][1].split('_')[0]

        words.append(possible_word)

    phrase = ' '.join(words)
    return phrase


if __name__ == '__main__':
    filename = sys.argv[1]

    phrase = workflow(filename)

    print('Phrase is ')
    print(phrase)