
#include "LPD8806.h"
#include "digit.h"

Digit::Digit(LPD8806& _lcd)
{
	lcd = _lcd;
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

void Digit::set(int number)
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

void Digit::set(float percentage)
{

}

void Digit::set_color(int r, int g, int b)
{
	color = lcd.Color(r, g, b);
}

void Digit::render()
{

	int i, state;

	for (i=0; i<length; i++)
	{
		state = bitRead(bit_mask, i);
		if (state == 1)
		{
			Serial.println(offset+i);
			lcd.setPixelColor(offset+i, color);
		}
		else
		{
			lcd.setPixelColor(offset+i, lcd.Color(0,0,0));
		}
	}
	lcd.show();

}

