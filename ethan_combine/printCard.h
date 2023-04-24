#ifndef __PRINT_CARD_H
#define __PRINT_CARD_H

#include "stm32l476xx.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "UART.h"
#include "card.h"

void printCard(Card **cards, int numCards);
void printFacedownCard(int rowNum);
void printCardLineIcons(int numOfIcons, uint8_t *icon);
void printAceLine(int lineNum, int suit);
void printFacecardLine(int value, Suit suit, int rowNum);
void printNumberTop(int value, uint8_t *suitIcon);
void printNumberBottom(int value, uint8_t *suitIcon);

#endif /* __PRINT_CARD_H */
