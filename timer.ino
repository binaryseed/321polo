
#include "LPD8806.h"
#include "SPI.h"
#include "Timer.h"
#include "Button.h"

#include "digit.h"
#include "game.h"

#define EVEN( _x ) ( _x & 1 ? false : true )

int fps = 30;
int buttonPin = 15;
int tonePin = 21;

	// http://github.com/adafruit/LPD8806
LPD8806 lcd(56, 12, 13);

	// http://playground.arduino.cc/Code/Timer
Timer timer;

	// http://playground.arduino.cc/Code/Button
Button button(buttonPin, BUTTON_PULLDOWN, true, 50);

Digit left(lcd);
Digit right(lcd);

Game game(left, right);


void setup()
{
	Serial.begin(9660);

	right.setup(0,28,false);
	left.setup(28,28,true);
	lcd.begin();

	timer.every(1000/fps, _render);
	timer.every(1000, _tick);

	pinMode(buttonPin, INPUT);
	button.pressHandler(_pressButton);
	button.releaseHandler(_releaseButton);
	button.holdHandler(_holdButton, 1000);

	game.setup(1);
}

void loop()
{
	timer.update();
	game.process();
	button.process();
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

	int wait = 1000 * (15 + random(10));
	wait = 9000;

	timer.after(wait-4000, pulseRed);
	timer.after(wait-3000, pulseOrange);
	timer.after(wait-2000, pulseYellow);
	timer.after(wait-1000, pulseGreen);
	timer.after(wait, playPolo);

	pulseWhite((wait/1000)-4);
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
}



// Sound
//

void beep() { beep(200); }

void beep(int duration)
{
	tone(tonePin, 3000+random(50));
	timer.after(duration, silence);
}

void silence() { noTone(tonePin); }



// Pulse
//

int pulseR, pulseG, pulseB, pulseStart;

void pulseRed()
{
	noTone(tonePin);
	tone(tonePin, 2000);
	pulseSolid(127, 0, 0);
}
void pulseOrange()
{
	noTone(tonePin);
	tone(tonePin, 2001);
	pulseSolid(127, 10, 0);
}
void pulseYellow()
{
	noTone(tonePin);
	tone(tonePin, 2002);
	pulseSolid(127, 70, 0);
}
void pulseGreen()
{
	noTone(tonePin);
	tone(tonePin, 2500);
	pulseSolid(0, 127, 0);
}
void pulseWhite(int duration)
{
	pulseSolid(100,100,100,duration);
}

void pulseSolid(int r, int g, int b) { pulseSolid(r, g, b, 1); }

void pulseSolid(int r, int g, int b, int duration)
{
	pulseR = r;
	pulseG = g;
	pulseB = b;

	timer.every(1000/fps, pulseFrame, fps*duration);
	pulseStart = millis();
}

void pulseFrame()
{
	int elapsed = millis() - pulseStart;
	float portion = (elapsed % 1000) / 1000.0;

	if (portion > .6) { noTone(tonePin); }

	int r = int(pulseR * portion);
	int g = int(pulseG * portion);
	int b = int(pulseB * portion);

	left.digit(8);
	right.digit(8);

	left.color(r,g,b);
	right.color(r,g,b);
}
