#include <stdio.h>
#include <float.h>

#include "MFCC.h"
#include "audio_divider.h"
#include "stored_sounds.h"
#include "dtw.h"
#include "debugger.h"
#include "do_matching.h"

// we should not operate on the original array, so we have to copy things out
int* select_int_segment(int* original, int start_index, int end_index){
    int* new_audio_arr = (int*)malloc((end_index - start_index) * sizeof(int));
    memcpy(new_audio_arr, &original[start_index], (end_index - start_index) * sizeof(int));
    return new_audio_arr;
}

unsigned int matching_workflow(int* audio_arr, int audio_len, XAxiDma AxiDmaFFT, XAxiDma AxiDmaLOG) {

	//system("Pause");
	int got_num_segs = 0;
	xil_printf("\r\nDividing audio...");
	int* speech_ranges = divide_audio(audio_arr, audio_len, &got_num_segs);
	xil_printf("\r\nDetected %d segments", got_num_segs);
	//system("Pause");
	unsigned int phrase_result = 0;
	
	//if (got_num_segs == 2) {
		// We only need to process 2 segments
		for (int j = 0; j < 2 * got_num_segs; j += 2) {
			int start_index = speech_ranges[j];
			int end_index = speech_ranges[j + 1];

			xil_printf("\r\nProcessing segment %d", (j / 2) + 1);
			int* cur_audio = select_int_segment(audio_arr, start_index, end_index);

			int mfcc_len;
			int selected_audio_len = end_index - start_index;
			//system("Pause");
			xil_printf("\r\nComputing MFCC for total length of %d", selected_audio_len);
			float** cur_mfcc = mfcc_workflow(cur_audio, selected_audio_len, &mfcc_len, AxiDmaFFT, AxiDmaLOG);
			//xil_printf("\r\nMFCC length is %d", mfcc_len);

			float min_dist = DBL_MAX;
			unsigned int selected_index = 0xF;
			int selected_lib_index = -1;
			//xil_printf("\r\nRunning DTW");

			if (j == 0) {
				for (int i = 0; i < LEN_AUDIO_LIB_1; i++) {
					//printf("Lib is %s\n", varTable[i].name);
					//print_2d_float_special(varTable[i].var, varTable[i].length, MFCC_SIZE);

					float distance = compute_dtw(varTable_1[i].var, cur_mfcc, varTable_1[i].length, mfcc_len);
					//xil_printf("\r\nDTW has returned");
					if (distance < min_dist && min_dist > 0) {
						min_dist = distance;
						selected_index = varTable_1[i].rep;
						selected_lib_index = i;
					}

					if (min_dist > 0) {
						xil_printf("\r\nDistance for %s: ", varTable_1[i].name);
						print_float(distance);
					}
				}

				if (min_dist > 0) {
					xil_printf("\r\nClosest matching word is %s: ", varTable_1[selected_lib_index].name);
					print_float(min_dist);
					// No shifting for the 1st matched sound :D
					phrase_result = selected_index;
					xil_printf("\r\nPhrase(HEX) is %x", phrase_result);
				}

				if (phrase_result == ENTER){
					break;
				}
			}
			else if (j == 2) {
				for (int i = 0; i < LEN_AUDIO_LIB_2; i++) {
					//printf("Lib is %s\n", varTable[i].name);
					//print_2d_float_special(varTable[i].var, varTable[i].length, MFCC_SIZE);

					float distance = compute_dtw(varTable_2[i].var, cur_mfcc, varTable_2[i].length, mfcc_len);
					//xil_printf("\r\nDTW has returned");
					if (distance < min_dist && min_dist > 0) {
						min_dist = distance;
						selected_index = varTable_2[i].rep;
						selected_lib_index = i;
					}

					if (min_dist > 0) {
						xil_printf("\r\nDistance for %s: ", varTable_2[i].name);
						print_float(distance);
					}
				}

				if (min_dist > 0) {
					xil_printf("\r\nClosest matching word is %s: ", varTable_2[selected_lib_index].name);
					print_float(min_dist);
					phrase_result = (phrase_result << 4) | selected_index;
					xil_printf("\r\nPhrase(HEX) is %x", phrase_result);
				}
			}

			xil_printf("\r\n ---- \r\n");
		}
	//}
	//else {
	//	phrase_result = ENTER;
	//}


	xil_printf("\r\nPhrase(HEX) is %x", phrase_result);
	return phrase_result;
}
