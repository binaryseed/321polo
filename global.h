
#define FPS 20
#define BUTTON_PIN 15
#define TONE_PIN 21

#define CEIL(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))
#define EVEN( _x ) ( _x & 1 ? false : true )

#define LEN(x) (sizeof(x) / sizeof(x[0]))

float ease(int, int);

uint32_t _color(char[], float=1.0);
int from_hex(char);
