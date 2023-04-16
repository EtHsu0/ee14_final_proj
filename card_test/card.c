#include "card.h"
#include <stdlib.h>


static Card *create_card(Suit type, int value) {
	Card *new_card = malloc(sizeof(Card *));
	new_card->type = type;
	new_card->value = value;
	return new_card;
}

// This function assume that srand has been setup
// This function assume user give a valid pointer
void Card_Init(void *buf) {
	Card *shoe = malloc((NUM_DECK * NUM_CARD) * sizeof(Card));
	
    uint8_t idx = 0;
	// First generate all the card;
	for (uint8_t i = 0; i < NUM_SUIT; i++) {
        for (int j = 0; j < NUM_CARD_EACH_SUIT; j++) {
            shoe[idx++] = create_card(i, j);
        }
    }

    for (uint8_t i = 0; i < NUM_DECK * NUM_CARD - 2; i++) {
        j = rand() % (NUM_DECK * NUM_CARD);
        Card *temp = shoe[i];
        shoe[i] = shoe[j];
        shoe[j] = temp;
    }
}