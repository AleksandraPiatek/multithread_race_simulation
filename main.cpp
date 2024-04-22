#include <GL/glut.h>
#include <thread>
#include <vector>
#include <iostream>
#include <random>
#include <GL/freeglut.h>
#include <csignal>
#include <algorithm>

// Structure to hold data needed by each drawing thread
struct ThreadData {
    double objectPositionX;
    double objectPositionY;
    int vehicleNumber;
    double speed;
};

// Global variables to hold data for each vehicle
ThreadData threadData1, threadData2, threadData3, threadData4, threadData5, threadData6;

bool stop = false;
bool windowClosed = false;
int amountOfHorizontalThreadsActive = 0;
bool vehicleSpawned[3] = {true, true, true};
float startingPoints[6] = {-0.47, 0.77, -0.4, 0.7, -0.37, 0.67};
float path[3][4] = {{0.47, -0.77, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.37, -0.67, -0.37, 0.67}};
float colors[3][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}};

std::default_random_engine gen;
std::uniform_real_distribution<double> distribution(0.0001,0.04);

std::vector<std::thread> activeThreads;

void drawVehicle(double x, double y, int color) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(colors[color][0], colors[color][1], colors[color][2]);
    glBegin(GL_QUADS);
    glVertex2f(-0.01f, -0.01f);
    glVertex2f(0.01f, -0.01f);
    glVertex2f(0.01f, 0.01f);
    glVertex2f(-0.01f, 0.01f);
    glEnd();
    glPopMatrix();
}

// Function to initialize OpenGL
void initOpenGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void displayScene() {
    float backgroundColor[3] = {1, 1, 0.924};
    float course1Color[3] = {0.8, 0.8, 1};
    float course2Color[3] = {0.6, 0.6, 1};

    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(course1Color[0], course1Color[1], course1Color[2]);
    glVertex2f(-0.8, 0.5);
    glVertex2f(0.8, 0.5);
    glVertex2f(0.8, -0.5);
    glVertex2f(-0.8, -0.5);

    glColor3f(course1Color[0]-0.1, course1Color[1]-0.1, course1Color[2]);
    glVertex2f(-0.8, 0.5);
    glVertex2f(-1, 0.5);
    glVertex2f(-1, 0.3);
    glVertex2f(-0.8, 0.3);

    glVertex2f(-0.8, -0.5);
    glVertex2f(-1, -0.5);
    glVertex2f(-1, -0.3);
    glVertex2f(-0.8, -0.3);

    glColor3f(course2Color[0], course2Color[1], course2Color[2]);
    glVertex2f(-0.5, 0.8);
    glVertex2f(0.5, 0.8);
    glVertex2f(0.5, -0.8);
    glVertex2f(-0.5, -0.8);

    glColor3f(course1Color[0], course1Color[1], course1Color[2]);
    glVertex2f(-0.5, 0.3);
    glVertex2f(0.5, 0.3);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);

    glVertex2f(-0.5, -0.3);
    glVertex2f(0.5, -0.3);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);

    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    glVertex2f(-0.35, 0.3);
    glVertex2f(0.35, 0.3);
    glVertex2f(0.35, -0.3);
    glVertex2f(-0.35, -0.3);

    glVertex2f(-0.35, 0.65);
    glVertex2f(0.35, 0.65);
    glVertex2f(0.35, 0.5);
    glVertex2f(-0.35, 0.5);

    glVertex2f(-0.35, -0.5);
    glVertex2f(0.35, -0.5);
    glVertex2f(0.35, -0.65);
    glVertex2f(-0.35, -0.65);

    glVertex2f(0.65, 0.3);
    glVertex2f(0.65, -0.3);
    glVertex2f(0.5, -0.3);
    glVertex2f(0.5, 0.3);

    glVertex2f(-0.65, 0.3);
    glVertex2f(-0.65, -0.3);
    glVertex2f(-0.5, -0.3);
    glVertex2f(-0.5, 0.3);
    glEnd();

    drawVehicle(threadData1.objectPositionX, threadData1.objectPositionY, threadData1.vehicleNumber);
    drawVehicle(threadData2.objectPositionX, threadData2.objectPositionY, threadData2.vehicleNumber);
    drawVehicle(threadData3.objectPositionX, threadData3.objectPositionY, threadData3.vehicleNumber);
    drawVehicle(threadData4.objectPositionX, threadData4.objectPositionY, threadData4.vehicleNumber);
    drawVehicle(threadData5.objectPositionX, threadData5.objectPositionY, threadData5.vehicleNumber);
    drawVehicle(threadData6.objectPositionX, threadData6.objectPositionY, threadData6.vehicleNumber);

    glutSwapBuffers();
    glutPostRedisplay();
}

void updateVerticalVehicle(ThreadData* threadData) {
    double x = threadData->objectPositionX, y = threadData->objectPositionY;
    while (!stop && !windowClosed) {
        while (x < path[threadData->vehicleNumber][0] && y > 0.65 && !stop && !windowClosed) {
            x += threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y > path[threadData->vehicleNumber][1] && x > 0.35 && !stop && !windowClosed) {
            y -= threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (x > path[threadData->vehicleNumber][2] && y < -0.65 && !stop && !windowClosed) {
            x -= threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y < path[threadData->vehicleNumber][3] && x < -0.35 && !stop && !windowClosed) {
            y += threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
}

void updateHorizontalVehicle(ThreadData* threadData) {
    std::uniform_int_distribution<int> distribution1(10000, 1500000);
    usleep(distribution1(gen));
    double y = -startingPoints[2 * threadData->vehicleNumber], x = -1;
    amountOfHorizontalThreadsActive++;
    if (!stop && !windowClosed) {
        while (x < -startingPoints[2 * threadData->vehicleNumber + 1]) {
            x += threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (!stop && !windowClosed) {
            while (x < path[threadData->vehicleNumber][3] && !stop && !windowClosed) {
                x += threadData->speed;
                usleep(10000);
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (y > path[threadData->vehicleNumber][2] && !stop && !windowClosed) {
                y -= threadData->speed;
                usleep(10000);
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (x > path[threadData->vehicleNumber][1] && !stop && !windowClosed) {
                x -= threadData->speed;
                usleep(10000);
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            if (i != 2) {
                while (y < path[threadData->vehicleNumber][0] && !stop && !windowClosed) {
                    y += threadData->speed;
                    usleep(10000);
                    threadData->objectPositionX = x;
                    threadData->objectPositionY = y;
                }
            }
        }
    }
    if (!stop && !windowClosed) {
        while (x >= -1) {
            x -= threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    amountOfHorizontalThreadsActive--;

    // Remove this thread from activeThreads vector
    auto it = std::find_if(activeThreads.begin(), activeThreads.end(),
                           [&](std::thread& t) { return t.get_id() == std::this_thread::get_id(); });
    if (it != activeThreads.end()) {
        it->detach(); // Detach before erasing
        activeThreads.erase(it);
    }
    vehicleSpawned[threadData->vehicleNumber] = false;
}

void horizontalVehiclesHandler() {
    while (!stop && !windowClosed) {
        if (amountOfHorizontalThreadsActive == 3) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (stop || windowClosed) {
            break;
        }

        int index = -1;
        for (int i = 0; i < 3; ++i) {
            if (!vehicleSpawned[i]) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            vehicleSpawned[index] = true;
            ThreadData* threadData = nullptr;

            if (index == 0) {
                threadData = &threadData4;
            } else if (index == 1) {
                threadData = &threadData5;
            } else if (index == 2) {
                threadData = &threadData6;
            }

            threadData->speed = distribution(gen);
            std::thread horizontal_thread(updateHorizontalVehicle, threadData);
            activeThreads.push_back(std::move(horizontal_thread));
        }
    }
}

void stopFunction(unsigned char key, int x, int y) {
    if (key == ' ') {
        stop = true;
        // Join all active threads
        for (auto& thread : activeThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
}

void closeWindowFunction() {
    windowClosed = true;
    // Join all active threads
    for (auto& thread : activeThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void startingPointRandomizer(ThreadData& threadData) {
    std::uniform_real_distribution<double> distribution2(-0.5, 0.5);
    double x = distribution2(gen);
    double y;
    if (x > 0.35 || x < -0.35) {
        std::uniform_real_distribution<double> distribution3(-0.8, 0.8);
        y = distribution3(gen);
    } else {
        std::uniform_real_distribution<double> distribution3(0.65, 0.8);
        y = distribution3(gen);
    }

    threadData.objectPositionX = x;
    threadData.objectPositionY = y;
    std::cout << threadData.vehicleNumber << " " << threadData.objectPositionX << " " << threadData.objectPositionY << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("");
    glutKeyboardFunc(stopFunction);
    glutCloseFunc(closeWindowFunction);

    initOpenGL();

    startingPointRandomizer(threadData1);
    threadData1.vehicleNumber = 0;
    threadData1.speed = 0.01;

    startingPointRandomizer(threadData2);
    threadData2.vehicleNumber = 1;
    threadData2.speed = 0.01;

    startingPointRandomizer(threadData3);
    threadData3.vehicleNumber = 2;
    threadData3.speed = 0.01;

    threadData4.vehicleNumber = 0;
    threadData4.speed = distribution(gen);

    threadData5.vehicleNumber = 1;
    threadData5.speed = distribution(gen);

    threadData6.vehicleNumber = 2;
    threadData6.speed = distribution(gen);

    std::thread thread1(updateVerticalVehicle, &threadData1);
    std::thread thread2(updateVerticalVehicle, &threadData2);
    std::thread thread3(updateVerticalVehicle, &threadData3);
    std::thread thread4(horizontalVehiclesHandler);
    std::thread thread5(updateHorizontalVehicle, &threadData4);
    std::thread thread6(updateHorizontalVehicle, &threadData5);
    std::thread thread7(updateHorizontalVehicle, &threadData6);

    activeThreads.push_back(std::move(thread1));
    activeThreads.push_back(std::move(thread2));
    activeThreads.push_back(std::move(thread3));
    activeThreads.push_back(std::move(thread4));
    activeThreads.push_back(std::move(thread5));
    activeThreads.push_back(std::move(thread6));
    activeThreads.push_back(std::move(thread7));

    glutDisplayFunc(displayScene);

    glutMainLoop();

    return 0;
}
