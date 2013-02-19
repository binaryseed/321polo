
#include <Arduino.h>

float ease(int t, int d)
{
	// return 1.0-pow(2, 10*(float(t)/float(d)-1));
	return sin(float(t)/float(d) * (1.57079));
}

int from_hex(char c)
{
	switch (c)
	{
		case 'f': return 127;
		case 'e': return 119;
		case 'd': return 111;
		case 'c': return 103;
		case 'b': return 95;
		case 'a': return 87;
		case '9': return 71;
		case '8': return 63;
		case '7': return 55;
		case '6': return 47;
		case '5': return 39;
		case '4': return 31;
		case '3': return 24;
		case '2': return 15;
		case '1': return 7;
		case '0': return 0;
	}
}

uint32_t _color(char _rgb[], float easing)
{
	int r = int(from_hex(_rgb[0])*easing);
	int b = int(from_hex(_rgb[1])*easing);
	int g = int(from_hex(_rgb[2])*easing);

	return  ((uint32_t)(g | 0x80) << 16) |
			((uint32_t)(r | 0x80) <<  8) |
						b | 0x80 ;
}
