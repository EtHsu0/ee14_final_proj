
#include "UART.h"
#include "PrintCard.h"
#include <string.h>

char *facedownCard[] = { " _____ ", "|\\ ~ /|", "|}}:{{|", "|}}:{{|", "|}}:{{|", "|/_~_\\|" };
int cardPrintLen = 7;
int cardPrintHeight = 6;


void printCard(Card *cards) {
	printFacedownCard();
	return;
}


void printFacedownCard() {
	int i;
	
	
	for (i = 0; i < cardPrintHeight; i++) {
		USART_Write(USART2, (uint8_t *)facedownCard[i], cardPrintLen);
		USART_Write(USART2, (uint8_t *)"\r\n", 2);
	}
	return;
	
}