
class Game
{

public:
	Game(Digit&, Digit&);
	void setup(int);

	void reset();
	void start();
	void tick();
	void toggle();
	void process();

	int length;
	int duration;
	void onFinish(void (*callback)());

private:
	void (*callback)();

	void bump();
	void showTime(int, int);

	int mins();
	int secs();

	Digit& left;
	Digit& right;

	bool started, playing, final;
	int remaining_seconds, finished_at;
	int last_state, current_state;

	int left_goals, right_goals;
};
