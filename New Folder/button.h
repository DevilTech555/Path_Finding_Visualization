#ifndef BUTTON_H_INCLUDE
#define BUTTON_H_INCLUDE

class button
{
public:
    int X,Y,W,H;
	int state;
	char * text;
	button(char *label,int x,int y,int w,int h)
	{
		X=x;
		Y=y;
		W=w;
		H=h;
		state = 1;
		text = label;
	}
	void draw();
    int insidebutton(int,int);
    void togglestate();
};

void Font(void *,char *,int,int);

#endif