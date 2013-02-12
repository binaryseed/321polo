
#include <Arduino.h>

class Digit
{

public:

	LPD8806 lcd;
	Digit(LPD8806&);


	int offset;
	int length;
	bool mirrored;

	int pattern[10];
	unsigned long bit_mask;
	uint32_t color;

	void setup(int, int, bool);

	void set(int);
	void set(float);

	void set_color(int, int, int);

	void render();
};
