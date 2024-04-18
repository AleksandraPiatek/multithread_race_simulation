#include <GL/glut.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <bits/stdc++.h>

// Structure to hold data needed by each drawing thread
struct ThreadData {
    double objectPositionX;
    double objectPositionY;
    int vehicleNumber;
    double speed;
};

// Global variables to hold data for each rectangle
ThreadData threadData1, threadData2, threadData3, threadData4, threadData5, threadData6;

bool stop = false;
int amountOfHorizontalThreadsActive = 0;
bool vehicleSpawned[3] = {true, true, true};
float startingPoints[6] = {-0.47, 0.77, -0.4, 0.7, -0.37, 0.67};
float path[3][4] = {{0.47, -0.77, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.37, -0.67, -0.37, 0.67}};
float colors[3][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}};

// Mutex and condition variable for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

std::default_random_engine gen;
std::uniform_real_distribution<double> distribution(0.0001,0.04);

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
    // Lock the mutex before accessing shared data
    pthread_mutex_lock(&mutex);

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

    // Swap buffers to displayScene the scene
    glutSwapBuffers();

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    // Request a redraw
    glutPostRedisplay();
}

// Function to update object position for the vertical vehicle thread
void* updateVerticalVehicle(void * arg) {
    auto* threadData = reinterpret_cast<ThreadData*>(arg);
    double x = threadData->objectPositionX, y = threadData->objectPositionY;
    while (!stop) {
        while (x < path[threadData->vehicleNumber][0] && y> 0.65 && !stop) {
            x += threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y > path[threadData->vehicleNumber][1] && x>0.35 && !stop) {
            y -= threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (x > path[threadData->vehicleNumber][2] && y < -0.65 && !stop) {
            x -= threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y < path[threadData->vehicleNumber][3] && x<-0.35 && !stop) {
            y += threadData->speed;
            usleep(10000);
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    return nullptr;
}

void* updateHorizontalVehicle(void * arg){
    std::uniform_int_distribution<int> distribution1(10000, 1500000);
    usleep(distribution1(gen));
    auto* threadData = reinterpret_cast<ThreadData*>(arg);
    double y = -startingPoints[2*threadData->vehicleNumber], x = -1;
    amountOfHorizontalThreadsActive++;
    if(!stop) {
        while (x < -startingPoints[2 * threadData->vehicleNumber + 1]) {
            x += threadData->speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    for(int i=0; i<3; i++) {
        if (!stop) {

            while (x < path[threadData->vehicleNumber][3] && !stop) {
                x += threadData->speed;
                usleep(10000); // Sleep for 10 milliseconds
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (y > path[threadData->vehicleNumber][2] && !stop) {
                y -= threadData->speed;
                usleep(10000); // Sleep for 10 milliseconds
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (x > path[threadData->vehicleNumber][1] && !stop) {
                x -= threadData->speed;
                usleep(10000); // Sleep for 10 milliseconds
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            if(i !=2) {
                while (y < path[threadData->vehicleNumber][0] && !stop) {
                    y += threadData->speed;
                    usleep(10000); // Sleep for 10 milliseconds
                    threadData->objectPositionX = x;
                    threadData->objectPositionY = y;
                }
            }
            // Unlock the mutex

        }
    }
    if(!stop) {
        while (x >= -1) {
            x -= threadData->speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    amountOfHorizontalThreadsActive--;
    vehicleSpawned[threadData->vehicleNumber] = false;
    pthread_cond_broadcast(&cond);
    return nullptr;

}

void* horizontalVehiclesHandler(){
    while (!stop) {
        pthread_mutex_lock(&mutex);
        if(amountOfHorizontalThreadsActive==3) pthread_cond_wait(&cond, &mutex);

        // Check if the program is still running
        if (stop) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        // Find an available slot for a new vehicle
        int index = -1;
        for (int i = 0; i < 3; ++i) {
            if (!vehicleSpawned[i]) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            vehicleSpawned[index] = true; // Mark vehicle as spawned
            pthread_t thread;
            ThreadData* threadData = nullptr;

            // Assign threadData based on index
            if (index == 0) {
                threadData = &threadData4;
            } else if (index == 1) {
                threadData = &threadData5;
            } else if (index == 2) {
                threadData = &threadData6;
            }

            threadData->speed= distribution(gen);
            if (pthread_create(&thread, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(threadData)) != 0) {
                std::cerr << "Error: Thread creation failed!" << std::endl;
            }
        }

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }

    return nullptr;
}

void stopFunction(unsigned char key){
    if(key == ' '){
        stop=true;
    }
}

void startingPointRandomizer(ThreadData &threadData){
    std::uniform_real_distribution<double> distribution2(-0.5, 0.5);
    double x = distribution2(gen);
    double y;
    if(x>0.35 || x<-0.35){
        std::uniform_real_distribution<double> distribution3(-0.8, 0.8);
        y = distribution3(gen);
    }
    else{
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
    glutCreateWindow("OpenGL Thread Example");
    glutKeyboardFunc(reinterpret_cast<void (*)(unsigned char, int, int)>(stopFunction));

    // Initialize OpenGL
    initOpenGL();

    // Initialize data for the objects
    startingPointRandomizer(threadData1);
    threadData1.vehicleNumber = 0;
    threadData1.speed=0.01;

    startingPointRandomizer(threadData2);
    threadData2.vehicleNumber = 1;
    threadData2.speed=0.01;

    startingPointRandomizer(threadData3);
    threadData3.vehicleNumber = 2;
    threadData3.speed=0.01;

    threadData4.vehicleNumber = 0;
    threadData4.speed=distribution(gen);

    threadData5.vehicleNumber = 1;
    threadData5.speed= distribution(gen);


    threadData6.vehicleNumber = 2;
    threadData6.speed=distribution(gen);

    // Create the threads for updating and drawing the objects
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;

    pthread_create(&thread1, nullptr, updateVerticalVehicle, reinterpret_cast<void*>(&threadData1));
    pthread_create(&thread2, nullptr, updateVerticalVehicle, reinterpret_cast<void*>(&threadData2));
    pthread_create(&thread3, nullptr, updateVerticalVehicle, reinterpret_cast<void*>(&threadData3));
    pthread_create(&thread4, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(&threadData4));
    pthread_create(&thread5, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(&threadData5));
    pthread_create(&thread6, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(&threadData6));
    pthread_create(&thread7, nullptr, reinterpret_cast<void *(*)(void *)>(horizontalVehiclesHandler), nullptr);

    // Register the displayScene function
    glutDisplayFunc(displayScene);

    // Enter GLUT event processing loop
    glutMainLoop();

    // Wait for all threads to finish before exiting
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    pthread_join(thread3, nullptr);
    pthread_join(thread4, nullptr);
    pthread_join(thread5, nullptr);
    pthread_join(thread6, nullptr);
    pthread_join(thread7, nullptr);


    return 0;
}
