
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

void Pulser::now(char _rgb[], int dur)
{
	queue(0, _rgb, dur, 88);
}
void Pulser::after(int start, char _rgb[])
{
	queue(start, _rgb, 1, 88);
}
void Pulser::display(int num, char _rgb[], int dur)
{
	queue(0, _rgb, dur, num);
}

void Pulser::queue(int start, char _rgb[], int dur, int num)
{

	int i = nextPulseIndex();
	if (i == -1) return;

	pulses[i].active = true;
	pulses[i].color = _rgb;
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
	float easing = ease(elapsed, pulse_len);

	pulser->left.digit(pulse_num / 10);
	pulser->right.digit(pulse_num % 10);

	pulser->left.color(color, easing);
	pulser->right.color(color, easing);
}
