
#include "blackjack.h"

#define UINT32_ERROR ((uint32_t)UINT32_MAX)

Deck *shoe;
Person *dealer;
Person *player;

static void bj_init() {
    Card_Init((void *)shoe);
    
    dealer = malloc(sizeof(Person *));
    dealer->isDealer = true;
    dealer->hand = Deck_init(10);

    player = malloc(sizeof(Person *));
    player->isDealer = false;
    player->hand = Deck_init(10);
    player->tokens = 1000;
    player->bets = 0;
}

static uint32_t convert_string_to_num(char *data) {
    uint16_t i, len;
    uint32_t result;
    char *str;

    if (data == NULL)
        return UINT32_ERROR;
    
    len = strlen(data);

    for (i = 0; i < len; i++) {
        if (!isdigit(data[i])) {
            return UINT32_ERROR;
        }
    }

    result = atoi(str);

    return result;
}

void bj_run() {
    Card *card;
    uint8_t len, test, i;
    uint16_t user_input_number;
    char *data = malloc(1000);
    char newline[] = "\r\n";
	char input_msg[] = "Please enter the amount of token you want to bet\r\n";
    char hit_msg[] = "Press H to hit\r\n";
    char debug[] = "This is a debug message: end of while loop\r\n";
    char test_str[1000];

    bj_init();


    while(1) {
    USART_Write(USART2, (uint8_t *)input_msg, strlen(input_msg));
		//sprintf(str, "\n%u\r\n", test);
		//USART_Write(USART2, (uint8_t *)str, strlen(str));
	    
		len = USART_Readaline(&data);
		


		//USART_Write(USART2, (uint8_t *)debug, strlen(debug));
		
		USART_Write(USART2, (uint8_t *)data, len);
    
        USART_Write(USART2, (uint8_t *)newline, strlen(newline));
		
  
        
          
        GPIOB->ODR ^= GPIO_ODR_ODR_2;
        card = Card_draw(shoe);
          GPIOE->ODR ^= GPIO_ODR_ODR_8;

        test = card->value;
       
	  
        sprintf(test_str, "Value: %u\r\n", test);
        GPIOB->ODR ^= GPIO_ODR_ODR_2;
        USART_Write(USART2, (uint8_t *)test_str, strlen(test_str));

        
        USART_Write(USART2, (uint8_t *)hit_msg, strlen(hit_msg));
        len = USART_Readaline(&data);
        USART_Write(USART2, (uint8_t *)data, len);

        USART_Write(USART2, (uint8_t *)newline, strlen(newline));
      

        // free(&data);
	 

		//

        USART_Write(USART2, (uint8_t *)debug, strlen(debug));
	}

}
