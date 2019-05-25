#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "io.c"
#include "ledmatrix7219d88.c"
#include "max7219.c"

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

#define MAX_LEVEL 32

unsigned char grid[MAX_LEVEL];
unsigned char speeds[MAX_LEVEL] = {10,10,10,10,10,11,11,11,11,9,9,9,9,8,8,8,8,7,7,7,7,6,6,6,6,6,6,5,5,5,5,4}; //In terms of a 10ms Period
unsigned char sizes[MAX_LEVEL] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1};
unsigned char row_start[3] = {0x08,0x18,0x38};
unsigned char level, curr_size, curr_speed, curr_row, button, reset, move,cnt;

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
			if(lvl >= 25)
				return 1;
			else
				return 2;
		break;
		case 3:
			if(lvl >= 14)
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
	if(lvl > 0)
		grid[lvl] = row & grid[lvl - 1];
	else
		grid[lvl] = row;
}
void clearGrid(){
	for(unsigned char i = 0; i < MAX_LEVEL; i++){
		grid[i] = 0x00;
	}
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
void displayColumn(unsigned char col, unsigned char value){

	unsigned char start_loc = 8*col % 64;
	unsigned char matrix_num = 8*col / 64;


	for(int i = 0; i < 8; i++){
		ledmatrix7219d88_setled(matrix_num,start_loc + i, GetBit(value,i));
	}

}

//STATE ENUMS
enum GameLogicSM{GL_Start,Wait,Oscillate,Press,Win,Lose} GAMESTATE;
enum MovementSM{Mm_Start,Right,Left}MOVEMENTSTATE;

void Tick_GameLogic(){

	switch(GAMESTATE){ //State Transitions
		case GL_Start: 
			GAMESTATE = (button)? Wait : GL_Start; break;
		case Wait:
			if(button){ GAMESTATE = Wait;
			}else{
			GAMESTATE = Oscillate;
			curr_row = row_start[curr_size - 1];
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

			if(curr_size > 0 && level == (MAX_LEVEL - 1)){
				GAMESTATE = Win;
			}else if(curr_size > 0 && !(level == (MAX_LEVEL - 1))){ //Go to next level
				setGridRow(curr_row,level);
				displayColumn(level,grid[level]);
				level++;
				curr_speed = speeds[level];
				curr_row = row_start[curr_size - 1];
				GAMESTATE = Wait;
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
			for(unsigned char i = 0; i < 4; i++){
				ledmatrix7219d88_resetmatrix(i);
			}
			level = 0;
			curr_size = sizes[level];
			curr_speed = speeds[0];
			move = 0;
			curr_row = 0x00;
			cnt = 0;
		break;
		case Wait:
		break;
		case Oscillate:
			move = 1;
		break;
		case Press:
			move = 0;
		break;
		case Win:
			move = 0;
		break;
		case Lose:
			move = 0;
		break;
		default:break;
	}
}

void MovementSM_Tick(){

	switch(MOVEMENTSTATE){ //Transitions
		case Mm_Start:
			MOVEMENTSTATE = move? Right: Mm_Start;
		break;
		case Right:
		if(!move){
			MOVEMENTSTATE = Mm_Start;
		}else if(curr_row & 0x01){
			MOVEMENTSTATE = Left;
			cnt = 0;
		}else{
			MOVEMENTSTATE = Right;
		}
		break;
		case Left:
		if(!move){
			MOVEMENTSTATE = Mm_Start;
		}else if(curr_row & 0x80){
			MOVEMENTSTATE = Right;
			cnt = 0;
		}else{
			MOVEMENTSTATE = Left;
		}
		break;
		default:
		MOVEMENTSTATE = Mm_Start;
		break;
	}
	switch(MOVEMENTSTATE){ //Actions
		case Mm_Start:
		break;
		case Right:
		if(move){
			if(cnt % speeds[level] == 0){
				curr_row = curr_row >> 1;
				displayColumn(level,curr_row);
			}
			cnt++;
		}
		break;
		case Left:
		if(move){
			if(cnt % speeds[level] == 0){
				curr_row = curr_row << 1;
				displayColumn(level,curr_row);
			}
			cnt++;
		}
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
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	ledmatrix7219d88_init();
	for(unsigned char i = 0; i < 4; i++){
		ledmatrix7219d88_setintensity(i,1);
	}

	TimerSet(10);
	TimerOn();

	GAMESTATE = GL_Start;
	MOVEMENTSTATE = Mm_Start;

    while (1) 
    {
		button = ~PINA & 0x01;
		reset  = ~PINA & 0x02;

		checkReset();
		Tick_GameLogic();
		MovementSM_Tick();

		PORTC = GAMESTATE;
		PORTB = curr_size;
		while(!TimerFlag);
		TimerFlag = 0;

    }
}