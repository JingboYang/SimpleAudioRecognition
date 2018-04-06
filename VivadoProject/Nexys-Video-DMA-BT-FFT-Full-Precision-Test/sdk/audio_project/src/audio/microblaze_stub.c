#include "microblaze_stub.h"

#ifndef MICROBLAZE_C
int XAxiDma_SimpleTransfer(XAxiDma* a, u32 b, int c, int d) { return 0; }
int XAxiDma_Busy(XAxiDma*a , int b) { return 0; }

void microblaze_flush_dcache() {}
void microblaze_invalidate_dcache() {}

// Substitutde for xil_printf
void xil_printf(char *fmt, ...)
{
	va_list argp = NULL;
	va_start(argp, fmt);
	vfprintf(stdout, fmt, argp);
	va_end(argp);
}

#endif
