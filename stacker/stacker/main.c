#include <avr/io.h>
#include <stdio.h>
//Global Variables
unsigned char grid[16];
unsigned char speeds[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char sizes[16] = {3,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1};
unsigned char MAX_LEVEL = 16;
unsigned char level, curr_size, curr_speed, curr_row;
//
unsigned char checkHits(unsigned char row, unsigned char lvl){
	if(lvl == 0)
		return 0x01;
	
	return !(row & grid[lvl - 1]);
}
unsigned char resize(unsigned char row, unsigned char lvl){
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
unsigned char setCurrentRow(unsigned char size,unsigned char lvl){
	unsigned char new_size = resize(curr_row,lvl);
	//TODO
	return new_size;
}

enum GameLogicSM{GL_Start,Wait,Oscillate,Press,Win,Lose,Reset}GAMESTATE;
enum DisplaySM{DS_Start,Display}DISPLAYSTATE;


int main(void)
{	
	
    while (1) 
    {
    }
}


