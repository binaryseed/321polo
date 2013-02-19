
#include <LPD8806.h>
#include <SPI.h>
#include <Timer.h>
#include <Button.h>

#include "global.h"
#include "digit.h"
#include "game.h"
#include "pulse.h"
#include "beeper.h"


	// http://github.com/adafruit/LPD8806
LPD8806 lcd(56, 12, 13);

	// http://playground.arduino.cc/Code/Timer
Timer timer;

	// http://playground.arduino.cc/Code/Button
Button button(BUTTON_PIN, BUTTON_PULLDOWN, true, 50);


Digit left(lcd);
Digit right(lcd);

Game game(left, right);

Pulser pulser(left, right);

Beeper beeper(TONE_PIN);


void setup()
{
	Serial.begin(9660);

	right.setup(0,28,false);
	left.setup(28,28,true);
	lcd.begin();

	timer.every(1000/FPS, _render);
	timer.every(1000, _tick);

	pinMode(BUTTON_PIN, INPUT);
	button.pressHandler(_pressButton);
	button.releaseHandler(_releaseButton);
	button.holdHandler(_holdButton, 1500);

	game.setup(15);
}

void loop()
{
	timer.update();
	button.process();

	game.process();
	pulser.process();
	beeper.process();
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

void _pressButton(Button& b)
{
	beeper.now(.2, 3000);
}

void _releaseButton(Button& b)
{
	game.toggle();
}

void _holdButton(Button& b)
{
	beeper.now(.5, 3000);
	game.reset();

	int wait = (15 + random(5));

	pulser.now(90,90,90, wait-4);

	timer.after((wait-4)*1000, threeTwoOne);
}

void threeTwoOne()
{
	pulser.after(0, 127,0,0);
	beeper.after(0, .5, 2000);

	pulser.after(1, 127,20,0);
	beeper.after(1, .5, 2000);

	pulser.after(2, 127,70,0);
	beeper.after(2, .5, 2000);

	pulser.after(3, 0,127,0);
	beeper.after(3, 1.0, 2500);

	timer.after(4000, playPolo);
}

void playPolo()
{
	pulser.display(game.length, 0,127,0, 4);

	game.start();
	game.onFinish(gameOver);
}

void gameOver()
{
	beeper.now(2.0, 2000);

	// pulser.display(score, 50,50,50, 5);
}

