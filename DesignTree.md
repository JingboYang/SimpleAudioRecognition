# Design Tree of `SimpleAudioRecognition`

This is a description of our design hierarchy. 

## Controller Board

### Hardware IPs

Design components for audio acquisition, communication and algorithmaccelerator

* **axi_dma** Enables AXI-Stream operations among DDR3 and AXI-Stream enabled IP
* **d_axi_i2s_audio** Audio codec data transfer controller
* **xfft** Fast Fourier Transformfor audio signal processing
* **floating_point** Hardware acceleration for floating-point logarithm operation
* **PmodBT2** Bluetooth PMOD IP for data transmission

### Software

Microblaze routines for the audio recognition algorithm. All files have .h header and .c implementations.

* **coefficients** Matrixmultiplication coefficients for prep/post-processing, FFT and DCT
* **stored_sounds** Pre-computed MFCC library of commands
* **dma_functions** Interface with hardware accelerators. Deals with DMA memory content handover
* **audio_processor_entry** Preprocessing and wrapper function for audio recognition routine
* **audio_divider** VAD and audio segmentation
* **MFCC Wrapper** for MFCC computation routine, calls each component of theMFCC routine
* **do_matching Calls** DTWto find the command to which it has the minimumdistance
* **microblaze_stub** A dummy implementation ofMicroblaze-specific functions for the C program to be compatible with Visual Studio

## Hub Board

### Hardware IPs

Design components for communication and controlling various
PMODs (ip_repo)

* **PmodBT2** Bluetooth PMOD IP for data transmission
* **PmodMAXSONAR** Ultrasonic PMOD IP for distance detection
* **servo_con3** Servo controller PMOd IP for controlling two servo motors
* **ssrelay** Relay switch PMOD IP

### Software IPs

* **hub_control** Functions and routines for responding to the commands from the controller board
