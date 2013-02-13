
#include <Arduino.h>
#include "LPD8806.h"
#include "Timer.h"
#include "digit.h"
#include "game.h"

Game::Game(Digit& l, Digit& r) : left(l), right(r) {}

void Game::setup(int _duration)
{
	duration = _duration * 60;
}

void Game::reset()
{
	Serial.println("--[reset");
	last_state = 0;
	current_state = 0;
	started = false;
	playing = false;
	final = false;
	remaining_seconds = duration--;
}

void Game::start()
{
	Serial.println("--[start");
	started = true;
	playing = true;
	bump();
}

int Game::mins() { return remaining_seconds / 60; }
int Game::secs() { return remaining_seconds % 60; }

void Game::bump() { current_state++; }

void Game::tick()
{
	if (started == false || final == true || playing == false)
	{
		return;
	}

	if (remaining_seconds <= 0)
	{
		playing = false;
		final = true;
		// display final score
		return;
	}

	remaining_seconds--;
	bump();
}

void Game::toggle()
{
	if ( started == true && final == false)
	{
		Serial.println("--[toggle");
		playing = !playing;
		bump();
	}
}

void Game::process()
{
	if (current_state > last_state)
	{
		// prepare displays from current state

		if (playing == true)
		{
			Serial.println("--[remaining");
			int m = mins();
			int s = secs();
			Serial.println(s/60.0);


			if(m > 10)
			{
				left.color(10,0,0);
				right.color(10,0,0);
				left.digit(m / 10);
				right.digit(m % 10);
			}
			else if ( m < 10 && m > 1)
			{
				left.color(10,0,0);
				right.color(5,5,5);
				left.digit(m);
				right.percentage(s/60.0);
			}
		}

		last_state = current_state;
	}
}
