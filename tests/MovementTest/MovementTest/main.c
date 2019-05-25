#include <avr/io.h>
#include "ledmatrix7219d88.c"
#include "max7219.c"

unsigned char curr_row,level,cnt, speed;

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

enum MovementSM{Right,Left}MOVEMENTSTATE;

void MovementSM_Tick(){
	unsigned char button = ~PINA & 0x01;

	switch(MOVEMENTSTATE){ //Transitions
		case Right:
			if(curr_row & 0x01){
				MOVEMENTSTATE = Left;
				cnt = 0;
			}else{
				MOVEMENTSTATE = Right;
			}
		break;
		case Left:
			if(curr_row & 0x80){
				MOVEMENTSTATE = Right;
				cnt = 0;
				}else{
				MOVEMENTSTATE = Left;
			}
		break;
		default:
			MOVEMENTSTATE = Right;
		break;
	}
	switch(MOVEMENTSTATE){ //Actions
		case Right:
			level = (button)? level + 1: level;

			if(cnt % speed == 0){
				curr_row = curr_row >> 1;
				displayColumn(level,curr_row);
			}
			cnt++;
		break;
		case Left:
			level = (button)? level + 1: level;

			if(cnt % speed == 0){
				curr_row = curr_row << 1;
				displayColumn(level,curr_row);
			}
			cnt++;
		break;
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;

	ledmatrix7219d88_init();
	for(unsigned char i = 0; i < 4; i++){
		ledmatrix7219d88_setintensity(i,1);
	}

	TimerSet(10);
	TimerOn();

    unsigned char start = 0x38;
	curr_row = start; level = 0; 
	cnt = 0; speed = 20;
	MOVEMENTSTATE = Right;

	while(1){
		MovementSM_Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}

}

