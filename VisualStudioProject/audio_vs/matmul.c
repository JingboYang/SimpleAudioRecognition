#include <stdlib.h>

#include "matmul.h"

// we transpose a2 for you here :D
// ensure a1_nc == a2_nc
float** matmul(float** a1, int a1_nr, int a1_nc, float** a2, int a2_nr, int a2_nc){

    float** result = (float**)malloc(a1_nr * sizeof(float*));
    for (int i = 0; i < a1_nr; i++){
        float* temp = (float*)malloc(a2_nr * sizeof(float));
        result[i] = temp;
    }

	//print_2d_float(a2, a2_nr, a2_nc);

    for (int i = 0; i < a1_nr; i++){
        for (int j = 0; j < a2_nr; j++){
            float s = 0;
            for (int k = 0; k < a1_nc; k++){
				s += a1[i][k] * a2[j][k];
            }
            result[i][j] = s;
        }
    }
    return result;
}


// we transpose a2 for you here :D
// ensure a1_nc == a2_nc
float** matmul_special(float** a1, int a1_nr, int a1_nc, float* a2, int a2_nr, int a2_nc) {

	float** result = (float**)malloc(a1_nr * sizeof(float*));
	for (int i = 0; i < a1_nr; i++) {
		float* temp = (float*)malloc(a2_nr * sizeof(float));
		result[i] = temp;
	}
	//print_2d_float_special(a2, a2_nr, a2_nc);
	for (int i = 0; i < a1_nr; i++) {
		for (int j = 0; j < a2_nr; j++) {
			float s = 0;
			for (int k = 0; k < a1_nc; k++) {
				int a2_index = j * a2_nc + k;
				s += a1[i][k] * a2[a2_index];
			}
			result[i][j] = s;

			//xil_printf("\r\nSumming a1_row: %d a2_row: %d  Result: ", i, j);
			//print_float(s);
		}
	}
	return result;
}
