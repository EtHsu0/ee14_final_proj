#ifndef __PRINT_CARD_H
#define __PRINT_CARD_H

#include "stm32l476xx.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "UART.h"

typedef enum {CLUB = 0, DIAMOND = 1, HEART = 2, SPADE = 3} Suit;

typedef struct{
	Suit type;
	bool faceDown;
	uint8_t value; // From 1 - 13, 9 will indicate noVal
} Card;

void printCard(Card *cards);
void printFacedownCard();

#endif /* __PRINT_CARD_H */