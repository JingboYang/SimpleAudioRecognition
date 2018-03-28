#ifndef DO_MATCHING_H
#define DO_MATCHING_H

#include "microblaze_switch.h"

#ifdef MICROBLAZE_C
#include "xaxidma.h"
#else
#include "microblaze_stub.h"
#endif

// we should not operate on the original array, so we have to copy things out
int* select_int_segment(int* original, int start_index, int end_index);

unsigned int matching_workflow(int* audio_arr, int audio_len, XAxiDma AxiDmaFFT, XAxiDma AxiDmaLOG);

#endif
