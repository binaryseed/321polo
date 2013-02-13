
#include "LPD8806.h"
#include "SPI.h"
#include "Timer.h"
#include "Button.h"

#include "digit.h"
#include "game.h"

int _renderEvent, _tickEvent;

	// http://github.com/adafruit/LPD8806
LPD8806 lcd(56, 12, 13);

	// http://playground.arduino.cc/Code/Timer
Timer timer;

	// http://playground.arduino.cc/Code/Button
Button button(15, BUTTON_PULLDOWN, true, 50);

Digit left(lcd);
Digit right(lcd);

Game game(left, right);


void setup()
{
	Serial.begin(9660);

	right.setup(0,28,false);
	left.setup(28,28,true);
	lcd.begin();

	_renderEvent = timer.every(100, _render);
	_tickEvent = timer.every(1000, _tick);

	pinMode(15, INPUT);
	button.pressHandler(_buttonPress);
	button.holdHandler(_buttonHold, 1000);

	game.setup(10);
}

void loop()
{
	timer.update();
	button.process();
	game.process();
}

void _render()
{
	left.render();
	right.render();
}

void _tick()
{
	game.tick();
}

void _buttonPress(Button& b)
{
	game.toggle();
}

void _buttonHold(Button& b)
{
	game.reset();

	Serial.println("--[countdown");
	int wait = (10 + int(random(5)))*1000;
	wait = 1;
	Serial.println(wait);

	timer.after(wait, gamestart);
}


void gamestart()
{
	game.start();
}
