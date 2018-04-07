#include <math.h>
#include <limits.h>

#include "audio_divider.h"

// chunk has default of size 512
float rms(int* audio_arr, int start, int end){

    float sum = 0;
    for (int i = start; i < end; i++){
        sum += audio_arr[i] * audio_arr[i];
    }
	
	// Do not have to square the result
    //float result = sqrt(sum / (end - start));
	// return result;

	float result_squared = sum / (end - start);
    return result_squared;
}


int* divide_audio(int* audio_arr, int length, int* got_num_segs){

    int non_speech_count = 0;
    int is_speech = FALSE;
    int speech_start = INT_MAX;
    int speech_end = INT_MAX;

    // let's crash if we detect more than a few audio segments ....
    int* speech_ranges = (int*)malloc(MAX_NUM_SEGS * 2 * sizeof(int));
    int cur_seg = 0;

    //print_1d_int(audio_arr, length);

	int start_index;
	int end_index;

	*got_num_segs = 0;
	// Skip 1st segment because DMA sometimes give noise
    for (int i = CHUNK_SIZE; i < length - CHUNK_SIZE; i+= CHUNK_SIZE){
        start_index = i;
        end_index = i + CHUNK_SIZE;

        float rms_val = rms(audio_arr, start_index, end_index);

		//xil_printf("\r\n");
		//print_float(rms_val);
		
        if ( rms_val > SILENCE_THRESHOLD_SQ){
            non_speech_count = 0;
            if (is_speech == FALSE){
                speech_start = start_index;
                speech_end = end_index;
                is_speech = TRUE;
            } else {
                speech_end = end_index;
            }
        } else {
            if (is_speech == FALSE){
                // do nothing
            } else {
            	non_speech_count += 1;

            	if (non_speech_count > CONTINUED_SILENCE){
                    is_speech = FALSE;
                    speech_ranges[cur_seg] = speech_start;
                    speech_ranges[cur_seg + 1] = speech_end;

					xil_printf("\r\nSpeech segment: %d, %d", speech_start, speech_end);
					xil_printf("\r\n         Start: ");
					print_float_fixed(((float)speech_start) / SAMPLE_RATE, E0);
					xil_printf(", End: ");
					print_float_fixed(((float)speech_end) / SAMPLE_RATE, E0);

                    cur_seg += 2;
					
					*got_num_segs = (*got_num_segs) + 1;
					// Break after seeing 2 segments
                    // We only need to record first 2 segments
					if (cur_seg == MAX_NUM_SEGS * 2) {
						break;
					}
                } 
            }
        }
    }

	if (non_speech_count <= 4) {
		speech_ranges[cur_seg] = speech_start;
		speech_ranges[cur_seg + 1] = end_index;

		xil_printf("\r\nSpeech segment: %d, %d", speech_start, end_index);
		xil_printf("\r\n         Start: ");
		print_float_fixed(((float)speech_start) / SAMPLE_RATE, E0);
		xil_printf(", End: ");
		print_float_fixed(((float)speech_end) / SAMPLE_RATE, E0);

		*got_num_segs = (*got_num_segs) + 1;
	}

	

    return speech_ranges;
}
