#include "stm32l476xx.h"

#include "LED.h"
#include "UART.h"
#include "SysClock.h"
#include "Joypad.h"
#include "blackjack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
	char start_msg[] = "\033cPress blue button to start\r\n";

	System_Clock_Init();
	LED_Init();
	
	UART2_Init();

	Joypad_Init();
	
	USART_Write(USART2, (uint8_t *)start_msg, strlen(start_msg));	
	
	srand(wait_for_first_hit());
	
	bj_run();

	while(1);
}


