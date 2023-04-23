#include "stm32l476xx.h"
#include "LED.h"
#include "UART.h"
#include "PrintCard.h"
#include <string.h>
#include <stdbool.h>

void printCards();

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "CARD TEST\r\n";

int main(void){
	char rxByte;
	int a, n, i;
	float b;
	Card Sample_Card;
	Card Sample_Card2;
	Card Sample_Ace;
	Card *cards = malloc (sizeof(Card) * 3);
	
	//System Clock Initialization
	//LED Initialization
	//SysTick Initialization
	//delay of 1Sec
	//LED Toggle
	System_Clock_Init();
	LED_Init();
	UART2_Init();
	

	while(1) {
		USART_Write(USART2, (uint8_t *)str, strlen(str));	
		rxByte = USART_Read(USART2);
		if (rxByte == 'N' || rxByte == 'n'){
			Red_LED_Off();
			USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
			printCards();
		}
		else if (rxByte == 'Y' || rxByte == 'y'){
			Red_LED_On();
			USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
			printFacedownCard();
		}
		
	}
}

void printCards() {
	int i = 0;
	Card *cards = malloc (sizeof(Card));
	for (i = 0; i < 14; i++) {
		Card Sample_Card;
		Sample_Card.type = CLUB;
		Sample_Card.faceDown = false;
		Sample_Card.value = i;
		cards[0] = Sample_Card;
		printCard(cards, 1);
	}
	
}


