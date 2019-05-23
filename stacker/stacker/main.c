#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include "io.c"
//#include "ledmatrix7219d88/ledmatrix7219d88.h"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

//GLOBAL VARIABLES
#define MAX_LEVEL 16;
unsigned char grid[MAX_LEVEL];
unsigned char speeds[MAX_LEVEL] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char sizes[MAX_LEVEL] = {3,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1};
unsigned char level, curr_size, curr_speed, curr_row, button, reset,oscillate;

//Checks to see if the stack landed on top of the previous stack
unsigned char checkHits(unsigned char row, unsigned char lvl){
	if(lvl == 0)
		return 0x01;
	
	return !(row & grid[lvl - 1]);
}
//Returns the size of the new blocks
unsigned char getNewSize(unsigned char row, unsigned char lvl){
	unsigned char count = 0;
	unsigned char temp = row & grid[lvl - 1];
	
	while(temp != 0){
		if((temp & 0x01) == 1){
			count++;
		}
		temp = temp >> 1;
	}
	
	return count;
}
//
unsigned char setCurrentRow(unsigned char size,unsigned char lvl){
	unsigned char new_size = getNewSize(curr_row,lvl);
	//TODO
	return new_size;
}
void displayGrid(){
	for(unsigned char i = 0; i < MAX_LEVEL/8; i++){
		for(unsigned char j = 0; j < 8; i++){
			//ledmatrix7219d88_setrow(i, j, grid[8*i + j]);
		}
	}
}
void clearGrid(){
	for(unsigned char i = 0; i < MAX_LEVEL; i++){
		grid[i] = 0x00;
	}
}

//STATE ENUMS
enum GameLogicSM{GL_Start,Wait,Oscillate,Press,End}GAMESTATE;
enum

void Tick_GameLogic(){

	switch(GAMESTATE){ //State Transitions
		case GL_Start:
			GAMESTATE = (button)? Wait : GL_Start;
		break;
		case Wait:
			if(reset){
				GAMESTATE = GL_Start;
			}else if(button){
				GAMESTATE = Wait;
			}else{
				GAMESTATE = Oscillate;
				curr_row = 0x38;
			}
		break;
		case Oscillate:
			if(reset){
				GAMESTATE = GL_Start;
			}else if(button){
				GAMESTATE = Press;
			}else{
				GAMESTATE = Oscillate;
			}
		break;
		case Press:
			if(reset){
				GAMESTATE = GL_Start;
			}
		break;
		case End:
			if(reset){
				GAMESTATE = GL_Start;
			}
		break;
		default:
		GAMESTATE = GL_Start;
		break;
	}

	switch(GAMESTATE){ //State Actions
		case GL_Start:
			clearGrid();
			level = 0;
			curr_size = 0;
			curr_speed = speeds[0];
		break;
		case Wait:
		break;
		case Oscillate:
		break;
		case Press:
		break;
		case End:
		break;
		default:break;
	}




}

int main(void)
{	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(100);
	TimerOn();

	//ledmatrix7219d88_init();

	GAMESTATE = GL_Start;

    while (1) 
    {
		button = ~PINA & 0x01;
		reset  = ~PINA & 0x02;

		Tick_GameLogic();

		while(!TimerFlag);
		TimerFlag = 0;

    }
}