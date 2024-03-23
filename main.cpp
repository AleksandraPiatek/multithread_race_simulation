#include <iostream>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "graphics.h"
#include "verticalVehicles.h"

graphics graphics;

int argc1;
char** argv1;
void graphicsMain(){
    graphics.initialize(argc1, argv1, graphics);
}

int main(int argc, char** argv)
{
    argc1=argc;
    argv1=argv;
    pthread_t graphics_thread;

    verticalVehicles verticalVehicles;
    graphicsMain();
    //verticalVehicles.movingVehicle(0, graphics);
    return 0;
}

