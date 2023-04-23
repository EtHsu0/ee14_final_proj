
#include "blackjack.h"

#define UINT32_ERROR ((uint32_t)UINT32_MAX)
#define MAX_HAND 10
Deck *shoe;
Person *player;
Person *dealer;



char *suit_name[] = {"Club", "Diamond", "Heart", "Spade"};


static void bj_init() {
    Card_Init((void *)&shoe);
    
    dealer = malloc(sizeof(Person));
    dealer->isDealer = true;
    dealer->hand = Deck_init(MAX_HAND);

    player = malloc(sizeof(Person));
    player->isDealer = false;
    player->hand = Deck_init(MAX_HAND);
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

static void to_lower(char *data) {
    uint32_t i;
    for (i = 0; data[i] != 0; i++) {
        if (data[i] >= 'A' && data[i] <= 'Z')
            data[i] += ('a' - 'A');
    }
}

static void round_start() {
    Card *temp;
    char debug_msg[30];
    dealer->hand->size = 2;
    player->hand->size = 2;

    temp = Card_draw(shoe);
    if (temp == NULL)
        Red_LED_On();
    temp->faceDown = false;
    player->hand->deck[0] = temp;


    temp = Card_draw(shoe);
    if (temp == NULL)
        Red_LED_On();
    temp->faceDown = true;
    dealer->hand->deck[0] = temp;

    temp = Card_draw(shoe);
    if (temp == NULL)
        Red_LED_On();
    temp->faceDown = false;
    player->hand->deck[1] = temp;
    

    temp = Card_draw(shoe);
    if (temp == NULL)
        Red_LED_On();
    temp->faceDown = false;
    dealer->hand->deck[1] = temp;
}

static void print_hand(Person *person) {
    uint8_t i;
    char print_msg[20];
    char *faceDown_msg = "Face down card.\r\n";
    Green_LED_On();

    for (i = 0; i < person->hand->size; i++) {
    
        if (person->hand->deck[i]->faceDown) 
            USART_Print(faceDown_msg);
        else {
            sprintf(print_msg, "Suit: %s, Value: %u\r\n", suit_name[person->hand->deck[i]->type], person->hand->deck[i]->value);
            USART_Print(print_msg);
        }
    }
    Green_LED_Off();
}

static uint8_t hand_sum(Person *person) {
    uint8_t result, i, num_ace, card_val;
    num_ace = 0;
    result = 0;

    for (i = 0; i < person->hand->size; i++) {
        card_val = person->hand->deck[i]->value;
        if (card_val == 1) {
            num_ace++;
            result += 11;
        }
        else if (card_val > 10)
            result += 10;
        else
            result += card_val;
    }

    while (num_ace > 0 && result > 21) {
        result -= 10;
        num_ace--;
    }

    return result;
}


static void print_board() {
    char bet_msg[30];
    USART_Print("Dealer hand:\r\n");
    print_hand(dealer);
    sprintf(bet_msg, "Your current bet: %u\r\n", player->bets);
    USART_Print(bet_msg);
    USART_Print("Your hand:\r\n");
    print_hand(player);
}

void bj_run() {
    Card *card;
    bool new_round, while_state;
    bool round_lose;
    uint8_t cards_sum = 0;
    uint16_t len, test, i;
    uint32_t user_input_number, bet;
    char *data = malloc(1000);
    char newline[] = "\r\n";\

    Card *temp_card;
    
    uint32_t round_num = 0;

	char input_msg[] = "Please enter the amount of token you want to bet.\r\n";
    char round_end_msg[] = "The round has end, press enter to continue.\r\n";
    char round_msg[20];
    char bet_msg[20];
    char reinput_msg[] = "\nPlease re-enter the amount of token you want to bet.\r\n";
    char NaN_msg[] = "Your bet is invalid.\r\n";
    char hit_msg[] = "Type H to hit, and S to stand.\r\n";
    char neg_msg[] = "You are betting more than you have.\r\n";
    char debug[] = "This is a debug message: end of while loop\r\n";
    char test_str[100];
    char confirm_msg[100];

    bj_init();


    while(1) {
        new_round = true;
        user_input_number = UINT32_ERROR;
        LCD_DisplayNum(player->tokens);

        sprintf(round_msg, "\033c<<Round %u>>\r\n", ++round_num);
        USART_Print(round_msg);
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

            sprintf(confirm_msg, "You are betting [%d], type Y to confirm, type N to provide another bet.\r\n", user_input_number);
            USART_Print(confirm_msg);
            player->bets = user_input_number;
            while(1) {
                len = USART_Readaline(&data);
                USART_Print(data);
                USART_Print(newline);
                to_lower(data);

                if (strcmp(data, "y") == 0 || strcmp(data, "yes") == 0) {
                    break;
                }
                else if (strcmp(data, "n") == 0 || strcmp(data, "no") == 0){
                    user_input_number = UINT32_ERROR;
                    break;
                }
            }
        }

        player->tokens -= player->bets;
        LCD_DisplayNum(player->tokens);
        round_start();
        while_state = true;
        new_round = true;
        round_lose = false;
        while(while_state) {
            if (new_round)
                new_round = false;
            else {
                temp_card = Card_draw(shoe);
                if (temp_card == NULL)
                    Red_LED_On();
                temp_card->faceDown = false;
                player->hand->deck[player->hand->size++] = temp_card;
            }
            print_board();
            
            cards_sum = hand_sum(player);
            if (cards_sum > 21) {
                USART_Print("BUSTED!!\r\n");
                round_lose = true;
                break;
            }
            sprintf(test_str, "Player has total hand of: %u\r\n", cards_sum);
            USART_Print(test_str);
            // Calculate black jack;
            // Test the sum


            USART_Print(hit_msg);
            while(1) {
                len = USART_Readaline(&data);
                USART_Print(data);
                USART_Print(newline);
                to_lower(data);

                if (strcmp(data, "h") == 0 || strcmp(data, "hit") == 0) {
                    break;
                }
                else if (strcmp(data, "s") == 0 || strcmp(data, "stand") == 0){
                    while_state = false;
                    break;
                }
            }
        }
        
//        bool new_round = true;
        dealer->hand->deck[0]->faceDown = false;

        if (!round_lose) {
            print_board();

            while(hand_sum(dealer) <= 16) {
                temp_card = Card_draw(shoe);
                if (temp_card == NULL)
                    Red_LED_On();
                temp_card->faceDown = false;
                dealer->hand->deck[dealer->hand->size++] = temp_card;
                USART_Print("Dealer hand:\r\n");
                print_hand(dealer);
                sprintf(bet_msg, "Your current bet: %u\r\n", player->bets);
                USART_Print(bet_msg);
                USART_Print("Your hand:\r\n");
                print_hand(player);
            }

            if (hand_sum(dealer) == hand_sum(player)) {
                USART_Print("\n{TIE!}\r\n");
                player->tokens += player->bets;
            }
            else if (hand_sum(dealer) > 21) {
                USART_Print("\n{Dealer busted! You win!}\r\n");
                player->tokens += 2 * player->bets;
            }
            else if (hand_sum(dealer) > hand_sum(player)) {
                USART_Print("\n{You lose!}\r\n");
            }
            else {
                USART_Print("\n{You win!}\r\n");
                player->tokens += 2 * player->bets;
            }
        }

        LCD_DisplayNum(player->tokens);

        USART_Print(newline);


        USART_Print("[ROUND END]\r\n");
        print_board();
        USART_Print(round_end_msg);
        USART_Readaline(&data);
        USART_Write(USART2, (uint8_t *)debug, strlen(debug));
	}

}
