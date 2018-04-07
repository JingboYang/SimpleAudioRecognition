#ifndef MEL_FREQ_H
#define MEL_FREQ_H

#include <math.h>
#include "debugger.h"
#include "dma_functions.h"
#include "coefficients.h"

#define NUMBER_FFT 257
#define NUMBER_MEL 22

#define USE_IP_LOG 1
#define LOG_RATIO_CUSTOM 8.68588963807
#define LOG_RATIO_IP 2.8

float** compute_mel(float* fft_powers, XAxiDma AxiDmaLOG);

#endif
