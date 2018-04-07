/*
 * audio_processor_entry.h
 *
 *  Created on: Mar 9, 2018
 *      Author: jingb
 */

#ifndef SRC_AUDIO_AUDIO_PROCESSOR_ENTRY_H_
#define SRC_AUDIO_AUDIO_PROCESSOR_ENTRY_H_


#ifdef MICROBLAZE_C
#include "xaxidma.h"
#else
#include "microblaze_stub.h"
#endif


#include "stored_sounds.h"
#include "do_matching.h"
#include "debugger.h"

#define SCALE 256

#define OPEN_DOOR_CMD		'A'
#define CLOSE_DOOR_CMD		'B'
#define LIGHT_ON_CMD		'C'
#define LIGHT_OFF_CMD		'D'
#define ENTER_CMD			'E'

#define SACLE_SKIP 1600
#define INITIAL_MAX 32767.0
#define ACCEPTED_MAX 1048576

char match_phrase(unsigned int phrase);

int* convert_audio_original(int* original, int original_length);
int* convert_audio(int* original, int original_length);

char entry_point(int* audio_arr, int audio_len, XAxiDma AxiDmaFFT, XAxiDma AxiDmaLOG);

#endif /* SRC_AUDIO_AUDIO_PROCESSOR_ENTRY_H_ */
