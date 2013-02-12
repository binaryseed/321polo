
#include "LPD8806.h"
#include <SPI.h>
#include "Timer.h"

#include "digit.h"

LPD8806 lcd = LPD8806(56, 12, 13);

Timer timer;

Digit left = Digit(lcd);
Digit right = Digit(lcd);

int num = 0;

int _renderEvent;

void setup()
{

	Serial.begin(9660);

	right.setup(0,28,false);
	left.setup(28,28,true);

	lcd.begin();

	_renderEvent = timer.every(50, _render);
}

void loop()
{

	timer.update();

	left.digit(num);
	right.digit(num);

	left.color( random(30),random(30),random(30) );
	right.color( random(30),random(30),random(30) );

	if (num == 9)
	{
		num = 0;
	}
	else
	{
		num++;
	}

	delay(1000);

}

void _render()
{
	left.render();
	right.render();
}
