#include "stm32l476xx.h"

#include "LED.h"

#include "UART.h"
#include <string.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Enter a wager for this round:\r\n";

int main(void){
	char rxByte;
	int a, n, i;
	float b;
	
	//System Clock Initialization
	//LED Initialization
	//SysTick Initialization
	//delay of 1Sec
	//LED Toggle
	System_Clock_Init();
	LED_Init();
	UART2_Init();
	//
	
	

	while(1) {
		USART_Write(USART2, (uint8_t *)str, strlen(str));	
		rxByte = USART_Read(USART2);
		if (rxByte == 'N' || rxByte == 'n'){
			Red_LED_Off();
			USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
		}
		else if (rxByte == 'Y' || rxByte == 'y'){
			Red_LED_On();
			USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
		}
		
	}
}


