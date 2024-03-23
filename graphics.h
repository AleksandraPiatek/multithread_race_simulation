//
// Created by aleksandra on 22.03.24.
//

#ifndef SO_1_GRAPHICS_H
#define SO_1_GRAPHICS_H


class graphics {
    void static drawScene();
public:
    void initialize(int argc, char** argv, graphics graphics);
    static void drawVehicle(int vehicleNumber,float oldX, float oldY, float x, float y);
    constexpr static const float speed = 0.01;
    constexpr static const float startingPoints[6] = {-0.45, 0.75, -0.4, 0.7, -0.35, -0.65};

    constexpr static const float path[3][4] = {{0.45, -0.75, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.35, -0.65, -0.35, 0.65}};

    static void movingVehicle(int pathNumber);

};


#endif //SO_1_GRAPHICS_H
