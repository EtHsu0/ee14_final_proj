// Ethan main.c

#include "stm32l476xx.h"

#include "LED.h"
#include "LCD.h"
#include "UART.h"
#include "SysClock.h"
#include "Joypad.h"



#include "blackjack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include <main.h>

void my_audio_init(void);
void my_audio_beep(void);

int main(void){
	

	
	char clear[] = "\033c\r\n";

	
// 	char* starting_screen = 
// "    ______    ______   _______  _______  _______  _______  _______  _______  _______ \r\n"
// "  /      \\  /      \\ /       |/       |/       |/       |/       |/       |/       |\r\n"
// " /$$$$$$  |/$$$$$$  |$$$$$$$//$$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ \r\n"
// " $$ |__$$ |$$ |__$$ |  $$ |  /$$_____//$$__  $$/$$_____/ $$_____/ $$_____/ $$ |__$$ \r\n"
// " $$    $$ |$$    $$<   $$ |  | $$     |$$    $$/ $$      /$$        $$      $$    $$< \r\n"
// " $$$$$$$$ |$$$$$$$  |  $$ |  | $$      $$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$  |\r\n"
// " $$ |__$$ |$$ |__$$ |  $$ |  | $$_____ $$ |__  $$ |_____ $$ |_____ $$ |_____ $$ |__$$ |\r\n"
// " $$    $$/ $$    $$/   $$ |  | $$_____|$$    $$/       |$$       |$$       |$$    $$/\r\n"
// " $$$$$$$/  $$$$$$/    $$/   |$$$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ $$$$$$$/ \r\n"
// "                                                                                  \r\n"
// "                          Press the blue joystick button to begin                  \r\n";

	System_Clock_Init();
	LCD_Initialization();
	LED_Init();
	UART2_Init();
	Joypad_Init();
	
	USART_Print(clear);
	// USART_Print(starting_screen);
	
	srand(wait_for_first_hit());
	
	USART_Print(clear);
	
	//imain2();
	my_audio_init(); // initialize audio
	//my_audio_beep();
	
	bj_run();

	while(1);
}


