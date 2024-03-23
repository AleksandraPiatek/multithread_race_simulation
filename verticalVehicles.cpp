#include "verticalVehicles.h"

void verticalVehicles::movingVehicle(int pathNumber, graphics graphics) {
    float x = startingPoints[pathNumber], y = startingPoints[pathNumber+1];
    //graphics.drawVehicle(pathNumber, x, y);
    while(true){
        while(x<path[pathNumber][0]){
            x+=speed;

        }
        while(y>path[pathNumber][1]){
            y-=speed;
        }
        while(x>path[pathNumber][2]){
            x-=speed;
        }
        while(y<path[pathNumber][3]){
            y+=speed;
        }
    }

}
