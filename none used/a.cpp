#include <GL/glut.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <vector>
#include <list>

using std::list;
using std::vector;

float wX = (float)430 / 16, wY = (float)380 / 16;

struct nod{
    int i,j;
};
nod *pat = nullptr;

struct sNode
{
    bool bObstacle = false; // Is the node an obstruction?
    bool bVisited = false;  // Have we searched this node before?
    float fGlobalGoal;      // Distance to goal so far
    float fLocalGoal;       // Distance to goal if we took the alternative route
    int x;                  // Nodes position in 2D space
    int y;
    bool path = false;
    vector<sNode *> vecNeighbours; // Connections to neighbours
    sNode *parent;                 // Node connecting to this node that offers shortest parent
};

sNode *nodes = nullptr;
int nMapWidth = 16;
int nMapHeight = 16;

sNode *nodeStart = nullptr;
sNode *nodeEnd = nullptr;

void doFirst()
{
    nodes = new sNode[nMapWidth * nMapHeight];
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++)
        {
            nodes[y * nMapWidth + x].x = x; // ...because we give each node its own coordinates
            nodes[y * nMapWidth + x].y = y;
            nodes[y * nMapWidth + x].bObstacle = false;
            nodes[y * nMapWidth + x].parent = nullptr;
            nodes[y * nMapWidth + x].bVisited = false;
        }

    // Create connections - in this case nodes are on a regular grid
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++)
        {
            if (y > 0)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
            if (y < nMapHeight - 1)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
            if (x > 0)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
            if (x < nMapWidth - 1)
                nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

            // We can also connect diagonally
            /*if (y>0 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
				if (y<nMapHeight-1 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
				if (y>0 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
				if (y<nMapHeight - 1 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
				*/
        }

    // Manually positio the start and end markers so they are not nullptr

    nodeStart = &nodes[0 * nMapWidth + 0];
    nodeEnd = &nodes[15 * nMapWidth + 15];
}

bool a_start()
{
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++)
        {
            nodes[y * nMapWidth + x].bVisited = false;
            nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
            nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
            nodes[y * nMapWidth + x].parent = nullptr; // No parents
        }

    auto distance = [](sNode *a, sNode *b) // For convenience
    {
        return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    };

    auto heuristic = [distance](sNode *a, sNode *b) // So we can experiment with heuristic
    {
        return distance(a, b);
    };
    // Setup starting conditions
    sNode *nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

    // Add start node to not tested list - this will ensure it gets tested.
    // As the algorithm progresses, newly discovered nodes get added to this
    // list, and will themselves be tested later
    list<sNode *> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);

    // if the not tested list contains nodes, there may be better paths
    // which have not yet been explored. However, we will also stop
    // searching when we reach the target - there may well be better
    // paths but this one will do - it wont be the longest.
    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) // Find absolutely shortest path // && nodeCurrent != nodeEnd)
    {
        // Sort Untested nodes by global goal, so lowest is first
        listNotTestedNodes.sort([](const sNode *lhs, const sNode *rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

        // Front of listNotTestedNodes is potentially the lowest distance node. Our
        // list may also contain nodes that have been visited, so ditch these...
        while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
            listNotTestedNodes.pop_front();

        // ...or abort because there are no valid nodes left to test
        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->bVisited = true; // We only explore a node once

        // Check each of this node's neighbours...
        for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
        {
            // ... and only if the neighbour is not visited and is
            // not an obstacle, add it to NotTested List
            if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
                listNotTestedNodes.push_back(nodeNeighbour);

            // Calculate the neighbours potential lowest parent distance
            float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

            // If choosing to path through this node is a lower distance than what
            // the neighbour currently has set, update the neighbour to use this node
            // as the path source, and set its distance scores as necessary
            if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
            {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

                // The best path length to the neighbour being tested has changed, so
                // update the neighbour's score. The heuristic is used to globally bias
                // the path algorithm, so it knows if its getting better or worse. At some
                // point the algo will realise this path is worse and abandon it, and then go
                // and search along the next best path.
                nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
            }
        }
    }
    pat = new nod[50];
    int index = 0;
    if (nodeEnd != nullptr)
    {
        sNode *p = nodeEnd;
        while (p->parent != nullptr)
        {
            pat[index].i = p->parent->x;
            pat[index].j = p->parent->y;
            p = p->parent;
            index++;
        }
    }
    return true;
}



void drawGrid()
{
    

    float x = 10.0, y = 60.0;
    for (int i = 0; i < nMapHeight; i++)
    {
        for (int j = 0; j < nMapWidth; j++)
        {
            for(int k=0;k<36;k++){
            if (pat[k].i == i && pat[k].j == j)
            {
                glColor3f(1, 1, 0);
            }
            else
            {
                glColor3f(1, 1, 1);
            }}
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

// int main(int argc, char **argv)
// {
//     init();
//     a_start();

//     return 0;
// }

// for(i;i<10;i++){
//     for(j;j<10;j++){

//     }
// }