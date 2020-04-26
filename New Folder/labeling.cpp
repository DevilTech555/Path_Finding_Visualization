#include <GL/glut.h>
#include "labeling.h"

int fontx,fonty;

void f(void *font, char *text, int x, int y)
{
    glRasterPos2i(x, y);
    while (*text != '\0')
    {
        glutBitmapCharacter(font, *text);
        ++text;
    }
}

void label::draw()
{
    switch (C)
    {
    case 1:
        if (state == 0)
            glColor3f(38.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
        else
            glColor3f(255.0f / 255.0f, 12.0f / 255.0f, 12.0f / 255.0f);
        break;
    case 2:
        if (state == 0)
            glColor3f(1.0f / 255.0f, 25.0f / 255.0f, 16.0f / 255.0f);
        else
            glColor3f(78.0f / 255.0f, 228.0f / 255.0f, 78.0f / 255.0f);
        break;
    case 3:
        if (state == 0)
            glColor3f(0.0f / 255.0f, 26.0f / 255.0f, 51.0f / 255.0f);
        else
            glColor3f(0.0f / 255.0f, 188.0f / 255.0f, 247.0f / 255.0f);
        break;

    default:
        break;
    }
    glBegin(GL_QUADS);
    glVertex2i(X, Y);
    glVertex2i(X, Y + H);
    glVertex2i(X + W, Y + H);
    glVertex2i(X + W, Y);
    glEnd();
    fontx = X + (W - glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char *)text)) / 2;
    fonty = Y + (H + 10) / 2;
    switch (C)
    {
    case 1:
        if (state != 0)
            glColor3f(0,0,0);
        else
            glColor3f(1,1,1);
        break;
    case 2:
        if (state != 0)
            glColor3f(0,0,0);
        else
            glColor3f(1,1,1);
        break;
    case 3:
        if (state != 0)
            glColor3f(0,0,0);
        else
            glColor3f(1,1,1);
        break;

    default:
        break;
    }
    f(GLUT_BITMAP_8_BY_13, text, fontx, fonty);
    glutPostRedisplay();
}


void banner(){
    glColor3f(1, 0, 0);
    glLineWidth(1.5);
    glBegin(GL_LINES);
    glVertex2i(450, 0);
    glVertex2i(450, 500);
    glVertex2f(0, 50);
    glVertex2i(600, 50);
    glVertex2f(0, 450);
    glVertex2i(600, 450);
    glVertex2i(450,275);
    glVertex2i(600,275);
    glEnd();
    glColor3f(1, 1, 1);
    f(GLUT_BITMAP_9_BY_15,(char *)"BY : Naveen PM",460,470);
    f(GLUT_BITMAP_9_BY_15,(char *)"( 1SK18CS413 )",460,490);
    fontx = 5 + (450 - glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"A* Path Finding Algorithm")) / 2;
    fonty = 5 + (50 + 10) / 2;
    f(GLUT_BITMAP_TIMES_ROMAN_24, (char *)"A* Path Finding Algorithm", fontx, fonty);
    fontx = 450 + (150 - glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Info")) / 2;
    fonty = 5 + (50 + 10) / 2;
    f(GLUT_BITMAP_TIMES_ROMAN_24, (char *)"Info", fontx, fonty);
    glColor3f(252.0f/255.f, 244.0f/255.f, 233.0f/255.f);
    glBegin(GL_QUADS);
    glVertex2i(451, 51);
    glVertex2i(451, 49 + 225);
    glVertex2i(451 + 150, 49 + 225);
    glVertex2i(451 + 150, 51);
    glColor3f(139.0f/255.f,90.0f/255.f,43.0f/255.f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2i(0, 51);
    glVertex2i(0, 449);
    glVertex2i(449, 449);
    glVertex2i(449, 51);
    glEnd();
}

void info(){
    glColor3f(0,0,0);
    f(GLUT_BITMAP_9_BY_15, (char *)"white -> Nodes", 460, 80);
    f(GLUT_BITMAP_9_BY_15, (char *)"Black -> Wall", 460, 100);
    f(GLUT_BITMAP_9_BY_15, (char *)"Pink -> Start", 460, 120);
    f(GLUT_BITMAP_9_BY_15, (char *)"Blue -> End", 460, 140);
    f(GLUT_BITMAP_9_BY_15, (char *)"Red -> Visted", 460, 160);
    f(GLUT_BITMAP_9_BY_15, (char *)"Green-> Waiting", 460, 180);
    f(GLUT_BITMAP_9_BY_15, (char *)"    for Visting", 460, 200);
    f(GLUT_BITMAP_9_BY_15, (char *)"Yellow -> Path", 460, 220);

}