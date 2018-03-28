#ifndef MB_H
#define MB_H

#include <stdio.h>
#include <stdarg.h>

#include "microblaze_switch.h"

// Remove to compile for Visual Studio

#ifndef MICROBLAZE_C
#include <time.h>

typedef void* XAxiDma;
typedef int u32;

#define XAXIDMA_DMA_TO_DEVICE 0
#define XAXIDMA_DEVICE_TO_DMA 1

#define XST_SUCCESS 0
#define XST_FAILURE 0

int XAxiDma_SimpleTransfer(XAxiDma*, u32, int, int);
int XAxiDma_Busy(XAxiDma*, int);

// Substitutde for xil_printf
void xil_printf(char *fmt, ...);

void microblaze_flush_dcache();
void microblaze_invalidate_dcache();
#endif		// ifdef MICROBLAZE_C

#endif
