#ifndef DTW_H
#define DTW_H

#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "microblaze_switch.h"

#define MFCC_SIZE 22
#define SKIP_FIRST 5
#define END_INDEX 22

#ifdef MICROBLAZE_SWITCH
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

float min3(float a, float b, float c);

float cart_dist(float* a1, float** a2, int a1_index, int a2_index);

float compute_dtw(float* a1, float** a2, int a1_len, int a2_len);

#endif // DTW_H
