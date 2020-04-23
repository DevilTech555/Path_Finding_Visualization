#include <GL/glut.h>
#include <iostream>
#include "button.h"
#include "array.h"

spot grid[cols][rows];
float wX=(float)430/cols,wY=(float)380/rows;

void initArray(){
    for(int i=0;i<cols;i++){
        for(int j=0;j<rows;j++){
            grid[i][j] = spot();
        }
    }
    printf("%f  %f",wX,wY);
}

void drawGrid(){
    float x=10.0,y=60.0;
    for(int i=0;i<cols;i++){
        for(int j=0;j<rows;j++){
            if(i==0&&j==0)
                glColor3f(1,0,0);
            else if(i==cols-1&&j==rows-1)
                glColor3f(0,1,0);
            else 
                glColor3f(1,1,1);
            glBegin(GL_POLYGON);
		    glVertex2f(x+i*wX ,y+j*wY);
		    glVertex2f(x+wX+i*wX ,y+j*wY);
		    glVertex2f(x+wX+i*wX ,y+wY+j*wY);
		    glVertex2f(x+i*wX ,y+wY+j*wY);
	        glEnd();
            glColor3f(0.5,0,0.5);
            glBegin(GL_LINE_LOOP);
		    glVertex2f(x+i*wX ,y+j*wY);
		    glVertex2f(x+wX+i*wX ,y+j*wY);
		    glVertex2f(x+wX+i*wX ,y+wY+j*wY);
		    glVertex2f(x+i*wX ,y+wY+j*wY);
	        glEnd();
        }
    }
}