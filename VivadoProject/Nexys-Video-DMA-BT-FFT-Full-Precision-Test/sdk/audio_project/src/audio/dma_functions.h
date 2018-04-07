#ifndef DMA_FUNCTIONS_H
#define DMA_FUNCTIONS_H

#include "microblaze_switch.h"

#ifdef MICROBLAZE_C
#include "xaxidma.h"
#else
#include "microblaze_stub.h"
#endif

// To compute FFT via custom IP
int DMA_FFT(u32 *TxBufferPtr, u32 *RxBufferPtr, XAxiDma AxiDmaFFT);

// To compute LOG via custom IP
int DMA_LOG(float *TxBufferPtr, float *RxBufferPtr, u32 Length, XAxiDma sAxiDmaLOG);

#endif
