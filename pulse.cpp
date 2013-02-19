
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
	queue(0, r,g,b, dur, 88);
}
void Pulser::after(int start, int r,int g,int b)
{
	queue(start, r,g,b, 1, 88);
}
void Pulser::display(int num, int r,int g,int b, int dur)
{
	queue(0, r,g,b, dur, num);
}

void Pulser::queue(int start, int r,int g,int b, int dur, int num)
{

	int i = nextPulseIndex();
	if (i == -1) return;

	pulses[i].active = true;
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

	int r = int(colorR*portion);
	int g = int(colorG*portion);
	int b = int(colorB*portion);

	pulser->left.digit(pulse_num / 10);
	pulser->right.digit(pulse_num % 10);

	pulser->left.color(r,g,b);
	pulser->right.color(r,g,b);

}
