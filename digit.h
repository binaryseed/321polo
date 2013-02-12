
#include <Arduino.h>

class Digit
{

public:

	Digit(LPD8806&);
	void setup(int, int, bool);

	LPD8806 lcd;
	int pattern[10];

	int offset;
	int length;
	bool mirrored;

	unsigned long bit_mask;
	uint32_t rgb_color;

	void digit(int);
	void digit(float);

	void color(int, int, int);

	void render();
};
