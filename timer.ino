
#include <LPD8806.h>
#include <SPI.h>
#include <Timer.h>
#include <Button.h>

#include "global.h"
#include "digit.h"
#include "game.h"
#include "pulse.h"


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
	button.holdHandler(_holdButton, 2000);

	game.setup(15);
}

void loop()
{
	timer.update();
	button.process();

	game.process();
	pulser.process();
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
	beep();
}

void _releaseButton(Button& b)
{
	game.toggle();
}

void _holdButton(Button& b)
{
	beep(500);
	game.reset();

	int wait = (10 + random(10));

	pulser.now(50,50,50, wait-4);

	pulser.countdown(wait-4, 127,0,0,  3);
	pulser.countdown(wait-3, 127,20,0, 2);
	pulser.countdown(wait-2, 127,70,0, 1);
	pulser.countdown(wait-1, 0,127,0,  0);

	pulser.display(game.length, wait, 50,50,50, 4);

	timer.after(wait*1000, playPolo);
}

void playPolo()
{
	silence();
	game.start();
	game.onFinish(gameOver);
}

void gameOver()
{
	beep(3000);

	// pulser.display(score, 0, 50,50,50, 5);
}



// Sound
//

	void beep() { beep(200); }

	void beep(int duration)
	{
		tone(TONE_PIN, 3000+random(50));
		timer.after(duration, silence);
	}

	void silence() { noTone(TONE_PIN); }
