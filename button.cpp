#include <GL/glut.h>
#include "button.h"

void button::draw()
{
    if (state != 0)
        glColor3f(0.0f / 255.0f, 71.0f / 255.0f, 141.0f / 255.0f);
    else
        glColor3f(0.0f / 255.0f, 33.0f / 255.0f, 85.0f / 255.0f);
    glBegin(GL_QUADS);
    glVertex2i(X, Y);
    glVertex2i(X, Y + H);
    glVertex2i(X + W, Y + H);
    glVertex2i(X + W, Y);
    glEnd();
    glColor3f(1,1,1);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2i(X, Y);
    glVertex2i(X, Y + H);
    glVertex2i(X + W, Y + H);
    glVertex2i(X + W, Y);
    glEnd();
    int fontx = X + (W- glutBitmapLength(GLUT_BITMAP_9_BY_15,(const unsigned char *)text)) / 2 ;
	int fonty = Y + (H+10)/2;
    glColor3f(1,1,1);
	Font(GLUT_BITMAP_9_BY_15,text,fontx,fonty);
}

void button::togglestate()
{

    state = (state == 1) ? 0 : 1;
}
int button::insidebutton(int x, int y)
{
    if (x > X && x < X + W && y > Y && y < Y + H)
        return 1;
    else
        return 0;
}

void Font(void *font,char *text,int x,int y)
{
	glRasterPos2i(x, y);
	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}
