
#define MAX_BEEPS 10

class Beeper;

class Beep
{
public:
	Beep();

	void process(Beeper*);

	bool active;
	int start_at;
	int end_at;

	int freq;
};

class Beeper
{
public:
	Beeper(int);

	int nextBeepIndex();
	void process();

	int pin;
	int _alt;

	int freq(int);

	void now(float, int);
	void after(float, float, int);
	void shh();

private:
	void queue(int, int, int);
	Beep beeps[MAX_BEEPS];
};
