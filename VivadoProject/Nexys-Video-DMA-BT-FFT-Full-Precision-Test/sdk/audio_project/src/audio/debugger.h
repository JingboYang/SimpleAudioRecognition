#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "microblaze_switch.h"

#ifdef MICROBLAZE_C
#include "xaxidma.h"
#else
#include "microblaze_stub.h"
#endif

#define KNOWN_LEN	10
#define E18			0
#define E15			1
#define E12			2
#define E9			3
#define E6			4
#define E3			5
#define E0			6
#define E_NEG3		7
#define E_NEG6		8
#define E_NEG9		9

int per_line;

void print_1d_float(float* arr, int length);

void print_1d_int(int* arr, int length);

void print_2d_float(float** arr, int n_row, int n_col);

void print_2d_float_special(float* arr, int n_row, int n_col);

int powChars[KNOWN_LEN];
float pow10s[KNOWN_LEN];
int precision;

int print_float(float value);

// Note that only 0 -> KNOWN_LEN are allowed
// Out of range values will fail to print
void print_float_fixed(float value, int decimal_index);

#endif // !DEBUGGER_H
