#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ledmatrix7219d88.c"
#include "ledmatrix7219d88.h"
#include "max7219.h"
#include "max7219.c"

void move_test(unsigned char num, unsigned char column){
		ledmatrix7219d88_setintensity(num,2);
		ledmatrix7219d88_setrow(num,0,column);
		ledmatrix7219d88_setrow(num,1,column);
		ledmatrix7219d88_setrow(num,2,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,1,column);
		ledmatrix7219d88_setrow(num,2,column);
		ledmatrix7219d88_setrow(num,3,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,2,column);
		ledmatrix7219d88_setrow(num,3,column);
		ledmatrix7219d88_setrow(num,4,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,3,column);
		ledmatrix7219d88_setrow(num,4,column);
		ledmatrix7219d88_setrow(num,5,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,4,column);
		ledmatrix7219d88_setrow(num,5,column);
		ledmatrix7219d88_setrow(num,6,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,5,column);
		ledmatrix7219d88_setrow(num,6,column);
		ledmatrix7219d88_setrow(num,7,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,4,column);
		ledmatrix7219d88_setrow(num,5,column);
		ledmatrix7219d88_setrow(num,6,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,3,column);
		ledmatrix7219d88_setrow(num,4,column);
		ledmatrix7219d88_setrow(num,5,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,2,column);
		ledmatrix7219d88_setrow(num,3,column);
		ledmatrix7219d88_setrow(num,4,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
		ledmatrix7219d88_setrow(num,1,column);
		ledmatrix7219d88_setrow(num,2,column);
		ledmatrix7219d88_setrow(num,3,column);
		_delay_ms(600);
		ledmatrix7219d88_resetmatrix(num);
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

int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char k = 1;
	ledmatrix7219d88_init();
	ledmatrix7219d88_setintensity(0,2);
	ledmatrix7219d88_setintensity(1,2);
	ledmatrix7219d88_setintensity(2,2);
	ledmatrix7219d88_setintensity(3,2);

	while(1){
		for(int i = 0; i < 32; i++){
			if(i % 2 == 0)
			displayColumn(i,0xF0);
			else
			displayColumn(i,0x0F);

			_delay_ms(1500);
		}

		for(int i = 0; i < 32; i++){
			if(i % 2 != 0)
			displayColumn(i,0xF0);
			else
			displayColumn(i,0x0F);

			_delay_ms(1500);
		}
	}

}


