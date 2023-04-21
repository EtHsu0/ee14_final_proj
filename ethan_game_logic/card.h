#ifndef _CARD_H
#define _CARD_H
#include "stm32l476xx.h"
#include "UART.h"
#include "LED.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_DECK 6
#define NUM_CARD (NUM_SUIT * NUM_CARD_EACH_SUIT)
#define NUM_CARD_EACH_SUIT 13
#define NUM_SUIT 4

typedef enum {CLUB = 0, DIAMOND = 1, HEART = 2, SPADE = 3} Suit;

// Each Card is 8 byte
typedef struct{
	Suit type;
	bool faceDown;
	uint8_t value; // From 1 - 13, 9 will indicate noVal
} Card;

typedef struct{
	Card **deck; // pointer to cards
	uint16_t index;
	uint16_t size;
	uint16_t capacity;
} Deck;
// This function assume that srand has been setup
// This function assume user give a valid pointer
void Card_Init(void **buf);

Card *Card_draw(Deck *deck);

Deck *Deck_init(uint16_t capacity);


#endif
