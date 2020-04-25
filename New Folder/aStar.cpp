#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#include <unistd.h>
#include "aStar.h"

using namespace std;

int dela = 10000;

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

void first_fun()
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            nodes[i][j].i = i;
            nodes[i][j].j = j;
            nodes[i][j].x = 10 + (i * wX);
            nodes[i][j].y = 60 + (j * wY);
            nodes[i][j].perv = nullptr;
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
        }
    }
}

void a_st()
{
    int index = 0;
    while (set)
    {
        if (openset.size() > 0)
        {
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
                path.push_back(current);
                node *temp = current;
                while (temp->perv != nullptr)
                {
                    path.push_back(temp->perv);
                    temp = temp->perv;
                    path.push_back(temp);
                }
                set = false;
                cout << "done" << endl;
                drawPath();
                break;
            }

            auto el = find(openset.begin(), openset.end(), current);
            openset.erase(el);
            closeset.push_back(current);

            for (auto n : current->neghbor)
            {

                bool foundC = (find(closeset.begin(), closeset.end(), n) != closeset.end());
                if (!foundC)
                {
                    int tempG = current->g + 1;
                    bool foundO = (find(openset.begin(), openset.end(), n) != openset.end());
                    if (foundO)
                    {
                        if (tempG < n->g)
                        {
                            n->g = tempG;
                        }
                    }
                    else
                    {
                        n->g = tempG;
                        openset.push_back(n);
                    }
                    // for diagonal
                    // n->h = (int)sqrt((n->i-endnode->i)*2 + (n->j-endnode->j)*2);
                    // for non diagonal
                    n->h = (abs(n->i - endnode->i) + abs(n->j - endnode->j));
                    n->f = n->g + n->h;
                    n->perv = current;
                    // cout << n->f << " " << n->g << endl;
                }
            }
            drawGrid();
            glutSwapBuffers();
            usleep(dela);
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
                    glColor3f(0, 1, 0);
                    o = 1;
                }
            }
            for (auto cp : closeset)
            {
                if (cp->i == i && cp->j == j)
                {
                    glColor3f(1, 0, 0);
                    c = 1;
                }
            }

            if (o == 0 && c == 0)
            {
                glColor3f(1, 1, 1);
            }
            if (startnode != nullptr && startnode->i == i && startnode->j == j)
            {
                glColor3f(0, 0, 1);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(1, 0, 1);
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
                glColor3f(0, 0, 1);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(1, 0, 1);
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
            glColor3f(0, 0, 1);
        }
        if (endnode != nullptr && endnode->i == i && endnode->j == j)
        {
            glColor3f(1, 0, 1);
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
                glColor3f(0, 0, 1);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(1, 0, 1);
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
            glColor3f(0, 0, 1);
        }
        if (endnode != nullptr && endnode->i == i && endnode->j == j)
        {
            glColor3f(1, 0, 1);
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
                glColor3f(0, 0, 1);
            }
            if (endnode != nullptr && endnode->i == i && endnode->j == j)
            {
                glColor3f(1, 0, 1);
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
            if (x > nodes[i][j].x && x < nodes[i][j].x + wX && y > nodes[i][j].y && y < nodes[i][j].y + wY)
            {
                std::cout << i << " , " << j << std::endl;
                startnode = &nodes[i][j];
                if(!openset.empty()){
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
            if (x > nodes[i][j].x && x < nodes[i][j].x + wX && y > nodes[i][j].y && y < nodes[i][j].y + wY)
            {
                std::cout << i << " , " << j << std::endl;
                endnode = &nodes[i][j];
            }
        }
    }
}

void resetG()
{
    set = true;
    startnode = nullptr;
    endnode = nullptr;
    openset.clear();
    closeset.clear();
    path.clear();
    first_fun();
}