#include <iostream>
#include <GL/glut.h>
#include "button.h"
#include "array.h"

int X = 600;
int Y = 450;

void init()
{
    glClearColor(0.0,0.0,0.0,0.0);
    initArray();
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
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex2i(450,0);
    glVertex2i(450,450);
    glVertex2f(0,50);
    glVertex2i(450,50);
    glEnd();
    aStar();
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
    glutPostRedisplay();
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
       
    }
}

void tasten(unsigned char key, int xmouse, int ymouse)
{
    switch (key)
    {
    case 32:

        break;
    case 't':
       
    case 'i':
       
        break;
    case 'd':
        
    default:
        break;
    }
    glutPostRedisplay(); //request display() call ASAP
}

void My_timer_event(int te){

glutPostRedisplay();

glutTimerFunc( 1, My_timer_event, 1);
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
    glutKeyboardFunc(tasten);
    glutTimerFunc( 1, My_timer_event, 1);
    init();
    glutMainLoop();
    return 0;
}
