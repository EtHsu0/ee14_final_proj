#ifndef _BJ_H
#define _BJ_H
#include "stm32l476xx.h"
#include "card.h"
#include "UART.h"
#include "LED.h"
#include "LCD.h"
#include "printCard.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_HAND 10

typedef struct {
    bool isDealer; // true or false
    Deck *hand;
    uint32_t tokens; // only when isDealer == false
    uint32_t bets; // only when isDealer == false
} Person;

void bj_run(void);


#endif
