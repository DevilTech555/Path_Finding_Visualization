#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "button.h"
#include "array.h"

spot grid[cols][rows];
float wX = (float)430 / cols, wY = (float)380 / rows;
spot start;
spot end;
spot openset[cols * rows];
spot closeset[cols * rows];
int winner = 0;

void spot::addNeg()
{
    if (i < cols - 1)
    {
        n[index] = &grid[i + 1][j];
        if (index < 4)
            index++;
    }
    if (i > 0)
    {
        n[index] = &grid[i - 1][j];
        if (index < 4)
            index++;
    }
    if (j < rows - 1)
    {
        n[index] = &grid[i][j + 1];
        if (index < 4)
            index++;
    }
    if (j > 0)
    {
        n[index] = &grid[i][j - 1];
        if (index < 4)
            index++;
    }
}

void initArray()
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            grid[i][j].i = i;
            grid[i][j].j = j;
        }
    }
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            grid[i][j].addNeg();
        }
    }

    start = grid[0][0];
    end = grid[cols - 1][rows - 1];
    openset[0] = start;
    printf("%d", openset[0].i);
}

void drawGrid()
{
    float x = 10.0, y = 60.0;
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            for (int k = 0; k < cols * rows; k++)
            {
                if (openset[k].i == i && openset[k].j == j)
                {
                    glColor3f(0, 1, 0);
                    break;
                }
                else if (closeset[k].i == i && closeset[k].j == j)
                {
                    glColor3f(1, 0, 0);
                    break;
                }
                else
                    glColor3f(1, 1, 1);
            }
            glBegin(GL_POLYGON);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
            glColor3f(0.5, 0, 0.5);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
        }
    }
}

void removeOpenset(spot t)
{
    int index, n = cols * rows;
    for (int i = 0; i < cols * rows; i++)
    {
        if (openset[i].i == t.i && openset[i].j == t.j)
        {
            index = i;
        }
    }
    if (index < n)
    {
        n = n - 1;
        for (int j = index; j < n; j++)
        {
            openset[j] = openset[j + 1];
        }
    }
}

int hur(spot a,spot b){
    int h = sqrt((a.i-b.i)*2 + (a.j-b.j)*2);
    return h;
}

void aStar()
{
    int flag = 1;
    int index = 0;
    int index2 = 0;
    drawGrid();
    spot current = openset[winner];
    while (current.i != end.i && current.j != end.j)
    {
        for (int i = 0; i < cols * rows; i++)
        {
            if (openset[i].f < openset[winner].f)
            {
                winner = i;
            }
        }
        if (current.i == end.i && current.j == end.j)
        {
            flag = 0;
            break;
        }

        removeOpenset(current);
        closeset[index] = current;
        if (index < cols * rows)
            index++;

        for (int i = 0; i < current.index; i++)
        {
            for (int j = 0; j < cols * rows; j++)
            {
                if (!(closeset[0].n[i]->i == current.n[i]->i && closeset[0].n[i]->j == current.n[i]->j))
                {
                    int tempG = current.n[i]->g+1;
                    for (int k = 0; k < cols * rows; j++)
                    {
                        if (openset[0].n[i]->i == current.n[i]->i && openset[0].n[i]->j == current.n[i]->j)
                        {
                            if(tempG < current.g){
                                current.n[i]->g = tempG;
                            }
                        }else{
                            current.n[i]->g = tempG;
                            openset[index2] = *current.n[i];
                            index2++;
                        }
                    }
                    current.n[i]->h = hur(*current.n[i],end);
                    current.n[i]->f = current.n[i]->f + current.n[i]->h;
                }
            }
        }
        drawGrid();
        glutSwapBuffers();
        usleep(100000);
    }
}
