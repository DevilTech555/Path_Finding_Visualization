#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#include <unistd.h>
#include "aStar.h"
#include "labeling.h"

using namespace std;

int dela = 18000;

int d = 0;

extern label l1;
extern label l2;
extern label l3;
extern label l4;

struct node
{
    int i, j, g, h, f;
    float x = 0, y = 0;
    node()
    {
        g = 0;
        h = 0;
        f = 0;
    }
    vector<node *> neghbor;
    node *perv;
    bool wall = false;
};

float wX = (float)430 / cols, wY = (float)380 / rows;
struct node nodes[cols][rows];
vector<node *> openset;
vector<node *> closeset;
node *startnode;
node *endnode;
vector<node *> path;

void drawGrid();
void drawPath();
void drawPathConstant();

bool set1 = true;
bool set = true;
bool done = false;

void randWall()
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            int ri = rand() % cols;
            int rj = rand() % rows;
            if (i == ri)
            {
                nodes[i][j].wall = true;
            }
        }
    }
}

void first_fun()
{
    if (d == 1)
    {
        l2.state = 1;
    }
    else
    {
        l2.state = 0;
    }
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            nodes[i][j].i = i;
            nodes[i][j].j = j;
            nodes[i][j].x = 10 + (i * wX);
            nodes[i][j].y = 60 + (j * wY);
            nodes[i][j].perv = nullptr;
            nodes[i][j].wall = false;
        }
    }
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            if (i < cols - 1)
                nodes[i][j].neghbor.push_back(&nodes[i + 1][j]);
            if (i > 0)
                nodes[i][j].neghbor.push_back(&nodes[i - 1][j]);
            if (j < rows - 1)
                nodes[i][j].neghbor.push_back(&nodes[i][j + 1]);
            if (j > 0)
                nodes[i][j].neghbor.push_back(&nodes[i][j - 1]);
            if (d == 1)
            {
                if (i > 0 && j > 0)
                    nodes[i][j].neghbor.push_back(&nodes[i - 1][j - 1]);
                if (i < cols - 1 && j > 0)
                    nodes[i][j].neghbor.push_back(&nodes[i + 1][j - 1]);
                if (i > 0 && j < rows - 1)
                    nodes[i][j].neghbor.push_back(&nodes[i - 1][j + 1]);
                if (i < cols - 1 && j < rows - 1)
                    nodes[i][j].neghbor.push_back(&nodes[i + 1][j + 1]);
            }
        }
    }
}

void a_st()
{
    while (set)
    {
        if (openset.size() > 0)
        {
            int index = 0;
            for (int i = 0; i < (int)openset.size(); i++)
            {
                if (openset[i]->f < openset[index]->f)
                {
                    index = i;
                }
            }

            node *current = openset[index];
            if (current->i == endnode->i && current->j == endnode->j)
            {
                node *temp = current;
                while (temp->perv != nullptr)
                {
                    path.push_back(temp->perv);
                    temp = temp->perv;
                    path.push_back(temp);
                }
                set = false;
                l1.state = l4.state = 1;
                Grid();
                glutSwapBuffers();
                usleep(dela);
                Grid();
                glutSwapBuffers();
                usleep(dela);
                drawPath();
                break;
            }

            auto el = find(openset.begin(), openset.end(), current);
            openset.erase(el);
            closeset.push_back(current);

            for (auto n : current->neghbor)
            {

                bool foundC = (find(closeset.begin(), closeset.end(), n) != closeset.end());
                if (!foundC && !n->wall)
                {
                    bool newpath = false;
                    int tempG = current->g + 1;
                    bool foundO = (find(openset.begin(), openset.end(), n) != openset.end());
                    if (foundO)
                    {
                        if (tempG < n->g)
                        {
                            n->g = tempG;
                            newpath = true;
                        }
                    }
                    else
                    {
                        n->g = tempG;
                        newpath = true;
                        openset.push_back(n);
                    }
                    if (newpath)
                    {
                        n->h = (abs(n->i - endnode->i) + abs(n->j - endnode->j));
                        n->f = n->g + n->h;
                        n->perv = current;
                    }
                }
            }
            drawGrid();
            glutSwapBuffers();
            usleep(dela);
        }
        else
        {
            set = false;
            l3.state = l1.state = 1;
            usleep(dela);
            break;
        }
    }
    drawPathConstant();
}

void drawGrid()
{
    float x = 10.0, y = 60.0;
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            int o = 0, c = 0;
            for (auto op : openset)
            {
                if (op->i == i && op->j == j)
                {
                    glColor3f(144.0f / 255.0f, 238.0f / 255.0f, 44.0f / 255.0f);
                    o = 1;
                }
            }
            for (auto cp : closeset)
            {
                if (cp->i == i && cp->j == j)
                {
                    glColor3f(255.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f);
                    c = 1;
                }
            }

            if (o == 0 && c == 0)
            {
                glColor3f(1, 1, 1);
            }
            if (startnode != nullptr && startnode->i == i && startnode->j == j)
            {
                glColor3f(1, 0, 0.9);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(0, 0, 0.9);
            }
            if (nodes[i][j].wall)
            {
                glColor3f(0, 0, 0);
            }
            glBegin(GL_POLYGON);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
        }
    }
}

void drawPath()
{
    float x = 10.0, y = 60.0;
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            glColor3f(1, 1, 1);
            if (startnode != nullptr && startnode->i == i && startnode->j == j)
            {
                glColor3f(1, 0, 0.9);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(0, 0, 0.9);
            }
            if (nodes[i][j].wall)
            {
                glColor3f(0, 0, 0);
            }
            glBegin(GL_POLYGON);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
        }
    }
    x = 10.0, y = 60.0;
    for (auto n : path)
    {
        int i = n->i;
        int j = n->j;
        glColor3f(1, 1, 0);
        if (startnode != nullptr && startnode->i == i && startnode->j == j)
        {
            glColor3f(1, 0, 0.9);
        }
        if (endnode != nullptr && endnode->i == i && endnode->j == j)
        {
            glColor3f(0, 0, 0.9);
        }
        if (nodes[i][j].wall)
        {
            glColor3f(0, 0, 0);
        }
        glBegin(GL_POLYGON);
        glVertex2f(x + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + wY + j * wY);
        glVertex2f(x + i * wX, y + wY + j * wY);
        glEnd();
        glColor3f(0, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + wY + j * wY);
        glVertex2f(x + i * wX, y + wY + j * wY);
        glEnd();
        glutSwapBuffers();
        usleep(dela);
    }
}

void drawPathConstant()
{
    float x = 10.0, y = 60.0;
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            glColor3f(1, 1, 1);
            if (startnode != nullptr && startnode->i == i && startnode->j == j)
            {
                glColor3f(1, 0, 0.9);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(0, 0, 0.9);
            }
            if (nodes[i][j].wall)
            {
                glColor3f(0, 0, 0);
            }
            glBegin(GL_POLYGON);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
        }
    }
    x = 10.0, y = 60.0;
    for (auto n : path)
    {
        int i = n->i;
        int j = n->j;
        glColor3f(1, 1, 0);
        if (startnode != nullptr && startnode->i == i && startnode->j == j)
        {
            glColor3f(1, 0, 0.9);
        }
        if (endnode != nullptr && endnode->i == i && endnode->j == j)
        {
            glColor3f(0, 0, 0.9);
        }
        if (nodes[i][j].wall)
        {
            glColor3f(0, 0, 0);
        }
        glBegin(GL_POLYGON);
        glVertex2f(x + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + wY + j * wY);
        glVertex2f(x + i * wX, y + wY + j * wY);
        glEnd();
        glColor3f(0, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + j * wY);
        glVertex2f(x + wX + i * wX, y + wY + j * wY);
        glVertex2f(x + i * wX, y + wY + j * wY);
        glEnd();
    }
    glutSwapBuffers();
    usleep(dela);
}

void Grid()
{
    float x = 10.0, y = 60.0;
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            glColor3f(1, 1, 1);
            if (startnode != nullptr && startnode->i == i && startnode->j == j)
            {
                glColor3f(1, 0, 0.9);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(0, 0, 0.9);
            }
            if (nodes[i][j].wall)
            {
                glColor3f(0, 0, 0);
            }
            glBegin(GL_POLYGON);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + j * wY);
            glVertex2f(x + wX + i * wX, y + wY + j * wY);
            glVertex2f(x + i * wX, y + wY + j * wY);
            glEnd();
        }
    }
}

void selGS(int x, int y)
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            if (nodes[i][j].wall == false && x > nodes[i][j].x && x < nodes[i][j].x + wX && y > nodes[i][j].y && y < nodes[i][j].y + wY)
            {
                startnode = &nodes[i][j];
                if (!openset.empty())
                {
                    openset.pop_back();
                }
                openset.push_back(startnode);
            }
        }
    }
}

void selGE(int x, int y)
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            if (nodes[i][j].wall == false && x > nodes[i][j].x && x < nodes[i][j].x + wX && y > nodes[i][j].y && y < nodes[i][j].y + wY)
            {
                endnode = &nodes[i][j];
            }
        }
    }
}

void selGW(int x, int y)
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            if (x > nodes[i][j].x && x < nodes[i][j].x + wX && y > nodes[i][j].y && y < nodes[i][j].y + wY)
            {
                nodes[i][j].wall = (nodes[i][j].wall == true) ? false : true;
            }
        }
    }
}

void clearNeb()
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            nodes[i][j].neghbor.clear();
        }
    }
}

void selGD()
{
    d = (d == 1) ? 0 : 1;
    resetG();
}

void resetG()
{
    set = true;
    startnode = nullptr;
    endnode = nullptr;
    openset.clear();
    closeset.clear();
    path.clear();
    clearNeb();
    first_fun();
}