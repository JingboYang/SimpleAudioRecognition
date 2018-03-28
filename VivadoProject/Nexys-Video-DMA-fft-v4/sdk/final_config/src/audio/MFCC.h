#ifndef MFCC_H
#define MFCC_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "microblaze_switch.h"

#ifdef MICROBLAZE_C
#include "xaxidma.h"
#else
#include "microblaze_stub.h"
#endif

#include "coefficients.h"
#include "matmul.h"
#include "debugger.h"
#include "dma_functions.h"
#include "fft.h"
#include "mel_freq.h"

#define SHIFT_SIZE 256
#define WINDOW_SIZE 512
#define PRE_EMPH_COEF 0.97
#define NUMBER_FFT 257
#define NUMBER_MEL 22

#define MAX_VALUE 31129.6

// converts to float for easier operation later
float* convert_audio_to_float(int* audio_arr, int audio_len);

// Normalize audio
void normalize_audio(float* audio_arr, int audio_len);

// we should not operate on the original array, so we have to copy things out
float* select_segment(float* original, int start_index, int end_index, int orig_arr_len);

float** cast_to_2d(float* original);

// operates in place, need start/end index
void pre_emphasis(float* audio);

// operates in place
void hanning_window(float* audio);
// unsure what to do for now
float* compute_fft(float* audio, XAxiDma AxiDmaFFT);

float** process_mel(float* fft_powers, XAxiDma AxiDmaLOG);

float** dct(float** values);

// also converts down from 2D to 1D float array
float* re_scaling(float** mfcc);

float** mfcc_workflow(int* audio_arr, int audio_len, int* out_mfcc_len, XAxiDma AxiDmaFFT, XAxiDma AxiDmaLOG);
#endif
