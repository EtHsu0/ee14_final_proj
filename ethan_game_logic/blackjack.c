
#include "blackjack.h"

#define UINT32_ERROR ((uint32_t)UINT32_MAX)

Deck *shoe;
Person *dealer;
Person *player;



uint32_t round_num = 0;

static void bj_init() {
    Card_Init((void *)&shoe);
    
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
    result = 0;
    if (data == NULL)
        Red_LED_On();
    if (data == "")
        return UINT32_ERROR;
    len = strlen(data);

    for (i = 0; i < len; i++) {
        if (data[i] < '0' || data[i] > '9') {
            return UINT32_ERROR;
        }
    }

    for (i = 0; data[i] != 0; i++) {
        result = result * 10 + (data[i] - '0');
    }

    return result;
}

void bj_run() {
    Card *card;
    bool new_round, while_state;
    uint16_t len, test, i;
    uint32_t user_input_number;
    char *data = malloc(1000);
    char newline[] = "\r\n";
	char input_msg[] = "\nPlease enter the amount of token you want to bet.\r\n";
    char reinput_msg[] = "\nPlease re-enter the amount of token you want to bet.\r\n";
    char NaN_msg[] = "Your bet is invalid.\r\n";
    char hit_msg[] = "Press H to hit\r\n";
    char neg_msg[] = "You are betting more than you have.\r\n";
    char debug[] = "This is a debug message: end of while loop\r\n";
    char test_str[100];
    char confirm_msg[100];

    bj_init();


    while(1) {
        new_round = true;
        user_input_number = UINT32_ERROR;
        LCD_DisplayNum(player->tokens);

        while (user_input_number == UINT32_ERROR) {
            if (new_round)
                USART_Print(input_msg);
            else
                USART_Print(reinput_msg);
            new_round = false;


            len = USART_Readaline(&data);
            USART_Print(data);
            USART_Print(newline);
            user_input_number = convert_string_to_num(data);

            sprintf(test_str, "Value of the user: %u\r\n", user_input_number);
          // USART_Print(test_str);
            if (user_input_number == UINT32_ERROR || user_input_number == 0) {
                USART_Print(NaN_msg);
                user_input_number = UINT32_ERROR;
                continue;
            }
            if (user_input_number > player->tokens) {
                USART_Print(neg_msg);
                user_input_number = UINT32_ERROR;
                continue;
            }

            sprintf(confirm_msg, "You are betting [%d], press Y to confirm, press N to provide another bet.\r\n", user_input_number);
            USART_Print(confirm_msg);
            
            while(1) {
                len = USART_Readaline(&data);
                USART_Print(data);
                USART_Print(newline);
                for (i = 0; i < len; i++) {
                    if (data[i] >= 'A' && data[i] <= 'Z')
                        data[i] += ('a' - 'A');
                }

                if (strcmp(data, "y") == 0 || strcmp(data, "yes") == 0) {
                    break;
                }
                else if (strcmp(data, "n") == 0 || strcmp(data, "no") == 0){
                    user_input_number = UINT32_ERROR;
                    break;
                }
            }

        }
        
        player->tokens += user_input_number;

        card = Card_draw(shoe);
        if (card == NULL)
            Red_LED_On();

        sprintf(test_str, "Value of the card: %u\r\n", card->value);
        USART_Print(test_str);

        
        // USART_Print(hit_msg);
        // len = USART_Readaline(&data);
        // USART_Print(data);
        // USART_Print(newline);
      

        // free(&data);
	 

		//

        USART_Write(USART2, (uint8_t *)debug, strlen(debug));
	}

}
