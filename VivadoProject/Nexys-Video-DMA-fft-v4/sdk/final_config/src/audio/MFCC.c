

#include "mfcc.h"

// converts to float for easier operation later
float* convert_audio_to_float(int* audio_arr, int audio_len){
    float* new_audio_arr = (float*)malloc(audio_len * sizeof(float));
    for (int i = 0; i < audio_len; i++){
        new_audio_arr[i] = audio_arr[i];
    }

    return new_audio_arr;
}


// Normalize audio
void normalize_audio(float* audio_arr, int audio_len) {
	
	// Okay because audio is -2^15 -> 2^15
	float audio_max = 0;
	float audio_min = 0;

	for (int i = 0; i < audio_len; i++) {
		audio_max = max(audio_arr[i], audio_max);
		audio_min = min(audio_arr[i], audio_min);
	}

	audio_min = -audio_min;
	float ratio = MAX_VALUE / max(audio_max, audio_min);
	xil_printf("\r\nRatio: ");
	print_float(ratio);

	for (int i = 0; i < audio_len; i++) {
		audio_arr[i] = audio_arr[i] * ratio;
	}
}


// we should not operate on the original array, so we have to copy things out
float* select_segment(float* original, int start_index, int end_index, int orig_arr_len){
    float* new_audio_arr = (float*)malloc(WINDOW_SIZE * sizeof(float));

	// If segment lies outside of selected audio, fill with 0
	if (end_index > orig_arr_len) {
		memcpy(new_audio_arr, &original[start_index], (orig_arr_len - start_index) * sizeof(float));
		for (int i = orig_arr_len; i < end_index; i++) {
			new_audio_arr[i - orig_arr_len] = 0;
		}
	}
	else {
		memcpy(new_audio_arr, &original[start_index], (end_index - start_index) * sizeof(float));
	}
    
    return new_audio_arr;
}


float** cast_to_2d(float* original){

    float** result = (float**)malloc(sizeof(float*));
    result[0] = original;

    return result;
}

// operates in place, need start/end index
void pre_emphasis(float* audio){
    for (int i = 1; i < WINDOW_SIZE; i++){
        audio[i] = audio[i] - PRE_EMPH_COEF * audio[i - 1];
    }
}


// operates in place
void hanning_window(float* audio){
    for (int i = 0; i < WINDOW_SIZE; i++){
        audio[i] = audio[i] * HANNING_COEF[i];
    }
}

// unsure what to do for now
float* compute_fft_mfcc(float* audio, XAxiDma AxiDmaFFT){
	float* result = compute_fft(audio, AxiDmaFFT);
    return result;
}


float** process_mel(float* fft_powers, XAxiDma AxiDmaLOG){
	float** result = compute_mel(fft_powers, AxiDmaLOG);
    return result;
}

float** dct(float** values){

    float** results = matmul_special(values, 1, NUMBER_MEL, DCT_COEF, NUMBER_MEL, NUMBER_MEL);
    return results;
}

// also converts down from 2D to 1D float array
float* re_scaling(float** mfcc){
    for (int i = 0; i < NUMBER_MEL; i++){
        mfcc[0][i] = mfcc[0][i] * LIFT[i];
    }

    return mfcc[0];
}


float** mfcc_workflow(int* audio_arr, int audio_len, int* out_mfcc_len, XAxiDma AxiDmaFFT, XAxiDma AxiDmaLOG){

	//print_1d_int(audio_arr, audio_len);

    float* audio_arr_float = convert_audio_to_float(audio_arr, audio_len);
	//print_1d_float(audio_arr_float, audio_len);
	normalize_audio(audio_arr_float, audio_len);

    int num_windows = ((audio_len - WINDOW_SIZE) / SHIFT_SIZE) + 1;
    float** result_mfcc = (float**)malloc(num_windows * sizeof(float*));

    int counter = 0;
    for (int i = 0; i < audio_len - SHIFT_SIZE; i += SHIFT_SIZE){

    	if (counter % 3 == 0 || counter == num_windows - 1){
    		xil_printf("\r\nOperating on window %d/%d", (counter + 1), num_windows);
    	}

        int start_index = i;
        int end_index = i + WINDOW_SIZE;

		//xil_printf("\r\nSelecting segment %d - %d", start_index, end_index);
        float* cur_audio = select_segment(audio_arr_float, start_index, end_index, audio_len);
		//print_1d_float(cur_audio, WINDOW_SIZE);

        //xil_printf("\r\npre emphasis");
        pre_emphasis(cur_audio);
		//print_1d_float(cur_audio, WINDOW_SIZE);

        //xil_printf("\r\nhanning window");
        hanning_window(cur_audio);
		//print_1d_float(cur_audio, WINDOW_SIZE);
		//system("pause");

        //xil_printf("\r\nmfcc");
        float* result_fft = compute_fft_mfcc(cur_audio, AxiDmaFFT);
		//print_1d_float(result_fft, NUMBER_FFT);

        //xil_printf("\r\nmel");
        float** result_mel = process_mel(result_fft, AxiDmaLOG);
		//print_2d_float(result_mel, 1, NUMBER_MEL);

        //xil_printf("\r\ndct");
        float** final_mel = dct(result_mel);
		//print_2d_float(final_mel, 1, NUMBER_MEL);

        //xil_printf("\r\nre-scale");
        float* scaled_mel = re_scaling(final_mel);
		//print_1d_float(scaled_mel, NUMBER_MEL);
        
        //xil_printf("\r\ndone");
		result_mfcc[counter] = scaled_mel;
        counter ++;

		//system("pause");
    }

	
    *out_mfcc_len = num_windows;
    return result_mfcc;
}
