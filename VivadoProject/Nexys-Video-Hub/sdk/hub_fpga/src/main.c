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
#include "servo_con3.h"
#include "PmodMAXSONAR.h"

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

#define SERVO_CON_BASEADDRESS 0x44a00000
#define SSRELAY_BASEADDRESS 0x44a10000
#define MAX_SONAR_BASEADDRESS 0x00025000

PmodBT2 myDevice1, myDevice2;
PmodMAXSONAR mySonar;
SysUart myUart;
volatile unsigned int *servo_con_ptr = (unsigned int*) SERVO_CON_BASEADDRESS;
volatile unsigned int *ssr_ptr = (unsigned int*) SSRELAY_BASEADDRESS;

void DemoInitialize();
void DemoRun();
void SysUartInit();
void EnableCaches();
void DisableCaches();
void DoorOpen(volatile unsigned int*);
void DoorClose(volatile unsigned int*);
void LockUp(volatile unsigned int*);
void LockDown(volatile unsigned int*);
void LightOn(volatile unsigned int*);
void LightOff(volatile unsigned int*);
void Enter(	PmodMAXSONAR mySonar,
			volatile unsigned int* ssr_ptr,
			volatile unsigned int* servo_con_ptr
			);

int main()
{
	*(servo_con_ptr + 1) = 0x8;
	*ssr_ptr = 0x2;
	DemoInitialize();
    DemoRun();
    DisableCaches();
    return XST_SUCCESS;
}

void DoorOpen(volatile unsigned int* servo_con_ptr)
{
	xil_printf("servo_1 value: %x\n", *(servo_con_ptr));
	int open_pos = 0xD;
	if (*(servo_con_ptr) == open_pos){
		return;
	}
	int diff = open_pos - (*(servo_con_ptr));
    for (int i = 0; i < diff * 200000; i++){
    	if (i % 200000 == 0){
    	    *(servo_con_ptr) += 0x1;
    	    xil_printf("servo_1 value: %x\n", *(servo_con_ptr));
    	}
    }
}

void DoorClose(volatile unsigned int* servo_con_ptr)
{
	if (*(servo_con_ptr) == 0x2){
		return;
	}
	int diff = (*(servo_con_ptr)) - 0x2;
    for (int i = 0; i < diff * 200000; i++){
    	if (i % 200000 == 0){
    	    *(servo_con_ptr) -= 0x1;
    	    xil_printf("servo_1 value: %x\n", *(servo_con_ptr));
    	}
    }
}

void LockUp(volatile unsigned int* servo_con_ptr)
{
	if (*(servo_con_ptr) == 0x0){
		return;
	}
	int diff = (*(servo_con_ptr)) - 0x0;
    for (int i = 0; i < diff * 100000; i++){
    	if (i % 100000 == 0){
    	    *(servo_con_ptr) -= 0x1;
    	    xil_printf("servo_2 value: %x\n", *(servo_con_ptr));
    	}
    }
}


void LockDown(volatile unsigned int* servo_con_ptr)
{
	if (*(servo_con_ptr) == 0x8){
		return;
	}
	int diff = 0x8 - (*(servo_con_ptr));
    for (int i = 0; i < diff * 100000; i++){
    	if (i % 100000 == 0){
    	    *(servo_con_ptr) += 0x1;
    	    xil_printf("servo_2 value: %x\n", *(servo_con_ptr));
    	}
    }
}

void LightOn(volatile unsigned int* ssr_ptr)
{
	*(ssr_ptr) = 0x1;
}

void LightOff(volatile unsigned int* ssr_ptr)
{
	*(ssr_ptr) = 0x0;
}

void Enter(
	PmodMAXSONAR mySonar,
	volatile unsigned int* ssr_ptr,
	volatile unsigned int* servo_con_ptr
	)
{
	int THRESHOLD = 250;
	int count = 0;
	u32 dist = MAXSONAR_getDistance(&mySonar);
	while (count < THRESHOLD)
	{
		xil_printf("sonar value: %3d\n", dist);
		dist = MAXSONAR_getDistance(&mySonar);
		if (dist <= 8 && dist >= 6){
			count++;
		}
	}
	LightOn(ssr_ptr);
	LockUp(servo_con_ptr + 1);
	DoorOpen(servo_con_ptr);
	count = 0;
	while (count < THRESHOLD)
	{
		xil_printf("sonar value: %3d\n", dist);
		dist = MAXSONAR_getDistance(&mySonar);
		if (dist > 8 || dist < 6){
			count++;
		}
	}
	LightOff(ssr_ptr);
	DoorClose(servo_con_ptr);
	LockDown(servo_con_ptr + 1);
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
    MAXSONAR_begin(
    	&mySonar,
		MAX_SONAR_BASEADDRESS,
		BT2_UART_AXI_CLOCK_FREQ);
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
        	n = BT2_RecvData(&myDevice2, buf, 1);
        	while (n != 0){
        		xil_printf("Received from BT2 on JB: %c\r\n", (char) buf[0]);
        		n = BT2_RecvData(&myDevice2, buf, 1);
        		k = 1;
        		if (buf[0] == 'A'){
                	LockUp(servo_con_ptr + 1);
                	DoorOpen(servo_con_ptr);
        		}
        		else if (buf[0] == 'B'){
                	DoorClose(servo_con_ptr);
                	LockDown(servo_con_ptr + 1);
        		}
        		else if (buf[0] == 'C'){
        			LightOn(ssr_ptr);
        		}
        		else if (buf[0] == 'D'){
        			LightOff(ssr_ptr);
        		}
        		else if (buf[0] == 'E'){
                	Enter(mySonar, ssr_ptr, servo_con_ptr);
        		}

        	}
        }
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
