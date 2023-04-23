

#include "printCard.h"


char *facedownCard[] = { " _____ ", "|\\ ~ /|", "|}}:{{|", "|}}:{{|", "|}}:{{|", "|/_~_\\|" };

// Hard Coded Aces
char *aceOfClubs[] = {" _____ ", "|A _  |", "| ( ) |", "|(_'_)|", "|  |  |", "|____A|"};
char *aceOfDiamonds[] = {" _____ ", "|A ^  |", "| / \\ |", "| \\ / |", "|  .  |", "|____A|"};
char *aceOfHearts[] = {" _____ ", "|A_ _ |", "|( v )|", "| \\ / |", "|  .  |", "|____A|"};
char *aceOfSpades[] = {" _____ ", "|A .  |", "| /.\\ |", "|(_._)|", "|  |  |", "|____A|"};

// Map the enum suit values to this array of characters
uint8_t *suits[] = {(uint8_t*)"&", (uint8_t*)"o", (uint8_t*)"v", (uint8_t*)"^"};

int cardPrintLen = 7;
int cardPrintHeight = 6;


void printCard(Card **cards, int numCards) {
	int i, j;
	
	// Print first lines
	for (i = 0; i < numCards; i++) {
		USART_Write(USART2, (uint8_t *) " _____ ", cardPrintLen);
	}
	USART_Write(USART2, (uint8_t *)"\r\n", 2);
	
	
	// Print second lines
	for (i = 0; i < numCards; i++) {
		if (cards[i]->faceDown) {
			printFacedownCard(1);
		}
		else if (cards[i]->value == 1) {
			printAceLine(1, cards[i]->type);
		}
		else {
			printNumberTop(cards[i]->value, suits[cards[i]->type]);
		}
	}
	USART_Write(USART2, (uint8_t *)"\r\n", 2);
	
	// Print 3rd lines
	for (i = 0; i < numCards; i++) {
		if (cards[i]->faceDown) {
			printFacedownCard(2);
		}
		else if (cards[i]->value == 1) {
			printAceLine(2, cards[i]->type);
		}
		else if (cards[i]->value > 10 && cards[i]->value < 14) {
			printFacecardLine(cards[i]->value, cards[i]->type, 2);
		}
		else {
			printCardLineIcons(((cards[i]->value + 2) / 5) + 1, suits[cards[i]->type]); 
		}
	}
	USART_Write(USART2, (uint8_t *)"\r\n", 2);
	
	// Print 4th lines
	for (i = 0; i < numCards; i++) {
		if (cards[i]->faceDown) {
			printFacedownCard(3);
		}
		else if (cards[i]->value == 1) {
			printAceLine(3, cards[i]->type);
		}
		else if (cards[i]->value > 10 && cards[i]->value < 14) {
			printFacecardLine(cards[i]->value, cards[i]->type, 3);
		}
		else if (cards[i]->value <= 5){
			printCardLineIcons(0 + cards[i]->value/5, suits[cards[i]->type]); 
		}
		else if (cards[i]->value == 6 || cards[i]->value == 8) {
			printCardLineIcons(2, suits[cards[i]->type]); 
		}
		else {
			printCardLineIcons(3, suits[cards[i]->type]); 
		}
	}
	USART_Write(USART2, (uint8_t *)"\r\n", 2);
	
	
	// Print 5th lines
	for (i = 0; i < numCards; i++) {
		if (cards[i]->faceDown) {
			printFacedownCard(4);
		}
		else if (cards[i]->value == 1) {
			printAceLine(4, cards[i]->type);
		}
		else if (cards[i]->value > 10 && cards[i]->value < 14) {
			printFacecardLine(cards[i]->value, cards[i]->type, 4);
		}
		else {
			printCardLineIcons(cards[i]->value/4 + 1, suits[cards[i]->type]); 
		}
	}
	USART_Write(USART2, (uint8_t *)"\r\n", 2);
	
	
		// Print last lines
	for (i = 0; i < numCards; i++) {
		if (cards[i]->faceDown) {
			printFacedownCard(5);
		}
		else if (cards[i]->value == 1) {
			printAceLine(5, cards[i]->type);
		}
		else {
			printNumberBottom(cards[i]->value, suits[cards[i]->type]);
		}
	}
	USART_Write(USART2, (uint8_t *)"\r\n", 2);
	
	
	return;
}

// Prints Line 2
void printNumberTop(int value, uint8_t *suitIcon) {
	uint8_t *printNumber = malloc(sizeof(uint8_t) * 1);
	uint8_t number = (uint8_t)'0';
	
	USART_Write(USART2, (uint8_t *) "|", 1);
	
	// 10, Ace, and Face Cards are special
	if (value == 10) {
		USART_Write(USART2, (uint8_t *) "10 ", 3);
		USART_Write(USART2, suitIcon, 3);
		USART_Write(USART2, (uint8_t *) " ", 1);
	}
	else if (value == 11) {
		USART_Write(USART2, (uint8_t *) "J  ww", 5);
	}
	else if (value == 12) {
		USART_Write(USART2, (uint8_t *) "Q  ww", 5);
	}
	else if (value == 13) {
		USART_Write(USART2, (uint8_t *) "K  WW", 5);
	}
	else {
		printNumber[0] = number + value;
		USART_Write(USART2, printNumber, 1);
		USART_Write(USART2, (uint8_t *) "    ", 4);
	}
	
	USART_Write(USART2, (uint8_t *) "|", 1);
}

// Prints Line 6
void printNumberBottom(int value, uint8_t *suitIcon) {
	uint8_t *printNumber = malloc(sizeof(uint8_t) * 1);
	uint8_t number = (uint8_t)'0';
	
	USART_Write(USART2, (uint8_t *) "|", 1);
	
	// 10, Ace, and Face Cards are special
	if (value == 10) {
		USART_Write(USART2, (uint8_t *) "___10", 5);
	}
	else if (value == 11) {
		USART_Write(USART2, (uint8_t *) "__%%J", 5);
	}
	else if (value == 12) {
		USART_Write(USART2, (uint8_t *) "_%%%Q", 5);
	}
	else if (value == 13) {
		USART_Write(USART2, (uint8_t *) "_%%%K", 5);
	}
	else {
		printNumber[0] = number + value;
		USART_Write(USART2, (uint8_t *) "____", 4);
		USART_Write(USART2, printNumber, 1);
	}
	
	USART_Write(USART2, (uint8_t *) "|", 1);
}

// Function to print a line of the hard coded aces
void printAceLine(int lineNum, int suit) {
	if (suit == 0) {
		USART_Write(USART2, (uint8_t *) aceOfClubs[lineNum], 7);
		return;
	}
	if (suit == 1) {
		USART_Write(USART2, (uint8_t *) aceOfDiamonds[lineNum], 7);
		return;
	}
	if (suit == 2) {
		USART_Write(USART2, (uint8_t *) aceOfHearts[lineNum], 7);
		return;
	}
	USART_Write(USART2, (uint8_t *) aceOfSpades[lineNum], 7);
	return;
}

// helper to print lines 3-5
void printCardLineIcons(int numOfIcons, uint8_t *icon) {
	USART_Write(USART2, (uint8_t *) "|", 1);
	
	// If no icons, print spaces
	
	if (numOfIcons == 1) {
		USART_Write(USART2, (uint8_t *) "  ", 2);
		USART_Write(USART2, icon, 1);
		USART_Write(USART2, (uint8_t *) "  ", 2);
	}
	
	else if (numOfIcons == 2) {
		USART_Write(USART2, (uint8_t *) " ", 1);
		USART_Write(USART2,  icon, 1);
		USART_Write(USART2, (uint8_t *) " ", 1);
		USART_Write(USART2, icon, 1);
		USART_Write(USART2, (uint8_t *) " ", 1);
	}
	
	else if (numOfIcons == 3) {
		USART_Write(USART2,  icon, 1);
		USART_Write(USART2, (uint8_t *) " ", 1);
		USART_Write(USART2,  icon, 1);
		USART_Write(USART2, (uint8_t *) " ", 1);
		USART_Write(USART2,  icon, 1);
	}
	else {
		USART_Write(USART2, (uint8_t *) "     ", 5);
	}
	
	USART_Write(USART2, (uint8_t *) "|", 1);
	
}

void printFacecardLine(int value, Suit suit, int rowNum) {
	USART_Write(USART2, (uint8_t *) "|", 1);
	if (rowNum == 2) {
		uint8_t* symbols[] = {(uint8_t *)"o ", (uint8_t *)"/\\", (uint8_t *)"  ", (uint8_t *)"^ "};
		USART_Write(USART2, (uint8_t *) " ", 1);
		USART_Write(USART2, symbols[suit], 2);
		USART_Write(USART2, (uint8_t *) "{", 1);
		if (value == 12) {
			USART_Write(USART2, (uint8_t *) "(", 1);
		}
		else {
			USART_Write(USART2, (uint8_t *) ")", 1);
		}
	}
	
	else if (rowNum == 3) {
		uint8_t* symbols[] = {(uint8_t *)"o o", (uint8_t *)" \\/", (uint8_t *)"(v)", (uint8_t *)"(.)"};
		USART_Write(USART2, (uint8_t *) symbols[suit], 3);
		if (value == 11) {
			USART_Write(USART2, (uint8_t *) "% ", 2);
		}
		else {
			USART_Write(USART2, (uint8_t *) "%%", 2);
		}
	}
	
	else if (rowNum == 4) {
		uint8_t* symbols[] = {(uint8_t *)"|", (uint8_t *)" ", (uint8_t *)"v", (uint8_t *)"|"};
		USART_Write(USART2, (uint8_t *) " ", 1);
		USART_Write(USART2, (uint8_t *) symbols[suit], 1);
		if (value == 11) {
			USART_Write(USART2, (uint8_t *) "%  ", 3);
		}
		else {
			USART_Write(USART2, (uint8_t *) "%%%", 3);
		}
		
	}
	USART_Write(USART2, (uint8_t *) "|", 1);
	
}


void printFacedownCard(int rowNum) {
	
	USART_Write(USART2, (uint8_t *)facedownCard[rowNum], cardPrintLen);
	
}