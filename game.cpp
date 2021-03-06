
#include <Arduino.h>
#include <LPD8806.h>
#include <Timer.h>

#include "global.h"
#include "digit.h"
#include "game.h"

Game::Game(Digit& l, Digit& r) : left(l), right(r) {}

void Game::setup(int _duration)
{
	length = _duration;
	duration = _duration * 60;
	reset();
}

void Game::reset()
{
	last_state = 0;
	current_state = 0;
	started = false;
	playing = false;
	final = false;
	finished_at = 0;
	remaining_seconds = duration--;
}

void Game::start()
{
	started = true;
	playing = true;
	bump();
}

void Game::onFinish(void (*_callback)()) {callback = _callback; }

int Game::mins() { return remaining_seconds / 60; }
int Game::secs() { return remaining_seconds % 60; }

void Game::bump() { current_state++; }

void Game::tick()
{
	if (!started || final || !playing)
	{
		return;
	}

	if (remaining_seconds <= 0)
	{
		playing = false;
		final = true;
		finished_at = millis();
		(*callback)();
		return;
	}

	remaining_seconds--;
	bump();
}

void Game::toggle()
{
	if (started && !final)
	{
		Serial.println("--[toggle");
		playing = !playing;
		bump();
		return;
	}

	if (final)
	{
		finished_at = millis();
	}
}

void Game::showTime(int m, int s)
{
	// Digits
	if(m >= 10)
	{
		left.digit(m / 10);
		right.digit(m % 10);
	}
	else if (m < 10 && m >= 1)
	{
		left.percentage(s / 60.0);
		right.digit(m);
	}
	else if (m < 1)
	{
		left.digit(s / 10);
		right.digit(s % 10);
	}

	if (m >= 1 && m < 10)
	{
		left.color("00f");
	}
	else
	{
		left.color("f00");
	}

	right.color("f00");

}

void Game::process()
{

	if (playing)
	{
		if (current_state > last_state)
		{
			showTime(mins(), secs());
		}

		last_state = current_state;
		return;
	}

	if (started && !playing) // paused
	{
		int now = millis();
		float easing = ease(now % 1000, 1000);

		showTime(11,0);

		// pulse out after pulsing in
		if ( EVEN(now / 1000) ) { easing = 1.0-easing; }

		left.color("800", easing);
		right.color("800", easing);
	}

	if (final)
	{
		int elapsed = millis() - finished_at;
		float easing = ease(elapsed % 1000, 1000);

		elapsed = elapsed / 1000;
		showTime(elapsed / 60, elapsed % 60);

		left.color("f00", easing);
		if (elapsed >= 60) { left.color("999"); }
		right.color("f00", easing);
	}

}
