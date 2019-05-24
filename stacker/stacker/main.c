#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include "io.c"
//#include "ledmatrix7219d88/ledmatrix7219d88.h"

#pragma region TimerCode
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
#pragma endregion TimerCode

#define MAX_LEVEL 16

unsigned char grid[MAX_LEVEL];
unsigned char speeds[MAX_LEVEL] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char sizes[MAX_LEVEL] = {3,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1};
unsigned char level, curr_size, curr_speed, curr_row, button, reset, move;

unsigned char checkHitsAndGetSize(unsigned char row, unsigned char lvl){
	//account for at a certain level the size max goes down
	unsigned char count = 0;
	unsigned char temp = row & grid[level - 1];
	
	if(lvl == 0){
		return 3;
	}
	
	while(temp != 0x00){
		if((temp & 0x01) == 0x01){
			count++;
		}
		temp = temp >> 1;
	}

	switch(count){
		case 0:
		case 1:
			return count;
		break;
		case 2:
			if(lvl >= 9)
				return 1;
			else
				return 2;
		break;
		case 3:
			if(lvl >= 4)
				return 2;
			else
				return 3;
		break;
		default:
		//DEBUG LED
		break;
	}


	
	return count;
	
}
void setGridRow(unsigned char row, unsigned char lvl){
	grid[lvl] = row;
}
void clearGrid(){
	for(unsigned char i = 0; i < MAX_LEVEL; i++){
		grid[i] = 0x00;
	}
}

//STATE ENUMS
enum GameLogicSM{GL_Start,Wait,Oscillate,Press,Win,Lose} GAMESTATE;
enum MovementSM{Mm_Wait,Align,Left,Right} MOVEMENTSTATE;

void Tick_GameLogic(){

	switch(GAMESTATE){ //State Transitions
		case GL_Start: 
			GAMESTATE = (button)? Wait : GL_Start; break;
		case Wait:
			if(button){ GAMESTATE = Wait;
			}else{
			GAMESTATE = Oscillate;
			curr_row = 0x38;
			}

		break;
		case Oscillate: 
			if(button){ 
			GAMESTATE = Press;
			move = 0;
			}else{ GAMESTATE = Oscillate;}

		break;
		case Press:
			curr_size = checkHitsAndGetSize(curr_row,level);

			if(curr_size > 0 && level == 15){
				GAMESTATE = Win;
			}else if(curr_size > 0 && !(level == 15)){ //Go to next level
				setGridRow(curr_row,level);
				level++;
				curr_speed = speeds[level];
				GAMESTATE = Oscillate;
			}else{ 
				GAMESTATE = Lose;
			}
		break;
		case Win:
			GAMESTATE = Win;
		break;
		case Lose:
			GAMESTATE = Lose;
		break;
		default: 
		GAMESTATE = GL_Start; break;
	}

	switch(GAMESTATE){ //State Actions
		case GL_Start:
			clearGrid();
			level = 0;
			curr_size = 3;
			curr_speed = speeds[0];
			move = 0;
			curr_row = 0x00;
		break;
		case Wait:
		break;
		case Oscillate:
			move = 1;
		break;
		case Press:
		break;
		case Win:
		//TODO
		break;
		case Lose:
		//TODO
		break;
		default:break;
	}
}

void Tick_Movement(){
	switch(MOVEMENTSTATE){ //State Transitions
		case Mm_Wait:
			MOVEMENTSTATE = move? Align: Mm_Wait;
		break;
		case Align:
		break;
		case Left:
		break;
		case Right:
		break;
		default:
		break;
	}
	switch(MOVEMENTSTATE){//State Actions
		case Mm_Wait:
		break;
		case Align:
		break;
		case Left:
		break;
		case Right:
		break;
		default:
		break;
	}
}

void checkReset(){
	if(reset){
		GAMESTATE = GL_Start;
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

		checkReset();
		Tick_GameLogic();
		Tick_Movement();

		while(!TimerFlag);
		TimerFlag = 0;

    }
}