
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

int t = 2;
int times[] = {10, 12, 15, 20, 30, 45, 50, 60};
bool time_set = false;

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
	pulser.display(game.length, "0f0", 1);
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
	if(!time_set)
	{
		game.setup( times[++t % LEN(times)] );
		pulser.display(game.length, "0f0", 1);
	}
	else
	{
		game.toggle();
	}

}

void _holdButton(Button& b)
{
	beeper.now(.5, 3000);

	game.reset();
	time_set = true;

	int wait = (5 + random(5));

	pulser.now("666", wait);

	timer.after(wait*1000, threeTwoOne);
}

void threeTwoOne()
{
	pulser.after(0, "f00");
	beeper.after(0.0, .5, 2000);

	pulser.after(1, "f30");
	beeper.after(1.0, .5, 2000);

	pulser.after(2, "f90");
	beeper.after(2.0, .5, 2000);

	pulser.after(3, "0f0");
	beeper.after(3.0, 1.0, 2500);

	timer.after(4000, playPolo);
}

void playPolo()
{
	pulser.display(game.length, "0f0", 4);

	game.start();
	game.onFinish(gameOver);
}

void gameOver()
{
	beeper.now(3.0, 2000);

	// pulser.display(score, "666", 5);
}

