/******************************************************************************
*
* (c) Copyright 2010-2013 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
 *
 * @file xaxidma_example_simple_intr.c
 *
 * This file demonstrates how to use the xaxidma driver on the Xilinx AXI
 * DMA core (AXIDMA) to transfer packets.in interrupt mode when the AXIDMA core
 * is configured in simple mode
 *
 * This code assumes a loopback hardware widget is connected to the AXI DMA
 * core for data packet loopback.
 *
 * To see the debug print, you need a Uart16550 or uartlite in your system,
 * and please set "-DDEBUG" in your compiler options. You need to rebuild your
 * software executable.
 *
 * Make sure that MEMORY_BASE is defined properly as per the HW system. The
 * h/w system built in Area mode has a maximum DDR memory limit of 64MB. In
 * throughput mode, it is 512MB.  These limits are need to ensured for
 * proper operation of this code.
 *
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 4.00a rkv  02/22/11 New example created for simple DMA, this example is for
 *       	       simple DMA,Added interrupt support for Zynq.
 * 4.00a srt  08/04/11 Changed a typo in the RxIntrHandler, changed
 *		       XAXIDMA_DMA_TO_DEVICE to XAXIDMA_DEVICE_TO_DMA
 * 5.00a srt  03/06/12 Added Flushing and Invalidation of Caches to fix CRs
 *		       648103, 648701.
 *		       Added V7 DDR Base Address to fix CR 649405.
 * 6.00a srt  03/27/12 Changed API calls to support MCDMA driver.
 * 7.00a srt  06/18/12 API calls are reverted back for backward compatibility.
 * 7.01a srt  11/02/12 Buffer sizes (Tx and Rx) are modified to meet maximum
 *		       DDR memory limit of the h/w system built with Area mode
 * 7.02a srt  03/01/13 Updated DDR base address for IPI designs (CR 703656).
 *
 * </pre>
 *
 * ***************************************************************************
 */


#include "demo.h"
//#include "audio/test_audio_eric.h"
//#include "audio/debugger.h"


#include "audio/audio.h"
#include "dma/dma.h"
#include "intc/intc.h"
#include "userio/userio.h"
#include "iic/iic.h"

/***************************** Include Files *********************************/

#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xiic.h"
#include "xaxidma.h"



#ifdef XPAR_INTC_0_DEVICE_ID
 #include "xintc.h"
#else
 #include "xscugic.h"
#endif

/************************** Include from Bluetooth Configuration **************/
#include "xil_cache.h"
#include "PmodBT2.h"
#include "xuartlite.h"
#define BT2_UART_AXI_CLOCK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ

/************************* Peter's audio test file ***************************/
#include "test_audio.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/
#if (!defined(DEBUG))
extern void xil_printf(const char *format, ...);
#endif

/************************** Variable Definitions *****************************/
/*
 * Device instance definitions
 */

static XIic sIic;
static XAxiDma sAxiDma, sAxiDmaFFT, sAxiDmaLOG;		/* Instance of the XAxiDma */
static XIntc sIntc;
static XGpio sUserIO;

//
// Interrupt vector table
const ivt_t ivt[] = {
	//IIC
	{XPAR_MICROBLAZE_0_AXI_INTC_AXI_IIC_0_IIC2INTC_IRPT_INTR, (XInterruptHandler)XIic_InterruptHandler, &sIic},
	//DMA Stream to MemoryMap Interrupt handler
	{XPAR_MICROBLAZE_0_AXI_INTC_AXI_DMA_0_S2MM_INTROUT_INTR, (XInterruptHandler)fnS2MMInterruptHandler, &sAxiDma},
	//DMA MemoryMap to Stream Interrupt handler
	{XPAR_MICROBLAZE_0_AXI_INTC_AXI_DMA_0_MM2S_INTROUT_INTR, (XInterruptHandler)fnMM2SInterruptHandler, &sAxiDma},
	//User I/O (buttons, switches, LEDs)
	{XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR, (XInterruptHandler)fnUserIOIsr, &sUserIO}
};

/************************* Function inclusion from Bluetooth module **********/
PmodBT2 myDevice1;
u8 buf[1];

void DemoInitialize()
{
    //EnableCaches();
    BT2_Begin (
        &myDevice1,
        XPAR_PMODBT2_0_AXI_LITE_GPIO_BASEADDR,
        XPAR_PMODBT2_0_AXI_LITE_UART_BASEADDR,
        BT2_UART_AXI_CLOCK_FREQ,
        115200
    );
}

void Bluetooth_send(u8* buf)
{
    xil_printf("Initialized PmodBT2 Demo\r\n");
    xil_printf("Sent from BT1 on JA: %c\r\n", (char) buf[0]);
    BT2_SendData(&myDevice1, buf, 1);
}
/************************ Peter's FFT Function ****************************************/
int FFT(u32 *TxBufferPtr, u32 *RxBufferPtr)
{
	/*
	 *  This function computes a 512-point FFT using by sending consecutive input data
	 *  points to the Xilinx FFT IP from memory location TxBufferPtr. After FFT is
	 *  completed, it retrieves the 512 FFT coefficients from the FFT IP and store them
	 *  consecutively into memory location RxBufferPtr.
	 *
	 *  The input to the FFT IP has to be 16 bit fixed-point number in 2's complement.
	 *  Sign-extension is ok as the custom configured input data path will pad zeros to the
	 *  upper 16 bits of the 32 bits input.
	 *
	 *  The output of the FFT data is the squared-magnitude spectrum scaled by a constant
	 *
	 */

	// Sending 512 input data from TxbufferPtr to the FFT IP
	xil_printf("\r\nFFT Start");
	int Status;
	Status = XAxiDma_SimpleTransfer(&sAxiDmaFFT,(u32) TxBufferPtr,
				4*512, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS) {
		xil_printf("\r\nFFT Write Failed");
		return XST_FAILURE;
	}
	while ((XAxiDma_Busy(&sAxiDmaFFT,XAXIDMA_DMA_TO_DEVICE))) {
			/* Wait */
	}

	// Reading 512 output data from FFT IP and store them in RxBufferPtr
	Status = XAxiDma_SimpleTransfer(&sAxiDmaFFT,(u32) RxBufferPtr,
				2*4*512, XAXIDMA_DEVICE_TO_DMA);
	if (Status != XST_SUCCESS) {
		xil_printf("\r\nFFT Read Failed");
		return XST_FAILURE;
	}

	while ((XAxiDma_Busy(&sAxiDmaFFT,XAXIDMA_DEVICE_TO_DMA))) {
			/* Wait */
	}

	//Flush cache
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	return XST_SUCCESS;
}
/************************ Peter's LOG Function ****************************************/
int LOG(float *TxBufferPtr, float *RxBufferPtr, u32 Length)
{
	/*
	 *  This function computes the natural logarithm (ln) for an array of Length
	 *  Both address pointers should point to float values
	 */

	// Sending 512 input data from TxbufferPtr to the FFT IP

	//Flush cache
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	xil_printf("\r\nLOG Start");
	int Status;
	Status = XAxiDma_SimpleTransfer(&sAxiDmaLOG,(u32) TxBufferPtr,
				sizeof(float)*Length, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS) {
		xil_printf("\r\nLOG Write Failed");
		return XST_FAILURE;
	}

	//Flush cache
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	/*
	while ((XAxiDma_Busy(&sAxiDmaLOG,XAXIDMA_DMA_TO_DEVICE))) {
			// Wait
	}
	*/

	// Reading 512 output data from FFT IP and store them in RxBufferPtr
	Status = XAxiDma_SimpleTransfer(&sAxiDmaLOG,(u32) RxBufferPtr,
				sizeof(float)*Length, XAXIDMA_DEVICE_TO_DMA);
	if (Status != XST_SUCCESS) {
		xil_printf("\r\nLOG Read Failed");
		return XST_FAILURE;
	}

	/*
	while ((XAxiDma_Busy(&sAxiDmaLOG,XAXIDMA_DEVICE_TO_DMA)) || (XAxiDma_Busy(&sAxiDmaLOG,XAXIDMA_DMA_TO_DEVICE))) {
			// Wait
	}
	*/


	//Flush cache
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Main function
*
* This function is the main entry of the interrupt test. It does the following:
*	Initialize the interrupt controller
*	Initialize the IIC controller
*	Initialize the User I/O driver
*	Initialize the 2 DMA engines
*	Initialize the Audio I2S controller
*	Enable the interrupts
*	Wait for a button event then start selected task
*	Wait for task to complete
*
* @param	None
*
* @return
*		- XST_SUCCESS if example finishes successfully
*		- XST_FAILURE if example fails.
*
* @note		None.
*
******************************************************************************/
int main(void)
{
	int Status;

	/* Peter's test buffer*/
    u32 *TxBufferPtr;
    u32 *RxBufferPtr;
    float *TxBufferPtr2;
    float *RxBufferPtr2;

    TxBufferPtr = (u32 *) MEM_BASE_ADDR;
    RxBufferPtr = (u32 *) RX_BUFFER_BASE;
    TxBufferPtr2 = (float *) MEM_BASE_ADDR;
    RxBufferPtr2 = (float *) RX_BUFFER_BASE;


	Demo.u8Verbose = 0;


	xil_printf("\r\n--- Entering main() --- \r\n");
	xil_printf("\r\n--- Initialize bluetooth module --- \r\n");
	DemoInitialize();


	//
	//Initialize the interrupt controller

	Status = fnInitInterruptController(&sIntc);
	if(Status != XST_SUCCESS) {
		xil_printf("Error initializing interrupts");
		return XST_FAILURE;
	}


	// Initialize IIC controller
	Status = fnInitIic(&sIic);
	if(Status != XST_SUCCESS) {
		xil_printf("Error initializing I2C controller");
		return XST_FAILURE;
	}

    // Initialize User I/O driver
    Status = fnInitUserIO(&sUserIO);
    if(Status != XST_SUCCESS) {
    	xil_printf("User I/O ERROR");
    	return XST_FAILURE;
    }


	//Initialize DMA0 for audio
	Status = fnConfigDma(&sAxiDma);
	if(Status != XST_SUCCESS) {
		xil_printf("DMA configuration ERROR");
		return XST_FAILURE;
	}

	//Initialize DMA1 for FFT
	Status = fnConfigDmaFFT(&sAxiDmaFFT);
	if(Status != XST_SUCCESS) {
		xil_printf("DMA configuration ERROR");
		return XST_FAILURE;
	}

	//Initialize DMA2 for LOG
	Status = fnConfigDmaLOG(&sAxiDmaLOG);
	if(Status != XST_SUCCESS) {
		xil_printf("DMA configuration ERROR");
		return XST_FAILURE;
	}

	//Initialize Audio I2S
	Status = fnInitAudio();
	if(Status != XST_SUCCESS) {
		xil_printf("Audio initializing ERROR");
		return XST_FAILURE;
	}


	// Enable all interrupts in our interrupt vector table
	// Make sure all driver instances using interrupts are initialized first
	fnEnableInterrupts(&sIntc, &ivt[0], sizeof(ivt)/sizeof(ivt[0]));



    xil_printf("DONE\r\n");

    //main loop

	xil_printf("----------------------------------------------------------\r\n");
	xil_printf("Nexys Video DMA Audio Demo\r\n");
	xil_printf("----------------------------------------------------------\r\n");

    while(1) {

    	// Checking the DMA S2MM event flag
    			if (Demo.fDmaS2MMEvent)
    			{
    				xil_printf("\r\nRecording Done...");

    				// Disable Stream function to send data (S2MM)
    				Xil_Out32(I2S_STREAM_CONTROL_REG, 0x00000000);
    				Xil_Out32(I2S_TRANSFER_CONTROL_REG, 0x00000000);
    				//Flush cache
    				microblaze_flush_dcache();
    				microblaze_invalidate_dcache();
    				// Reset S2MM event and record flag
    				Demo.fDmaS2MMEvent = 0;
    				Demo.fAudioRecord = 0;

    				//Insert test audio to TxBufferPtr
    				/*memcpy(TxBufferPtr, peter_fft_test, 512*sizeof(unsigned int));


    				//FFT Testing code
    				FFT(TxBufferPtr, RxBufferPtr);

    				xil_printf("\r\nFFT testing Complete");
    				xil_printf("\r\nPrinting FFT result");

    				for (i = 0; i < 512; i = i + 1){
    					int real, imaginary;
    					float power;
    					real = RxBufferPtr[2*i];
    					imaginary = RxBufferPtr[2*i + 1];
    					power = (float) real * (float) real + (float) imaginary * (float) imaginary;
    					xil_printf("\r\n%d, %d, %d",i, real, imaginary);
    				}

    				//LOG Testing

    				for (i = 0; i < 512; i = i + 1){
    					TxBufferPtr2[i] = i*20+1;
    				}

    				LOG(TxBufferPtr2, RxBufferPtr2, 50);

    				xil_printf("\r\nLOG testing Complete");
    				xil_printf("\r\nPrinting LOG result");

    				for (i = 0; i < 512; i = i + 1){
    					xil_printf("\r\n%d, %d, %d",i, (int) TxBufferPtr2[i], (int) RxBufferPtr2[i]);
    				}*/


    				//Insert audio recognition code here
    				xil_printf("\r\nAudio Recognition Start");


    				xil_printf("\r\nMEM_BASE_ADDR: %x\n", MEM_BASE_ADDR );

    				int* converted;
    				int* downsampled;
    				int option = 1;
    				//print_1d_signed_int(MEM_BASE_ADDR, NR_AUDIO_SAMPLES);
    				if (option == 0){
    					memcpy(MEM_BASE_ADDR, test_audio_open_door, NR_AUDIO_SAMPLES_ACTUAL * sizeof(int));
						//print_1d_signed_int(MEM_BASE_ADDR, 160000);
						xil_printf("\r\nConverting audio to signed int...");
						converted = convert_audio_original(MEM_BASE_ADDR, NR_AUDIO_SAMPLES_ACTUAL);
						//downsampled = converted;
    				} else if (option == 1){
						xil_printf("\r\nConverting audio to signed int...");
						converted = convert_audio(MEM_BASE_ADDR, NR_AUDIO_SAMPLES);

						//for (int i = 0; i < NR_AUDIO_SAMPLES; i++){
							//xil_printf("\r\n%d %d", ((int*)(MEM_BASE_ADDR))[i], converted[i]);
						//}

						//converted = MEM_BASE_ADDR;
						//print_1d_signed_int(MEM_BASE_ADDR, NR_AUDIO_SAMPLES);
						//print_1d_int(converted, NR_AUDIO_SAMPLES);
						//downsampled = downsampler(converted, NR_AUDIO_SAMPLES, AUDIO_DOWNSAMPLE_RATE);
    				} else {
    					// do nothing
    				}

    				char result = 'E';
    				//result = 'E';

    				//xil_printf("\r\noutput: %u", converted);
    				//print_1d_signed_int(converted, NR_AUDIO_SAMPLES);
    				xil_printf("\r\nRunning audio recognition...");
    				result = entry_point(converted, NR_AUDIO_SAMPLES_ACTUAL, sAxiDmaFFT, sAxiDmaLOG);
    				xil_printf("\r\nResult: %d", result);
    				//print_1d_unsigned_int(MEM_BASE_ADDR, NR_AUDIO_SAMPLES);

    				xil_printf("\r\nAudio recognition completed");

    				xil_printf("\r\nSending Bluetooth communication to Hub board");

    				buf[0] = result;
					Bluetooth_send(buf);
					xil_printf("\r\nBluetooth communicaiton complete");

    			}

    			// Checking the DMA MM2S event flag
    			if (Demo.fDmaMM2SEvent)
    			{
    				xil_printf("\r\nPlayback Done...");

    				// Disable Stream function to send data (S2MM)
    				Xil_Out32(I2S_STREAM_CONTROL_REG, 0x00000000);
    				Xil_Out32(I2S_TRANSFER_CONTROL_REG, 0x00000000);
    				//Flush cache
    				microblaze_flush_dcache();
    				microblaze_invalidate_dcache();
    				// Reset MM2S event and playback flag
    				Demo.fDmaMM2SEvent = 0;
    				Demo.fAudioPlayback = 0;
    			}

    			// Checking the DMA Error event flag
    			if (Demo.fDmaError)
    			{
    				xil_printf("\r\nDma Error...");
    				xil_printf("\r\nDma Reset...");


    				Demo.fDmaError = 0;
    				Demo.fAudioPlayback = 0;
    				Demo.fAudioRecord = 0;
    			}

    			// Checking the btn change event
    			if(Demo.fUserIOEvent) {

    				switch(Demo.chBtn) {
    					case 'u':
    						if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
    						{
    							xil_printf("\r\nStart Recording...");
    							fnSetMicInput();
    							fnAudioRecord(sAxiDma,NR_AUDIO_SAMPLES);
    							Demo.fAudioRecord = 1;
    						}
    						else
    						{
    							if (Demo.fAudioRecord)
    							{
    								xil_printf("\r\nStill Recording...");
    							}
    							else
    							{
    								xil_printf("\r\nStill Playing back...");
    							}
    						}
    						break;
    					case 'd':
    						if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
    						{
    							xil_printf("\r\nStart Playback...");
    							fnSetHpOutput();
    							fnAudioPlay(sAxiDma,NR_AUDIO_SAMPLES);
    							Demo.fAudioPlayback = 1;
    						}
    						else
    						{
    							if (Demo.fAudioRecord)
    							{
    								xil_printf("\r\nStill Recording...");
    							}
    							else
    							{
    								xil_printf("\r\nStill Playing back...");
    							}
    						}
    						break;
    					case 'r':
    						if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
    						{
    							xil_printf("\r\nStart Recording...");
    							fnSetLineInput();
    							fnAudioRecord(sAxiDma,NR_AUDIO_SAMPLES);
    							Demo.fAudioRecord = 1;
    						}
    						else
    						{
    							if (Demo.fAudioRecord)
    							{
    								xil_printf("\r\nStill Recording...");
    							}
    							else
    							{
    								xil_printf("\r\nStill Playing back...");
    							}
    						}
    						break;
    					case 'l':
    						if (!Demo.fAudioRecord && !Demo.fAudioPlayback)
    						{
    							xil_printf("\r\nStart Playback...");
    							fnSetLineOutput();
    							fnAudioPlay(sAxiDma,NR_AUDIO_SAMPLES);
    							Demo.fAudioPlayback = 1;
    						}
    						else
    						{
    							if (Demo.fAudioRecord)
    							{
    								xil_printf("\r\nStill Recording...");
    							}
    							else
    							{
    								xil_printf("\r\nStill Playing back...");
    							}
    						}
    						break;
    					default:
    						break;
    				}

    				// Reset the user I/O flag
    				Demo.chBtn = 0;
    				Demo.fUserIOEvent = 0;
    			}

    	usleep(20000);
    }

	xil_printf("\r\n--- Exiting main() --- \r\n");


	return XST_SUCCESS;

}









