#include <Arduino.h>

#include "global.h"
#include "beeper.h"

Beeper::Beeper(int _pin)
{
	_alt = 0;
	pin = _pin;
}

void Beeper::process()
{
	for(int i=0; i<MAX_BEEPS; i++)
	{
		beeps[i].process(this);
	}
}


int Beeper::nextBeepIndex()
{
	for(int i=0; i<MAX_BEEPS; i++)
	{
		if(!beeps[i].active)
		{
			return i;
		}
	}
	return -1;
}

int Beeper::freq(int _f)
{
	return (EVEN(_alt++)) ? _f : _f+1;
}

void Beeper::now(float _d, int _f)
{
	// int now = millis();

	// queue(now, now+_d*1000, freq(_f));
	after(0, _d, _f);
}

void Beeper::after(int _w, float _d, int _f)
{
	int now = millis();
	int start = now + _w*1000;
	int end = start + int(_d*1000);

	queue(start, end, freq(_f) );
}

void Beeper::queue(int _s, int _e, int _f)
{
	int i = nextBeepIndex();
	if (i == -1) return;

	Serial.println("--[queue:");
	Serial.println(i);
	beeps[i].active = true;
	beeps[i].freq = _f;
	beeps[i].start_at = _s;
	beeps[i].end_at = _e;
}

void Beeper::shh()
{
	noTone(pin);
}

Beep::Beep()
{
	active = false;
}

void Beep::process(Beeper* beeper)
{
	if(!active) { return; }

	int now = millis();
	if(now < start_at) { return; }
	if(now >= end_at)
	{
		active = false;
		beeper->shh();
		return;
	}

	tone(beeper->pin, freq);

}
