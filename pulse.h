
#define MAX_PULSES 10

class Pulser;

class Pulse
{
public:
	Pulse();

	void process(Pulser*);

	bool active;

	int colorR;
	int colorG;
	int colorB;

	int start_at;
	int end_at;

	int pulse_num;
	int pulse_len;
};

class Pulser
{
public:
	Pulser(Digit&, Digit&);

	void now       (int,int,int,int);
	void after     (int,int,int,int);
	void display   (int,int,int,int,int);

	Digit& left;
	Digit& right;

	int nextPulseIndex();
	void process();

private:
	Pulse pulses[MAX_PULSES];

	void queue(int,int,int,int,int,int);
};
