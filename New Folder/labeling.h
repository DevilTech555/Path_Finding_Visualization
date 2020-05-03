#ifndef LEABELING_H_INCLUDE
#define LEABELING_H_INCLUDE

class label
{
public:
	int X, Y, W, H, C;
	int state;
	char *text;
	label(char *label, int x, int y, int w, int h, int c)
	{
		X = x;
		Y = y;
		W = w;
		H = h;
		C = c;
		state = 0;
		text = label;
	}
	void draw();
};
void banner();
void info();

#endif