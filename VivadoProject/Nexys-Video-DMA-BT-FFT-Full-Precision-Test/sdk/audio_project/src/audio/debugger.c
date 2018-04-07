#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "debugger.h"

int per_line = 5;

void print_1d_float(float* arr, int length) {

	xil_printf("\n=== Printing 1D float ===\n");
	for (int i = 0; i < length; i += per_line) {
		//if (i % (per_line * 10) == 0) {
		//	xil_printf("==> At %d\n", i);
		//}
		for (int j = 0; j < per_line && (i + j) < length; j++) {
			xil_printf("%.5f, ", arr[i + j]);
		}
		xil_printf("\n");
	}
	xil_printf("\n=== End Printing 1D float ===\n");
}


void print_1d_int(int* arr, int length) {

	xil_printf("\n=== Printing 1D Int ===\n");
	for (int i = 0; i < length; i += per_line) {
		//if (i % (per_line * 10) == 0) {
		//	xil_printf("==> At %d\n", i);
		//}
		for (int j = 0; j < per_line && (i + j) < length; j++) {
			xil_printf("%d, ", arr[i + j]);
		}
		xil_printf("\n");
	}
	xil_printf("\n=== End Printing 1D Int ===\n");
}


void print_2d_float(float** arr, int n_row, int n_col) {

	xil_printf("\n=== Printing 2D float ===\n");
	for (int i = 0; i < n_row; i += 1) {
		xil_printf("==> Row : %d\n", i);
		for (int j = 0; j < n_col; j += per_line) {
			for (int k = 0; k < per_line && (k + j) < n_col; k++) {
				xil_printf("%.5f, ", arr[i][k + j]);
			}
			xil_printf("\n");
		}
		xil_printf("\n");
	}
	xil_printf("\n=== End Printing 2D float ===\n");
}

// print a 2D array as 1D
void print_2d_float_special(float* arr, int n_row, int n_col) {

	xil_printf("\n=== Printing 2D float !SPECIAL! ===\n");
	for (int i = 0; i < n_row; i += 1) {
		xil_printf("==> Row : %d\n", i);
		for (int j = 0; j < n_col; j += per_line) {
			for (int k = 0; k < per_line && (k + j) < n_col; k++) {
				int arr_index = i * n_col + k + j;
				xil_printf("%.5f, ", arr[arr_index]);
			}
			xil_printf("\n");
		}
		xil_printf("\n");
	}
	xil_printf("\n=== End Printing 2D float !SPECIAL! ===\n");
}


int precision = 10000;
float pow10s[KNOWN_LEN] = {	1000000000000000000,
							1000000000000000,	
							1000000000000,
							1000000000, 
							1000000, 
							1000, 
							1, 
							0.001, 
							0.000001, 
							0.000000001};
//{ 0.000001, 0.001, 1, 1000, 1000000, 1000000000 };
int powChars[KNOWN_LEN] = {	18, 15, 12, 9, 
							6, 3, 0, -3, 
							-6, -9};

int print_float(float value) {
	for (int i = 0; i < KNOWN_LEN; i++) {
		float divided = value / pow10s[i];
		if (divided < 1000 && divided >= 1) {
			int whole = (int)divided;
			int decimal = (int)((divided - whole) * precision);
			if (i != E0) {
				xil_printf("%3d.%04dE%d", whole, decimal, powChars[i]);
			}
			else {
				xil_printf("%3d.%04d  ", whole, decimal);
			}
			
			return i;
		}
	}
	return -1;
}

void print_float_fixed(float value, int decimal_index) {
	float divided = value / pow10s[decimal_index];
	int whole = (int)divided;
	int decimal = (int)((divided - whole) * precision);

	if (decimal_index != E0) {
		xil_printf("%3d.%04dE%d", whole, decimal, powChars[decimal_index]);
	}
	else {
		xil_printf("%3d.%04d  ", whole, decimal);
	}
}
