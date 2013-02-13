
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

private:
	void bump();

	int mins();
	int secs();

	Digit& left;
	Digit& right;

	bool started, playing, final;
	int duration;
	int remaining_seconds;
	int last_state, current_state;

	int left_goals, right_goals;
};
