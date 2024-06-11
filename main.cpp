#include <GL/glut.h>
#include <thread>
#include <vector>
#include <iostream>
#include <random>
#include <GL/freeglut.h>
#include <memory>
#include <unistd.h>
#include <shared_mutex>


struct ThreadData {
    double objectPositionX;
    double objectPositionY;
    int vehicleNumber;
    double speed;
};

ThreadData threadData1, threadData2, threadData3;
bool stop = false;
bool windowClosed = false;
float step = 0.0001;
float startingPoints[6] = {-0.47, 0.77, -0.4, 0.7, -0.37, 0.67};
float path[3][4] = {{0.47, -0.77, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.37, -0.67, -0.37, 0.67}};
float colors[20][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 0.5, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.5, 0.0, 1.0}, {0.0, 0.5, 1.0}, {0.5, 1.0, 0.0}, {1.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {0.5, 0.5, 0.5}, {0.75, 0.75, 0.75}, {0.5, 1.0, 1.0}, {1.0, 0.5, 1.0}, {1.0, 1.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 0.5, 1.0}};

std::default_random_engine gen;

std::vector<std::thread> activeThreads;
std::vector<std::shared_ptr<ThreadData>> activeHorizontalThreadsData;

std::shared_mutex crossroadMutexes[4];

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

    for (const auto& data : activeHorizontalThreadsData) {
        drawVehicle(data->objectPositionX, data->objectPositionY, data->vehicleNumber);
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void updateVerticalVehicle(ThreadData* threadData) {
    double x = threadData->objectPositionX, y = threadData->objectPositionY;
    while (!stop && !windowClosed) {

        // Move the vehicle
        while (x < path[threadData->vehicleNumber][0] && y > 0.65 && !stop && !windowClosed) {
            x += step;
            usleep(threadData->speed);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y > path[threadData->vehicleNumber][1] && x > 0.35 && !stop && !windowClosed) {
            if (y >= 0.3 && y <= 0.5) {
                crossroadMutexes[1].try_lock(); //skrzyżowanie nr 2!!!!
            }
            else if (y <= -0.3 && y >= -0.5) {
                crossroadMutexes[2].try_lock(); //skrzyżowanie 3!
            }
            else if (y <= -0.5 && y >= -0.5002) {
                crossroadMutexes[2].unlock();
                std::cout << "Unlocked mutex for crossroad 4" << std::endl;
            }
            else if(y<=0.3 && y>=0.2998){
                crossroadMutexes[1].unlock();
            }
            y -= step;
            usleep(threadData->speed);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (x > path[threadData->vehicleNumber][2] && y < -0.65 && !stop && !windowClosed) {
            x -= step;
            usleep(threadData->speed);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y < path[threadData->vehicleNumber][3] && x < -0.35 && !stop && !windowClosed) {
            if (y >= 0.3 && y <= 0.5) {  //skrzyżowanie nr 1!!!
                crossroadMutexes[3].try_lock();
            }
             else if (y <= -0.3 && y >= -0.5) {
                crossroadMutexes[0].try_lock(); //skrzyzowanie nr 4
            }
            else if (y >= -0.2999 && y <= -0.2997) {
                crossroadMutexes[0].unlock();
                std::cout << "Unlocked mutex for crossroad 4" << std::endl;
            }
            else if(y>=0.5 && y<=0.5002){
                crossroadMutexes[3].unlock();
            }
            y += step;
            usleep(threadData->speed);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
}

void updateHorizontalVehicle(std::shared_ptr<ThreadData> threadData) {
    int vehicleNumber = threadData->vehicleNumber % 3;
    double y = -startingPoints[2 * vehicleNumber], x = -1;


    if (!stop && !windowClosed) {
        while (x < -startingPoints[2 * vehicleNumber + 1]) {
            x += step;
            usleep(threadData->speed);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    for (int i = 0; i < 3; i++) {
        bool crossroadClear = true;
        for (auto & crossroadMutex : crossroadMutexes) {
            if (crossroadMutex.try_lock()) {
                crossroadMutex.unlock(); // Release the lock immediately
            } else {
                crossroadClear = false; // Crossroad is not clear, set flag to false
                break;
            }
        }

        // If the crossroad is not clear, wait
        if (!crossroadClear) {
            usleep(100000); // Wait for a short time before re-checking
            continue;
        }
        if (!stop && !windowClosed) {
            while (x < path[vehicleNumber][3] && !stop && !windowClosed) {
                x += step;
                usleep(threadData->speed);
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (y > path[vehicleNumber][2] && !stop && !windowClosed) {
                y -= step;
                usleep(threadData->speed);
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (x > path[vehicleNumber][1] && !stop && !windowClosed) {
                x -= step;
                usleep(threadData->speed);
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            if (i != 2) {
                while (y < path[vehicleNumber][0] && !stop && !windowClosed) {
                    y += step;
                    usleep(threadData->speed);
                    threadData->objectPositionX = x;
                    threadData->objectPositionY = y;
                }
            }
        }
    }
    if (!stop && !windowClosed) {
        while (x >= -1.05) {
            x -= step;
            usleep(threadData->speed);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
}


void horizontalVehiclesHandler() {
    int vehicleCounter = 3;

    while (!stop && !windowClosed) {
        // Generate a random sleep time between spawns
        std::uniform_int_distribution<int> sleepDistribution(2000, 7000);
        int sleepTime = sleepDistribution(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        // Generate a random speed for the vehicle
        std::uniform_real_distribution<double> speedDistribution(1,200);
        double speed = speedDistribution(gen);


        auto threadData = std::make_shared<ThreadData>();
        threadData->speed = speed;
        threadData->vehicleNumber = vehicleCounter; // Assign unique vehicle ID
        threadData->objectPositionX = -1;
        threadData->objectPositionY = -startingPoints[2* (threadData->vehicleNumber)%3 +1];
        vehicleCounter++;
        if (vehicleCounter == 20) {
            vehicleCounter = 0;
        }
        std::cout << "Spawned new horizontal vehicle: ID " << threadData->vehicleNumber << ", Speed " << speed << std::endl;
        activeHorizontalThreadsData.push_back(threadData);
        std::thread horizontal_thread(updateHorizontalVehicle, threadData);
        activeThreads.push_back(std::move(horizontal_thread));
    }
}

void stopFunction(unsigned char key, int x, int y) {
    if (key == ' ') {
        stop = true;
        for (auto& thread : activeThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
}

void closeWindowFunction() {
    windowClosed = true;
    stop=true;
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
    threadData1.speed = 70;

    startingPointRandomizer(threadData2);
    threadData2.vehicleNumber = 1;
    threadData2.speed = 70;

    startingPointRandomizer(threadData3);
    threadData3.vehicleNumber = 2;
    threadData3.speed = 70;

    std::shared_ptr<ThreadData> threadData7 = std::make_shared<ThreadData>();
    std::shared_ptr<ThreadData> threadData6 =  std::make_shared<ThreadData>();
    std::shared_ptr<ThreadData> threadData5 = std::make_shared<ThreadData>();

    std::uniform_real_distribution<double> speedDistribution(1, 200);
    double speed = speedDistribution(gen);

    threadData5->speed = speed;
    threadData5->vehicleNumber = 0;
    threadData5->objectPositionX = -1;
    threadData5->objectPositionY = -startingPoints[2* (threadData5->vehicleNumber) +1];

    speed = speedDistribution(gen);
    threadData6->speed = speed;
    threadData6->vehicleNumber = 1;
    threadData6->objectPositionX = -1;
    threadData6->objectPositionY = -startingPoints[2* (threadData6->vehicleNumber) +1];

    speed = speedDistribution(gen);
    threadData7->speed = speed;
    threadData7->vehicleNumber = 2;
    threadData7->objectPositionX = -1;
    threadData7->objectPositionY = -startingPoints[2* (threadData7->vehicleNumber) +1];


    std::thread thread1(updateVerticalVehicle, &threadData1);
    std::thread thread2(updateVerticalVehicle, &threadData2);
    std::thread thread3(updateVerticalVehicle, &threadData3);
    std::thread thread4(horizontalVehiclesHandler);
    std::thread thread5(updateHorizontalVehicle, threadData5);
    std::thread thread6(updateHorizontalVehicle, threadData6);
    std::thread thread7(updateHorizontalVehicle, threadData7);


    activeHorizontalThreadsData.push_back(threadData5);
    activeHorizontalThreadsData.push_back(threadData6);
    activeHorizontalThreadsData.push_back(threadData7);

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