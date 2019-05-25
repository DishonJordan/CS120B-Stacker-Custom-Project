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

int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char k = 1;
	ledmatrix7219d88_init();
	while(1){
	for(unsigned char i = 0; i < 4; i++){
		for(unsigned char j = 0; j < 128; j+=2){
			move_test(i,j);
			}
		}
	}
}
