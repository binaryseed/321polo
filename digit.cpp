
#include <Arduino.h>
#include "LPD8806.h"
#include "digit.h"

#define CEIL(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))

Digit::Digit(LPD8806& _lcd) : lcd(_lcd)
{
	pattern[0] = B11111100;
	pattern[1] = B00011000;
	pattern[2] = B01101110;
	pattern[3] = B00111110;
	pattern[4] = B10011010;
	pattern[5] = B10110110;
	pattern[6] = B11110110;
	pattern[7] = B00011100;
	pattern[8] = B11111110;
	pattern[9] = B10011110;
}

void Digit::setup(int o, int l, bool m)
{
	offset = o;
	length = l;
	mirrored = m;

	bit_mask = 0;
}

void Digit::digit(int number)
{
	int i, state, loc;

	for (i=0; i<=7; i++)
	{
		loc = 4*i;
		state = bitRead(pattern[number], 7-i);

		if (mirrored == true)
		// left LCD order is mirrored
		{
			switch(i)
			{
				case 0:
					loc = 4*4;
					break;
				case 1:
					loc = 4*3;
					break;
				case 3:
					loc = 4*1;
					break;
				case 4:
					loc = 4*0;
					break;
			}
		}
		bitWrite(bit_mask, loc+0, state);
		bitWrite(bit_mask, loc+1, state);
		bitWrite(bit_mask, loc+2, state);
		bitWrite(bit_mask, loc+3, state);
	}
}

void Digit::color(int r, int g, int b)
{
	rgb_color = lcd.Color(r, b, g);
}

void Digit::percentage(float percentage)
{

	int i, state;
	int dist = CEIL(24*percentage);
	Serial.println(24*percentage);
	Serial.println(dist);

	for (i=0; i<length; i++)
	{
		state = ((i<dist) ? 1 : 0);
		bitWrite(bit_mask, 23-i, state);
	}
}

void Digit::render()
{

	int i, state;

	for (i=0; i<length; i++)
	{
		state = bitRead(bit_mask, i);
		if (state == 1)
		{
			lcd.setPixelColor(offset+i, rgb_color);
		}
		else
		{
			lcd.setPixelColor(offset+i, lcd.Color(0,0,0));
		}
	}
	lcd.show();

}

