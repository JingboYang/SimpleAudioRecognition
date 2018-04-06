
#ifndef MATMUL_C
#define MATMUL_C

#include "debugger.h"

// "typical" matmul
float** matmul(float** a1, int a1_nr, int a1_nc, float** a2, int a2_nr, int a2_nc);

// we transpose a2 for you here :D
// ensure a1_nc == a2_nc
// note for all of our applications, a2 is in fact a 2D array but passed around as 1D
// so that C can correctly handle it
float** matmul_special(float** a1, int a1_nr, int a1_nc, float* a2, int a2_nr, int a2_nc);
#endif // GRANDPARENT_H

