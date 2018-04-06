#include "mel_freq.h"


float** compute_mel(float* fft_powers, XAxiDma AxiDmaLOG) {
	float** powers_2d = cast_to_2d(fft_powers);

	//print_2d_float(powers_2d, 1, NUMBER_FFT);

	float** results = matmul_special(powers_2d, 1, NUMBER_FFT, MEL_FILTER_BANK, NUMBER_MEL, NUMBER_FFT);

	//print_2d_float(results, 1, NUMBER_MEL);

	float* ip_log = (float*)malloc(NUMBER_MEL * sizeof(float));

	if (USE_IP_LOG == 1) {
		DMA_LOG(results[0], ip_log, NUMBER_MEL, AxiDmaLOG);
		for (int i = 0; i < NUMBER_MEL; i++) {
			//float temp = log(results[0][i]) * LOG_RATIO_CUSTOM;
			results[0][i] = ip_log[i] * LOG_RATIO_CUSTOM;
			//xil_printf("\r\nRATIO: ");
			//print_float(temp / results[0][i]);
		}
	}
	else {
		for (int i = 0; i < NUMBER_MEL; i++) {
			results[0][i] = log(results[0][i]) * LOG_RATIO_CUSTOM;
		}
	}
	//print_2d_float(results, 1, NUMBER_MEL);
	return results;
}

