#include <iostream>
#include <GL/glut.h>
#include "button.h"
#include "aStar.h"
#include "labeling.h"

int X = 600;
int Y = 500;
int start = 0;
button b1((char *)"Start", 475, 407, 100, 30);
button b2((char *)"Reset", 475, 367, 100, 30);
button b3((char *)"Random Wall", 475, 327, 100, 30);
button b4((char *)"Diagonal", 475, 287, 100, 30);

label l1((char *)"Reset", 340, 460, 100, 30, 1);
label l2((char *)"Diagonal", 230, 460, 100, 30, 3);
label l3((char *)"NO Path", 120, 460, 100, 30, 1);
label l4((char *)"FOUND", 10, 460, 100, 30, 2);

int cs = 0;
int ce = 0;
int cw = 0;

void init()
{
    glClearColor(8.0 / 255.0f, 0.0 / 255.0f, 15.0 / 255.0f, 0.0);
    first_fun();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, X, Y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    banner();
    info();
    b1.draw();
    b2.draw();
    b3.draw();
    b4.draw();
    l1.draw();
    l2.draw();
    l3.draw();
    l4.draw();
    Grid();
    if (start == 1)
    {
        a_st();
    }
    glutSwapBuffers();
}

void reshape(int x, int y)
{
    glViewport(0, 0, (GLsizei)x, (GLsizei)y);
    glutReshapeWindow(X, Y);
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
        if (cs == 1)
        {
            selGS(x, y);
        }
        if (ce == 1)
        {
            selGE(x, y);
        }
        if (cw == 1)
        {
            selGW(x, y);
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
            l1.state = 0;
            l3.state = 0;
            l4.state = 0;
        }
        if (b3.insidebutton(x, y))
        {
            b3.togglestate();
            randWall();
            randWall();
            randWall();
            randWall();
            randWall();
        }
        if (b4.insidebutton(x, y))
        {
            b4.togglestate();
            selGD();
            start = 0;
            l1.state = 0;
            l3.state = 0;
            l4.state = 0;
        }
    }
    glutPostRedisplay();
}

void keyPress(unsigned char key, int xmouse, int ymouse)
{
    if (key == 's')
    {
        cs = 1;
    }
    if (key == 'e')
    {
        ce = 1;
    }
    if (key == 'w')
    {
        cw = 1;
    }
    glutPostRedisplay(); //request display() call ASAP
}

void keyUp(unsigned char key, int xmouse, int ymouse)
{
    if (key == 's')
    {
        cs = 0;
    }
    if (key == 'e')
    {
        ce = 0;
    }
    if (key == 'w')
    {
        cw = 0;
    }
    glutPostRedisplay(); //request display() call ASAP
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
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
