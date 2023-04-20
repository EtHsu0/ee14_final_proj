#include "card.h"
#include <stdlib.h>


Deck *Deck_init(uint16_t capacity) {
    Deck *new_deck = malloc(sizeof(Deck));
    new_deck->deck = malloc(capacity * sizeof(Card));
    new_deck->index = 0;
    new_deck->size = 0;
    new_deck->capacity = capacity;
    return new_deck;
}

static Card *create_card(Suit type, uint8_t value) {
	Card *new_card = malloc(sizeof(Card));
    if (new_card == NULL)
        GPIOB->ODR ^= GPIO_ODR_ODR_2;
	new_card->type = type;
	new_card->value = value;
	return new_card;
}

// This function assume that srand has been setup
// This function assume user give a valid pointer
void Card_Init(void *buf) {
    char test_str[1000];
	uint8_t i,j, k, test;
    
    uint16_t idx = 0;
	Card *temp;

    
	Deck *shoe = Deck_init(NUM_CARD * NUM_DECK);
	// First generate all the card;
    for (k = 0; k < NUM_DECK; k++) {
        for (i = 0; i < NUM_SUIT; i++) {
            for (j = 0; j < NUM_CARD_EACH_SUIT; j++) {
                temp = create_card((Suit)i, j+1);
                shoe->deck[idx] = temp;
                shoe->size++;
                sprintf(test_str, "Value: %u\r\n", (temp)->value);
                USART_Write(USART2, (uint8_t *)test_str, strlen(test_str));
                idx++;
            }
        }
    }
    USART_Write(USART2, (uint8_t *)"Test\r\n", strlen("Test\r\n"));

    for (i = 0; i < shoe->size - 2; i++) {
        j = rand() % (NUM_DECK * NUM_CARD);
        temp = shoe->deck[i];
        shoe->deck[i] = shoe->deck[j];
        shoe->deck[j] = temp;
    }

    buf = (void *) shoe;
}

Card *Card_draw(Deck *deck) {
    Card *ret;

    ret = deck->deck[deck->index];
    deck->index++;

    return ret;
    

}
