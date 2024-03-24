#ifndef SO_1_VERTICALVEHICLES_H
#define SO_1_VERTICALVEHICLES_H


#include "graphics.h"

class verticalVehicles {
    float speed = 0.01;
    float startingPoints[6] = {-0.45, 0.75, -0.4, 0.7, -0.35, -0.65};

    float path[3][4] = {{0.45, -0.75, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.35, -0.65, -0.35, 0.65}};

public:
    //void movingVehicle(int pathNumber, graphics graphics);
};


#endif //SO_1_VERTICALVEHICLES_H
