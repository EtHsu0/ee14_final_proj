#include "card.h"
#include <stdlib.h>





Card *create_card(Suit type, int value) {
	Card *new_card = malloc(sizeof(Card *));
	new_card->type = type;
	new_card->value = value;
	return new_card;
}

// This function assume that srand has been setup
// This function assume user give a valid pointer
void Card_Init(void *buf) {
	uint8_t i,j;
	Card **shoe = malloc((NUM_DECK * NUM_CARD) * sizeof(Card *));
	Card *temp;
    uint8_t idx = 0;
	// First generate all the card;
	for (i = 0; i < NUM_SUIT; i++) {
        for (j = 0; j < NUM_CARD_EACH_SUIT; j++) {
            shoe[idx++] = create_card(i, j);
        }
    }

    for (i = 0; i < NUM_DECK * NUM_CARD - 2; i++) {
        j = rand() % (NUM_DECK * NUM_CARD);
        temp = shoe[i];
        shoe[i] = shoe[j];
        shoe[j] = temp;
    }

    buf = (void *) shoe;
}
