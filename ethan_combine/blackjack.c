
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



static void person_draw(Person *person, bool faceDown) {
    Card *temp_card;
    temp_card = Card_draw(shoe);
    if (temp_card == NULL)
        Red_LED_On();
    temp_card->faceDown = faceDown;
    person->hand->deck[person->hand->size++] = temp_card;
}


static void round_start() {
    person_draw(player, false);
    person_draw(dealer, true);
    person_draw(player, false);
    person_draw(dealer, false);
}

static void round_end() {
    player->hand->size = 0;
    dealer->hand->size = 0;
    player->bets = 0;
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
    printCard(dealer->hand->deck, dealer->hand->size);
    sprintf(bet_msg, "Your bet: %u\r\n", player->bets);
    USART_Print(bet_msg);
    USART_Print("Your hand:\r\n");
    printCard(player->hand->deck, player->hand->size);
    USART_Print("\r\n");
}

static void delay(uint32_t i) {
    while (i > 0)
        i--;
}


static void bj_read_bet() {
    uint32_t user_input_number = UINT32_ERROR;
    bool new_round = true;

    char *data = malloc(500);
    char newline[] = "\r\n";

	char input_msg[] = "How many chips would you like to wager?\r\n";
    char reinput_msg[] = "\nPlease re-enter the number of chips you'd like to wager.\r\n";
    char NaN_msg[] = "Your bet is invalid.\r\n";
    char neg_msg[] = "You are betting more chips than you currently have.\r\n";

    char confirm_msg[100];

    while (user_input_number == UINT32_ERROR) {
        new_round ? USART_Print(input_msg) : USART_Print(reinput_msg);
        new_round = false;

        USART_Readaline(&data);
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

        sprintf(confirm_msg, "You are betting [%d] chips, type Y to confirm, type N to provide another bet.\r\n", user_input_number);
        USART_Print(confirm_msg);
        while(1) {
            USART_Readaline(&data);
            USART_Print(data);
            USART_Print(newline);
            to_lower(data);

            if (strcmp(data, "y") == 0 || strcmp(data, "yes") == 0) {
                player->bets = user_input_number;
                break;
            }
            else if (strcmp(data, "n") == 0 || strcmp(data, "no") == 0){
                user_input_number = UINT32_ERROR;
                break;
            }
        }
    }
}


void bj_run() {
    bool new_round, while_state, blackjack;
    bool round_lose;
    uint8_t cards_sum = 0;
    char *data = malloc(1000);
    char newline[] = "\r\n";\

    uint32_t round_num = 0;

    char round_end_msg[] = "The round has ended, press enter to continue.\r\n";
    char round_msg[20];
    char hit_msg[] = "Type Hit (H) to draw another card, and Stand (S) to end your turn.\r\n";
    char debug[] = "This is a debug message: end of while loop\r\n";

    bj_init();

    while(1) {
        new_round = true;
        LCD_DisplayNum(player->tokens);

        sprintf(round_msg, "\033c<<Round %u>>\r\n", ++round_num);
        USART_Print(round_msg);

        bj_read_bet();

        player->tokens -= player->bets;
        LCD_DisplayNum(player->tokens);

        round_start();
        while_state = true;
        new_round = true;
        round_lose = false;
        blackjack = false;
        while(while_state) {
            if (new_round)
                new_round = false;
            else {
                person_draw(player, false);
            }
            print_board();
            
            cards_sum = hand_sum(player);

            if (player->hand->size == 2 && cards_sum == 21) {
                USART_Print("BLACKJACK!!\r\n");
                blackjack = true;
                break;
            }
            if (cards_sum > 21) {
                USART_Print("BUSTED!!\r\n");
                round_lose = true;
                break;
            }

            USART_Print(hit_msg);
            while(1) {
                USART_Readaline(&data);
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
        
        dealer->hand->deck[0]->faceDown = false;
        if (blackjack) {
            player->tokens += player->bets * 2 + player->bets / 2;
        }
        else if (!round_lose) {
            print_board();

            while(hand_sum(dealer) <= 16) {
                person_draw(dealer, false);
                delay(20000000);
                print_board();
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

       delay(20000000);
        USART_Print("[ROUND END]\r\n");
        print_board();

        if (player->tokens == 0) {
            USART_Print("You ran out of tokens. Game over :(\r\n");
            USART_Print("PRESS THE BLACK BUTTON TO RESTART\r\n");
            while(1);
        }

        round_end();
        USART_Print(newline);
        USART_Print(round_end_msg);

        USART_Readaline(&data);
        USART_Write(USART2, (uint8_t *)debug, strlen(debug));
	}

}
