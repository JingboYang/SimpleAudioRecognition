#include <stdio.h>
#include <stdlib.h>


#include "audio_processor_entry.h"

int* read_audio(char* filename, int* out_audio_len){

    FILE *fileptr;
    unsigned char *buffer;
    long filelen;

    fileptr = fopen(filename, "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (unsigned char *)malloc((filelen + 1) * sizeof(unsigned char)); // Enough memory for file + \0
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file

    printf("buffer size %d\n", filelen);

    int audio_len = (filelen + 1) / 2;
    int* audio_array = (int *)malloc(audio_len * sizeof(int)); // Enough memory for file + \0

    /*for (int i = 0; i < filelen; i++){
        printf("butter at %d: %d\n", i, buffer[i]);
    }*/

    for (int i = 0; i < filelen; i += 2){
        //printf("butter at %d: %d\n", i, buffer[i]);
        unsigned char low = buffer[i];
        unsigned char high = buffer[i + 1];
        short temp = (high << 8) | low;
        //printf("high at %d: %u\n", i, high);
        //printf("low at %d: %u\n", i + 1, low);
        //printf("combined is %u\n", temp);
        audio_array[i / 2] = (int)(temp);
        //xil_printf("combined is %d\n", temp);
    }

    for (int i = 0; i < audio_len; i++){
        //printf("audio array at %d: %u\n", i, audio_array[i]);
    }

    *out_audio_len = audio_len;

    return audio_array;
}



char match_phrase(unsigned int phrase) {

	char result = 'F';

	switch (phrase) {
	case OPEN_DOOR:
		xil_printf("\r\nOpen door");
		result = OPEN_DOOR_CMD;
		break;
	case CLOSE_DOOR:
		xil_printf("\r\nClose door");
		result = CLOSE_DOOR_CMD;
		break;
	case KAI_DENG:
		xil_printf("\r\nKai deng");
		result = LIGHT_ON_CMD;
		break;
	case GUAN_DENG:
		xil_printf("\r\nGuan deng");
		result = LIGHT_OFF_CMD;
		break;
	case LIGHT_OFF:
			xil_printf("\r\nLight off");
			result = LIGHT_OFF_CMD;
			break;
	case LIGHT_ON:
			xil_printf("\r\nLight on");
			result = LIGHT_ON_CMD;
			break;
	case ENTER_FULL:
	case ENTER_PART:
		xil_printf("\r\nEnter sequence");
		result = ENTER_CMD;
		break;
	default:
		xil_printf("\r\nUnable to recognize command %x. Checking for other options", phrase);
	}

	// In case we still haven't recognized
	if (result == 'F') {
		unsigned int temp_phrase = phrase & 0xF0;
		switch (temp_phrase) {
			case ENTER_FULL:
			case ENTER_PART:
				xil_printf("\r\nEnter sequence");
				result = ENTER_CMD;
				break;
		default:
			xil_printf("\r\nFailed to recognize %x.", phrase);
		}

		temp_phrase = phrase & 0x0F;
		switch (temp_phrase) {
			case ENTER_FULL:
			case ENTER_PART:
				xil_printf("\r\nEnter sequence");
				result = ENTER_CMD;
				break;
		default:
			xil_printf("\r\nFailed to recognize %x.", phrase);
		}
	}

	xil_printf("\r\n");
	return result;
}


int* convert_audio(int* original, int original_length){
	int* result = (int*)malloc(original_length * sizeof(int));

	for (int i = 0; i < original_length; i++){

		result[i] = original[i];
		if (result[i] & 0x800000){
			result[i] |=  ~0xffffff;
		}
		result[i] = result[i] - 5000;
		//result[i] = result[i] / 32;

		//if (result[i] > 32767){
		//	result[i] = 32767;
		//} else if (result[i] < -32768){
		//	result[i] = -32768;
		//}
		//xil_printf("\r\n%d %d", original[i], result[i]);
	}

	int audio_min = 0;
	int audio_max = 0;

	for (int i = SACLE_SKIP; i < original_length; i++){
		if (result[i] > -ACCEPTED_MAX && result[i] < ACCEPTED_MAX){
			audio_min = min(result[i], audio_min);
			audio_max = max(result[i], audio_max);
		}
	}

	xil_printf("\r\nOriginal Max: %d Original Min: %d", audio_max, audio_min);
	float ratio = INITIAL_MAX / max(audio_max, -audio_min);
	xil_printf("\r\nInitial scale ratio: ");
	print_float(ratio);

	for (int i = 0; i < original_length; i++){
		result[i] = result[i] * ratio;
	}

	xil_printf("\r\n%u", result);
	return result;
}

int* convert_audio_original(int* original, int original_length){
	int* result = (int*)malloc(original_length * sizeof(int));

	for (int i = 0; i < original_length; i++){
		result[i] = (int)original[i];
	}

	//xil_printf("\r\n%u", result);
	return result;
}


int* downsampler(int* original, int orig_length, int ratio){
	int* result = (int*)malloc((orig_length / ratio) * sizeof(int));

	int j = 0;
	for (int i = 0; i < orig_length; i+= ratio){
		result[j] = original[i];
		j++;
	}

	return result;
}


char entry_point(int* audio_arr, int audio_len, XAxiDma AxiDmaFFT, XAxiDma AxiDmaLOG){
	unsigned int phrase = matching_workflow(audio_arr, audio_len, AxiDmaFFT, AxiDmaLOG);

	xil_printf("\r\nMatching audio workflow has completed...");
	char result = match_phrase(phrase);
	return result;
}
