#include <iostream>
#include <GL/glut.h>
#include "button.h"
#include "aStar.h"

int X = 600;
int Y = 450;
int fontx, fonty;
int start = 0;
button b1((char *)"Start", 475, 400, 100, 30);
button b2((char *)"Reset", 475, 350, 100, 30);
button b3((char *)"Random Wall", 475, 300, 100, 30);
button b4((char *)"Diagonal", 475, 250, 100, 30);
int cs = 0;
int ce = 0;
int cw = 0;

void init()
{
    glClearColor(0.1, 0.0, 0.0, 0.0);
    first_fun();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE_ARB);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, X, Y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(450, 0);
    glVertex2i(450, 450);
    glVertex2f(0, 50);
    glVertex2i(450, 50);
    glEnd();
    glColor3f(1, 1, 1);
    fontx = 5 + (450 - glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"A* Path Finding Algorithm")) / 2;
    fonty = 5 + (50 + 10) / 2;
    Font(GLUT_BITMAP_TIMES_ROMAN_24, (char *)"A* Path Finding Algorithm", fontx, fonty);
    b1.draw();
    b2.draw();
    b3.draw();
    b4.draw();
    Grid();
    if (start == 1)
    {
        a_st();
    }
    glutSwapBuffers();
}

void reshape(int x, int y)
{
    X = x;
    Y = y;
    glViewport(0, 0, (GLsizei)x, (GLsizei)y);
}

void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (b1.insidebutton(x, y))
        {
            b1.togglestate();
        }
        if (b2.insidebutton(x, y))
        {
            b2.togglestate();
        }
        if (b3.insidebutton(x, y))
        {
            b3.togglestate();
        }
        if (b4.insidebutton(x, y))
        {
            b4.togglestate();
        }
        if(cs == 1){
            selGS(x,y);
        }
        if(ce == 1){
            selGE(x,y);
        }
        if(cw == 1){
            selGW(x,y);
        }
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (b1.insidebutton(x, y))
        {
            b1.togglestate();
            if (start != 1)
                start = 1;
        }
        if (b2.insidebutton(x, y))
        {
            b2.togglestate();
            resetG();
            start = 0;
        }
        if (b3.insidebutton(x, y))
        {
            b3.togglestate();
            randWall();
        }
        if (b4.insidebutton(x, y))
        {
            b4.togglestate();
            selGD();
            start = 0;
        }
    }
    glutPostRedisplay();
}

void keyPress(unsigned char key, int xmouse, int ymouse)
{
    if(key == 's'){
        cs = 1;
    }
    if(key == 'e'){
        ce = 1;
    }
    if(key == 'w'){
        cw = 1;
    }
    glutPostRedisplay(); //request display() call ASAP
}


void keyUp(unsigned char key, int xmouse, int ymouse)
{
    if(key == 's'){
        cs = 0;
    }
    if(key == 'e'){
        ce = 0;
    }
    if(key == 'w'){
        cw = 0;
    }
    glutPostRedisplay(); //request display() call ASAP
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(X, Y);
    glutCreateWindow("Path Finding Algorithms Visualization");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(MouseButton);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    init();
    glutMainLoop();
    return 0;
}
