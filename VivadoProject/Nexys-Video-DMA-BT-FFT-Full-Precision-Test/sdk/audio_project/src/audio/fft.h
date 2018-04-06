
#ifndef FFT_H
#define FFT_H

#ifdef MICROBLAZE_C
#include "xaxidma.h"
#else
#include "microblaze_stub.h"
#endif

#include "dma_functions.h"
#include "debugger.h"
#include "fft_coeffs.h"
#include "matmul.h"

#define FFT_N 512
#define FFT_K 257

#define SIGNED_SHORT_MAX 32767		// 2^15 - 1

#define SCALE_DOWN			16
#define POWER_SCALE_UP	SCALE_DOWN * SCALE_DOWN
#define HW_SCALE			FFT_N

// this function only handles real-value discrete FFT of
// n = 512, k = 257
float* compute_fft(float* values, XAxiDma AxiDmaFFT);

// Compute using software via look up table matmul
float* fft_with_matmul(float* input);

// Compute using hardwre via FFT IP
float* fft_with_ip(int* input, XAxiDma AxiDmaFFT);

// Compare hardware/software value and print a ratio
void fft_result_compare(float* matmul_fft, float* ip_fft);

float test_x[512];

 #endif

