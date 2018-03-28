
#include "dtw.h"

float min3(float a, float b, float c)
{
    float m = a;
    if (m > b) m = b;
    if (m > c) m = c;
    return m;
}


float norm_sqr_1d_as_2d(float* arr, int row) {
	
	int start_index = row * MFCC_SIZE;
	float sum = 0;
	for (int i = start_index; i < start_index + MFCC_SIZE; i++) {
		sum += arr[i] * arr[i];
	}

	return sum;
}


float norm_sqr_2d(float** arr, int row) {

	float sum = 0;
	for (int i = 0; i < MFCC_SIZE; i++) {
		sum += arr[row][i] * arr[row][i];
	}

	return sum;
}


float cart_dist(float* a1, float** a2, int a1_index, int a2_index){

    float sum = 0;
    for (int i = SKIP_FIRST; i < END_INDEX; i++){
    	float temp = *((a1 + a1_index * MFCC_SIZE) + i) - a2[a2_index][i];
    	sum += temp * temp;
    }

    return sum;
}

// We do not have to cheap out to do half
/*
float compute_dtw(float* a1, float** a2, int a1_len, int a2_len){

	//xil_printf("\r\nDTW initialization");

	int a1_half = (a1_len + 1) / 2;
	int a2_half = (a2_len + 1) / 2;

	//xil_printf("\r\nMalloc size: %d %d", a1_half , a2_half);
    float** dtw_array  = (float **)malloc((a1_half + 1) * sizeof(float*));
    for (int i = 0; i < a1_half + 1; i++){
        dtw_array[i] = (float *)malloc((a2_half + 1) * sizeof(float));
    }

    for (int i = 1; i < a1_half + 1; i++){
        dtw_array[i][0] = norm_sqr_1d_as_2d(a1, i);
    }

    for (int i = 1; i < a2_half + 1; i++){
        dtw_array[0][i] = norm_sqr_2d(a2, i);
    }

    dtw_array[0][0]= 0;

    //xil_printf("\r\nDTW running");
    for (int i = 1; i < a1_half + 1; i++) {
        for (int j = 1; j < a2_half + 1; j++){

            float cost = cart_dist(a1, a2, i - 1, j - 1);
			
			int len_diff = min(max(abs(i - j), 1), 5);

			//int len_diff = 1;

			float d1 = cost * len_diff + dtw_array[i - 1][j];
			float d2 = cost * len_diff + dtw_array[i][j - 1];
			float d3 = cost * max(len_diff / 2, 1) + dtw_array[i - 1][j - 1];

            dtw_array[i][j] = min3(d1, d2, d3);
			//printf("Resule %f\n", dtw_array[i][j]);
        }
    }
    //xil_printf("\r\nDTW completed");
	//print_2d_float(dtw_array, a1_len + 1, a2_len + 1);
    return dtw_array[a1_half][a2_half];
}
*/



float compute_dtw(float* a1, float** a2, int a1_len, int a2_len){

	//xil_printf("\r\nDTW initialization");

	int a1_half = a1_len;
	int a2_half = a2_len;

	//xil_printf("\r\nMalloc size: %d %d", a1_half , a2_half);
	float** dtw_array  = (float **)malloc((a1_half + 1) * sizeof(float*));
	for (int i = 0; i < a1_half + 1; i++){
		dtw_array[i] = (float *)malloc((a2_half + 1) * sizeof(float));
	}

	for (int i = 1; i < a1_half + 1; i++){
		dtw_array[i][0] = norm_sqr_1d_as_2d(a1, i - 1);
	}

	for (int i = 1; i < a2_half + 1; i++){
		dtw_array[0][i] = norm_sqr_2d(a2, i - 1);
	}

	dtw_array[0][0]= 0;

	//xil_printf("\r\nDTW running");
	for (int i = 1; i < a1_half + 1; i++) {
		for (int j = 1; j < a2_half + 1; j++){

			float cost = cart_dist(a1, a2, i - 1, j - 1);

			//xil_printf("\r\n");
			//print_float(cost);
			//printf("\r\n%f", cost);

			int len_diff = max(abs(i - j), 1);

			//int len_diff = 1;

			float d1 = cost * len_diff + dtw_array[i - 1][j];
			float d2 = cost * len_diff + dtw_array[i][j - 1];
			float d3 = cost * max(len_diff / 2, 1) + dtw_array[i - 1][j - 1];

			dtw_array[i][j] = min3(d1, d2, d3);
			//printf("Resule %f\n", dtw_array[i][j]);
		}
	}
	//xil_printf("\r\nDTW completed");
	//print_2d_float(dtw_array, a1_len + 1, a2_len + 1);
	return dtw_array[a1_half][a2_half];
}

