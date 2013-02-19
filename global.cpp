
#include <Arduino.h>

float ease(int t, int d)
{
	return 1.0-pow(2, 10*(float(t)/float(d)-1));
}
