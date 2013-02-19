
class Digit
{

public:
	Digit(LPD8806&);
	void setup(int, int, bool);

	void digit(int);
	void percentage(float);
	void color(char*, float=1.0);
	void render();

private:
	LPD8806& lcd;
	int pattern[10];

	int offset;
	int length;
	bool mirrored;

	unsigned long bit_mask;
	uint32_t rgb_color;
};
