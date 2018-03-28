#include <stdio.h>
#include <stdlib.h>

//#include "stored_sounds.h"
//#include "do_matching.h"

#include "audio_processor_entry.h"
#include "debugger.h"
#include "time.h"

//#include "matmul.h"
#include "coefficients.h"
//#include "MFCC.h"

// All the stuff should be in audio_processor_entry.h for microblaze
int main()
{
	/*
	float* fft_powers = (float*)malloc(257 * sizeof(float));
	for (int i = 0; i < 257; i++) {
		fft_powers[i] = 1;
	}
	float** powers_2d = cast_to_2d(fft_powers);

	float** results = matmul_special(powers_2d, 1, 257, MEL_FILTER_BANK, 22, 257);
	print_2d_float(powers_2d, 1, 257);
	*/

	
    int audio_len;
	int* audio_arr = NULL;
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/kai_deng.raw", &audio_len);
	audio_arr = read_audio("../../audio_processor/audio_lib/phrase/guan_deng.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/open_door.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/close_door.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/enter_full.raw", &audio_len);

	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/kai_deng_2.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/guan_deng_2.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/open_door_2.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/close_door_2.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/enter_full_2.raw", &audio_len);

	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/kai_deng_3.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/guan_deng_3.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/open_door_3.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/close_door_3.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/enter_full_3.raw", &audio_len);

	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/kai_deng_4.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/guan_deng_4.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/open_door_4.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/close_door_4.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/enter_full_4.raw", &audio_len);

	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/open_door_002.raw", &audio_len);
	//audio_arr = read_audio("../../audio_processor/audio_lib/phrase/enter_full_001.raw", &audio_len);

	//int* converted = convert_audio(audio_arr, audio_len);

	clock_t t;
	t = clock();

	char matching_result = entry_point(audio_arr, audio_len, NULL, NULL);
	
	xil_printf("\r\n =-> %c \r\n", matching_result);

	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

	printf("fun() took %f seconds to execute \n", time_taken);

    xil_printf("\r\n--- DONE ---\r\n");
	system("Pause");
    return 0;
}
