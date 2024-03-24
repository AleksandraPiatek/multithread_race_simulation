//
// Created by aleksandra on 22.03.24.
//

#ifndef SO_1_GRAPHICS_H
#define SO_1_GRAPHICS_H

#include <GL/glx.h>
#include "verticalVehicles.h"
#include <iostream>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <csignal>
#include <unistd.h>



class graphics {
    void drawScene();

    Display *display;
    Window win;
    GLXContext ctx;

 public:
    void initialize(int argc, char** argv, graphics graphics);
    void drawVehicle(int vehicleNumber,float oldX, float oldY, float x, float y);
    constexpr static const float speed = 0.01;
    constexpr static const float startingPoints[6] = {-0.45, 0.75, -0.4, 0.7, -0.35, -0.65};

    constexpr static const float path[3][4] = {{0.45, -0.75, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.35, -0.65, -0.35, 0.65}};

    void movingVehicle(int pathNumber);

};


#endif //SO_1_GRAPHICS_H
