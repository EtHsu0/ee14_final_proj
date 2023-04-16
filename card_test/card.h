#ifndef _CARD_H
#define _CARD_H

#include <stdlib.h>
#include <stdint.h>

#define NUM_DECK 6
#define NUM_CARD 52
#define NUM_CARD_EACH_SUIT
#define NUM_SUIT 4

typedef enum {CLUB = 0, DIAMOND = 1, HEART = 2, SPADE = 3} Suit;

typedef struct{
	Suit type;
	uint8_t value; // From 1 - 13, 9 will indicate noVal
} Card;


// This function assume that srand has been setup
// This function assume user give a valid pointer
void Card_Init(void *buf);



#endif