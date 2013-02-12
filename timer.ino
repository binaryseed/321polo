
#include "LPD8806.h"
#include <SPI.h>

#include "digit.h"

LPD8806 lcd = LPD8806(56, 12, 13);

Digit left = Digit(lcd);
Digit right = Digit(lcd);

int num = 0;

void setup()
{

	Serial.begin(9660);

	right.setup(0,28,false);
	left.setup(28,28,true);

	lcd.begin();
}

void loop()
{

	left.set(num);
	right.set(num);

	left.set_color( random(30),random(30),random(30) );
	right.set_color( random(30),random(30),random(30) );

	left.render();
	right.render();

	if (num == 9)
	{
		num = 0;
	}
	else
	{
		num++;
	}

	delay(2000);

}

