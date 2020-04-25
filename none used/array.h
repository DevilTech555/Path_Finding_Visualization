#ifndef ARRAY_H_INCLUDE
#define ARRAY_H_INCLUDE

#define cols 5
#define rows 5

void initArray();
void drawGrid();
void aStar();

class spot{

public:
    int g=0,f=0,h=0,i,j;
    int index = 0;
    spot* n[4];
    void addNeg();
};

class neg{

public:
    int g=0,f=0,h=0,i,j;
};

#endif