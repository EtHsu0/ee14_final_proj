#include "stm32l476xx.h"

#include "LED.h"
#include "UART.h"
#include "SysClock.h"
#include "Joypad.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];

int main(void){
	int i;
	char *data;
	int len;
	unsigned int seed;
	
	char start_msg[] = "\033cPress blue button to start\r\n";
	char input_msg[] = "Please enter the amount of token you want to bet\r\n";
	
	
	char str[7];
	uint8_t test;
	System_Clock_Init();
	LED_Init();
	
	UART2_Init();

	Joypad_Init();
	
	USART_Write(USART2, (uint8_t *)start_msg, strlen(start_msg));	
	
	
	seed = wait_for_first_hit();
	srand(seed);
	

	while(1) {
		test = rand() % 10000;
		sprintf(str, "\n%u\r\n", test);
		USART_Write(USART2, (uint8_t *)str, strlen(str));
		
		USART_Write(USART2, (uint8_t *)input_msg, strlen(input_msg));
		len = USART_Readaline(&data);
		
		
		USART_Write(USART2, (uint8_t *)str, strlen(str));
		
		USART_Write(USART2, (uint8_t *)data, len);
		
		for(i = 0; i < 10000000; i++);
	}
}


