#ifndef ARRAY_H_INCLUDE
#define ARRAY_H_INCLUDE

#define cols 20
#define rows 20

void initArray();
void drawGrid();

class spot{

public:
    int g=0,f=0,h=0;
    spot(){
        this->g;
        this->h;
        this->f;
    }

};

#endif