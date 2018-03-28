#include "dma_functions.h"


/************************ Peter's FFT Function ****************************************/
int DMA_FFT(u32 *TxBufferPtr, u32 *RxBufferPtr, XAxiDma sAxiDmaFFT)
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
	//xil_printf("\r\nFFT Start");
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	int Status;
	Status = XAxiDma_SimpleTransfer(&sAxiDmaFFT, (u32)TxBufferPtr,
		4 * 512, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS) {
		xil_printf("\r\nFFT Write Failed");
		return XST_FAILURE;
	}
	while ((XAxiDma_Busy(&sAxiDmaFFT, XAXIDMA_DMA_TO_DEVICE))) {
		/* Wait */
	}

	// Reading 512 output data from FFT IP and store them in RxBufferPtr
	Status = XAxiDma_SimpleTransfer(&sAxiDmaFFT, (u32)RxBufferPtr,
		8 * 512, XAXIDMA_DEVICE_TO_DMA);
	if (Status != XST_SUCCESS) {
		xil_printf("\r\nFFT Read Failed");
		return XST_FAILURE;
	}

	while ((XAxiDma_Busy(&sAxiDmaFFT, XAXIDMA_DEVICE_TO_DMA))) {
		/* Wait */
	}

	//Flush cache
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	return XST_SUCCESS;
}


/************************ Peter's LOG Function ****************************************/
int DMA_LOG(float *TxBufferPtr, float *RxBufferPtr, u32 Length, XAxiDma sAxiDmaLOG)
{
	/*
	*  This function computes the natural logarithm (ln) for an array of Length
	*  Both address pointers should point to float values
	*/

	// Sending 512 input data from TxbufferPtr to the FFT IP

	//Flush cache
	microblaze_flush_dcache();
	microblaze_invalidate_dcache();

	//xil_printf("\r\nLOG Start");
	int Status;
	Status = XAxiDma_SimpleTransfer(&sAxiDmaLOG, (u32)TxBufferPtr,
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
	Status = XAxiDma_SimpleTransfer(&sAxiDmaLOG, (u32)RxBufferPtr,
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
// ---------------- End of Peter's LOG function--------------------------

