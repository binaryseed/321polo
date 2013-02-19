
#include <Arduino.h>
#include <LPD8806.h>

#include "global.h"
#include "digit.h"
#include "pulse.h"

Pulser::Pulser(Digit& l, Digit& r) : left(l), right(r)
{
}

int Pulser::nextPulseIndex()
{
	for(int i=0; i<MAX_PULSES; i++)
	{
		if(!pulses[i].active)
		{
			return i;
		}
	}
	return -1;
}

void Pulser::now(int r,int g,int b, int dur)
{
	queue(0, r,g,b, dur, 88, -1);
}
void Pulser::countdown(int start, int r,int g,int b, int tone)
{
	queue(start, r,g,b, 1, 88, tone);
}
void Pulser::display(int num, int start, int r,int g,int b, int dur)
{
	queue(start, r,g,b, dur, num, -1);
}

void Pulser::queue(int start, int r,int g,int b, int dur, int num, int tone)
{

	int i = nextPulseIndex();
	if (i == -1) return;

	pulses[i].active = true;
	pulses[i].tone = tone;
	pulses[i].colorR = r;
	pulses[i].colorG = g;
	pulses[i].colorB = b;
	pulses[i].start_at = millis()+start*1000;
	pulses[i].end_at = millis()+(start+dur)*1000;
	pulses[i].pulse_num = num;
}

void Pulser::process()
{
	for(int i=0; i<MAX_PULSES; i++)
	{
		pulses[i].process(this);
	}
}

Pulse::Pulse()
{
	active = false;
	pulse_len = 1000;
}

void Pulse::process(Pulser* pulser)
{
	if(!active) { return; }

	int now = millis();
	if(now < start_at) { return; }
	if(now >= end_at) { active = false; return; }

	int elapsed = (now - start_at) % pulse_len;
	float portion = ease(elapsed, pulse_len);

	if (tone >= 0)
	{
		if (portion > .1) { pulser->playTone(tone); }
		if (portion < .1) { pulser->stopTone(); }
	}

	int r = int(colorR*portion);
	int g = int(colorG*portion);
	int b = int(colorB*portion);

	pulser->left.digit(pulse_num / 10);
	pulser->right.digit(pulse_num % 10);

	pulser->left.color(r,g,b);
	pulser->right.color(r,g,b);

}

void Pulser::playTone(int _tone)
{
	switch(_tone)
	{
		case 3:
		case 2:
		case 1:
			tone(TONE_PIN, 2000+_tone);
			break;
		case 0:
			tone(TONE_PIN, 2500);
			break;
	}
}

void Pulser::stopTone()
{
	noTone(TONE_PIN);
}
