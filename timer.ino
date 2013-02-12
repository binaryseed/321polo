
#include "LPD8806.h"
#include "SPI.h"
#include "Timer.h"
#include "Button.h"

#include "digit.h"

LPD8806 lcd = LPD8806(56, 12, 13);

Digit left = Digit(lcd);
Digit right = Digit(lcd);

Timer timer;

Button button = Button(15, BUTTON_PULLUP_INTERNAL, true, 50);

int _renderEvent;

void setup()
{

	Serial.begin(9660);

	right.setup(0,28,false);
	left.setup(28,28,true);

	lcd.begin();

	_renderEvent = timer.every(100, _render);

	pinMode(15, INPUT);
	button.pressHandler(_buttonPress);
}

void loop()
{
	timer.update();
	button.process();
}

void _buttonPress(Button& b)
{
	Serial.println("button Pressed");
	left.color( random(30),random(30),random(30) );
	right.color( random(30),random(30),random(30) );
	left.digit(int(random(10)));
	right.digit(int(random(10)));
}

void _render()
{
	left.render();
	right.render();
}
