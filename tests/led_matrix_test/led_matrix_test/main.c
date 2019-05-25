//ledmatrix7219d88 output example

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ledmatrix7219d88.c"
#include "ledmatrix7219d88.h"
#include "max7219.h"
#include "max7219.c"

int main(void) {
	//init ledmatrix
	ledmatrix7219d88_init();
	DDRD = 0xFF; PORTD = 0x00;

	uint8_t ledmatrix = 0;

	//display test rows
	uint8_t rows[8] = {
			0b10000001,
			0b01000010,
			0b00100100,
			0b00011000,
			0b00011000,
			0b00100100,
			0b01000010,
			0b10000001
	};
	

	ledmatrix = 0;
	ledmatrix7219d88_setrow(ledmatrix, 0, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 1, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 2, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 3, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 4, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 5, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 6, 0x01);
	ledmatrix7219d88_setrow(ledmatrix, 7, 0x01);
	_delay_ms(2000);
	ledmatrix7219d88_setrow(ledmatrix, 0, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 1, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 2, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 3, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 4, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 5, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 6, 0x02);
	ledmatrix7219d88_setrow(ledmatrix, 7, 0x02);
	
	//display test rows
	//ledmatrix = 0;
	//ledmatrix7219d88_setrow(ledmatrix, 0, 0b10000000);
	//ledmatrix7219d88_setrow(ledmatrix, 1, 0b01000000);
	//ledmatrix7219d88_setrow(ledmatrix, 2, 0b00100000);
	//ledmatrix7219d88_setrow(ledmatrix, 3, 0b00010000);
	//ledmatrix7219d88_setrow(ledmatrix, 4, 0b00001000);
	//ledmatrix7219d88_setrow(ledmatrix, 5, 0b00000100);
	//ledmatrix7219d88_setrow(ledmatrix, 6, 0b00000010);
	//ledmatrix7219d88_setrow(ledmatrix, 7, 0b00000001);
	//ledmatrix = 1;
	//ledmatrix7219d88_setrow(ledmatrix, 0, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 1, 0b01010101);
	//ledmatrix7219d88_setrow(ledmatrix, 2, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 3, 0b01010101);
	//ledmatrix7219d88_setrow(ledmatrix, 4, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 5, 0b01010101);
	//ledmatrix7219d88_setrow(ledmatrix, 6, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 7, 0b01010101);
	//ledmatrix = 2;
	//ledmatrix7219d88_setrow(ledmatrix, 0, 0b10000000);
	//ledmatrix7219d88_setrow(ledmatrix, 1, 0b01000000);
	//ledmatrix7219d88_setrow(ledmatrix, 2, 0b00100000);
	//ledmatrix7219d88_setrow(ledmatrix, 3, 0b00010000);
	//ledmatrix7219d88_setrow(ledmatrix, 4, 0b00001000);
	//ledmatrix7219d88_setrow(ledmatrix, 5, 0b00000100);
	//ledmatrix7219d88_setrow(ledmatrix, 6, 0b00000010);
	//ledmatrix7219d88_setrow(ledmatrix, 7, 0b00000001);
	//ledmatrix = 3;
	//ledmatrix7219d88_setrow(ledmatrix, 0, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 1, 0b01010101);
	//ledmatrix7219d88_setrow(ledmatrix, 2, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 3, 0b01010101);
	//ledmatrix7219d88_setrow(ledmatrix, 4, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 5, 0b01010101);
	//ledmatrix7219d88_setrow(ledmatrix, 6, 0b10101010);
	//ledmatrix7219d88_setrow(ledmatrix, 7, 0b01010101);
	//_delay_ms(2000);
	//ledmatrix7219d88_resetmatrix(0);
	//ledmatrix7219d88_resetmatrix(1);
	//ledmatrix7219d88_resetmatrix(2);
	//ledmatrix7219d88_resetmatrix(3);

}
