
#define MAX_PULSES 10

class Pulser;

class Pulse
{
public:
	Pulse();

	void process(Pulser*);

	bool active;

	char* color;

	int start_at;
	int end_at;

	int pulse_num;
	int pulse_len;
};

class Pulser
{
public:
	Pulser(Digit&, Digit&);

	void now       (char*,int);
	void after     (int,char*);
	void display   (int,char*,int);

	Digit& left;
	Digit& right;

	int nextPulseIndex();
	void process();

private:
	Pulse pulses[MAX_PULSES];

	void queue(int,char*,int,int);
};
