/******************************************************************************/
/*                                                                            */
/* main.c -- Example program using the PmodBT2 IP                            */
/*                                                                            */
/******************************************************************************/
/* Author: Arthur Brown                                                       */
/*                                                                            */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This demo continuously polls the Pmod BT2 and host development board's     */
/* UART connections and forwards each character from each to the other.       */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    10/04/2017(artvvb):  Created                                            */
/*                                                                            */
/******************************************************************************/

#include "xil_cache.h"
#include "xparameters.h"
#include "PmodBT2.h"

//required definitions for sending & receiving data over the host board's UART port
#ifdef __MICROBLAZE__
#include "xuartlite.h"
typedef XUartLite SysUart;
#define SysUart_Send            XUartLite_Send
#define SysUart_Recv            XUartLite_Recv
#define SYS_UART_DEVICE_ID      XPAR_AXI_UARTLITE_0_DEVICE_ID
#define BT2_UART_AXI_CLOCK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ
#else
#include "xuartps.h"
typedef XUartPs SysUart;
#define SysUart_Send            XUartPs_Send
#define SysUart_Recv            XUartPs_Recv
#define SYS_UART_DEVICE_ID      XPAR_PS7_UART_1_DEVICE_ID
#define BT2_UART_AXI_CLOCK_FREQ 100000000
#endif

PmodBT2 myDevice1, myDevice2;
SysUart myUart;

void DemoInitialize();
void DemoRun();
void SysUartInit();
void EnableCaches();
void DisableCaches();

int main()
{
    DemoInitialize();
    DemoRun();
    DisableCaches();
    return XST_SUCCESS;
}

void DemoInitialize()
{
    EnableCaches();
    SysUartInit();
    BT2_Begin (
        &myDevice1,
        XPAR_PMODBT2_0_AXI_LITE_GPIO_BASEADDR,
        XPAR_PMODBT2_0_AXI_LITE_UART_BASEADDR,
        BT2_UART_AXI_CLOCK_FREQ,
        115200
    );
    BT2_Begin(
    	&myDevice2,
		XPAR_PMODBT2_1_AXI_LITE_GPIO_BASEADDR,
		XPAR_PMODBT2_1_AXI_LITE_UART_BASEADDR,
		BT2_UART_AXI_CLOCK_FREQ,
		115200
    );
}

void DemoRun()
{
    u8 buf[1];
    //u8 buffer[1];
    int n, k, setup;
    k = 1;
    setup = 0;
    //char prompt[] = "Sending multiple characters\n";
    xil_printf("Initialized PmodBT2 Demo, received data will be echoed here, type to send data\r\n");

    //Configuration setup
    if (setup == 1){
    	while(1){
    		n = SysUart_Recv(&myDevice1, buf, 1);
    		n = SysUart_Recv(&myDevice2, buf, 1);
    	}
    }
    while(1) {

        if (k == 1) {
        	n = SysUart_Recv(&myUart, buf, 1);
        	while (n != 0){
        		xil_printf("Sent from BT1 on JA: %c\r\n", (char) buf[0]);
        		BT2_SendData(&myDevice1, buf, 1);
        		n = SysUart_Recv(&myUart, buf, 1);
        		k = 1;
        	}
        }
        /*
        else if (k == 2) {
        	n = BT2_RecvData(&myDevice2, buf, 1);
        	while (n != 0){
        		xil_printf("Received from BT2 on JB: %c\r\n", (char) buf[0]);
        		n = BT2_RecvData(&myDevice2, buf, 1);
        		k = 1;
        	}
        }*/
    }
}

//initialize the system uart device, AXI uartlite for microblaze, uartps for Zynq
void SysUartInit()
{
#ifdef __MICROBLAZE__
    XUartLite_Initialize(&myUart, SYS_UART_DEVICE_ID);
#else
    XUartPs_Config *myUartCfgPtr;
    myUartCfgPtr = XUartPs_LookupConfig(SYS_UART_DEVICE_ID);
    XUartPs_CfgInitialize(&myUart, myUartCfgPtr, myUartCfgPtr->BaseAddress);
#endif
}

void EnableCaches()
{
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheEnable();
#endif
#endif
}

void DisableCaches()
{
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheDisable();
#endif
#endif
}
