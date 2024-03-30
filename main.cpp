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
    pthread_mutex_t mutex; // Mutex for synchronizing access to thread data
};

// Global variables to hold data for each rectangle
ThreadData threadData1, threadData2, threadData3, threadData4, threadData5, threadData6;

bool stop = false;

int amountOfHorizontalThreadsActive = 0;
bool vehicleSpawned[3] = {true, true, true};
float startingPoints[6] = {-0.47, 0.77, -0.4, 0.7, -0.37, 0.67};
float path[3][4] = {{0.47, -0.77, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.37, -0.67, -0.37, 0.67}};
float path2[3][4] = {{0.47, -0.77, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.37, -0.67, -0.37, 0.67}};
float colors[6][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0 },{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };

// Mutex and condition variable for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

std::default_random_engine gen;
//std::default_random_engine gen2;
//std::default_random_engine gen3;
std::uniform_real_distribution<double> distribution(0.0001,0.04);

// Function to draw a simple object
void drawObject(double x, double y, int color) {
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Function to display the scene
void display() {
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
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(course1Color[0]-0.1, course1Color[1]-0.1, course1Color[2]);
    glVertex2f(-0.8, 0.5);
    glVertex2f(-1, 0.5);
    glVertex2f(-1, 0.3);
    glVertex2f(-0.8, 0.3);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(course1Color[0]-0.1, course1Color[1]-0.1, course1Color[2]);
    glVertex2f(-0.8, -0.5);
    glVertex2f(-1, -0.5);
    glVertex2f(-1, -0.3);
    glVertex2f(-0.8, -0.3);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(course2Color[0], course2Color[1], course2Color[2]);
    glVertex2f(-0.5, 0.8);
    glVertex2f(0.5, 0.8);
    glVertex2f(0.5, -0.8);
    glVertex2f(-0.5, -0.8);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(course1Color[0], course1Color[1], course1Color[2]);
    glVertex2f(-0.5, 0.3);
    glVertex2f(0.5, 0.3);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(course1Color[0], course1Color[1], course1Color[2]);
    glVertex2f(-0.5, -0.3);
    glVertex2f(0.5, -0.3);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    glVertex2f(-0.35, 0.3);
    glVertex2f(0.35, 0.3);
    glVertex2f(0.35, -0.3);
    glVertex2f(-0.35, -0.3);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    glVertex2f(-0.35, 0.65);
    glVertex2f(0.35, 0.65);
    glVertex2f(0.35, 0.5);
    glVertex2f(-0.35, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    glVertex2f(-0.35, -0.5);
    glVertex2f(0.35, -0.5);
    glVertex2f(0.35, -0.65);
    glVertex2f(-0.35, -0.65);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    glVertex2f(0.65, 0.3);
    glVertex2f(0.65, -0.3);
    glVertex2f(0.5, -0.3);
    glVertex2f(0.5, 0.3);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    glVertex2f(-0.65, 0.3);
    glVertex2f(-0.65, -0.3);
    glVertex2f(-0.5, -0.3);
    glVertex2f(-0.5, 0.3);
    glEnd();

    // Draw the first object
    drawObject(threadData1.objectPositionX, threadData1.objectPositionY, threadData1.vehicleNumber);

    // Draw the second object
    drawObject(threadData2.objectPositionX, threadData2.objectPositionY, threadData2.vehicleNumber);

    drawObject(threadData3.objectPositionX, threadData3.objectPositionY, threadData3.vehicleNumber);

    drawObject(threadData4.objectPositionX, threadData4.objectPositionY, threadData4.vehicleNumber);

    drawObject(threadData5.objectPositionX, threadData5.objectPositionY, threadData5.vehicleNumber);

    drawObject(threadData6.objectPositionX, threadData6.objectPositionY, threadData6.vehicleNumber);

    // Swap buffers to display the scene
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
        // Lock the mutex before accessing shared data
        pthread_mutex_lock(&(threadData->mutex));

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

        // Unlock the mutex
        pthread_mutex_unlock(&(threadData->mutex));
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
            // Lock the mutex before accessing shared data
            pthread_mutex_lock(&(threadData->mutex));

            while (x < path2[threadData->vehicleNumber][3] && !stop) {
                x += threadData->speed;
                usleep(10000); // Sleep for 10 milliseconds
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (y > path2[threadData->vehicleNumber][2] && !stop) {
                y -= threadData->speed;
                usleep(10000); // Sleep for 10 milliseconds
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            while (x > path2[threadData->vehicleNumber][1] && !stop) {
                x -= threadData->speed;
                usleep(10000); // Sleep for 10 milliseconds
                threadData->objectPositionX = x;
                threadData->objectPositionY = y;
            }
            if(i !=2) {
                while (y < path2[threadData->vehicleNumber][0] && !stop) {
                    y += threadData->speed;
                    usleep(10000); // Sleep for 10 milliseconds
                    threadData->objectPositionX = x;
                    threadData->objectPositionY = y;
                }
            }
            // Unlock the mutex

            pthread_mutex_unlock(&(threadData->mutex));
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
        // Lock the mutex before accessing shared data
        pthread_mutex_lock(&mutex);

        // Wait until there's an available slot for a new vehicle
            pthread_cond_wait(&cond, &mutex);

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
            std::cout << threadData->speed << std::endl;
            if (pthread_create(&thread, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(threadData)) != 0) {
                std::cerr << "Error: Thread creation failed!" << std::endl;
            }
            std::cout << "New thread!" << std::endl;
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

    // Initialize GLUT
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
    pthread_mutex_init(&(threadData1.mutex), nullptr);

    startingPointRandomizer(threadData2);
    threadData2.vehicleNumber = 1;
    threadData2.speed=0.01;
    pthread_mutex_init(&(threadData2.mutex), nullptr);

    startingPointRandomizer(threadData3);
    threadData3.vehicleNumber = 2;
    threadData3.speed=0.01;
    pthread_mutex_init(&(threadData3.mutex), nullptr);

    threadData4.objectPositionX = 0.0f;
    threadData4.objectPositionY = 0.0f;
    threadData4.vehicleNumber = 0;
    threadData4.speed=distribution(gen);
    std::cout << threadData4.speed;
    pthread_mutex_init(&(threadData4.mutex), nullptr);

    threadData5.objectPositionX = 0.0f;
    threadData5.objectPositionY = 0.0f;
    threadData5.vehicleNumber = 1;
    threadData5.speed= distribution(gen);
    std::cout << threadData5.speed;
    pthread_mutex_init(&(threadData5.mutex), nullptr);

    threadData6.objectPositionX = 0.0f;
    threadData6.objectPositionY = 0.0f;
    threadData6.vehicleNumber = 2;
    threadData6.speed=distribution(gen);
    std::cout << threadData5.speed;
    pthread_mutex_init(&(threadData6.mutex), nullptr);

    // Create the threads for updating and drawing the objects
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;

    if (pthread_create(&thread1, nullptr, updateVerticalVehicle, reinterpret_cast<void*>(&threadData1)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, nullptr, updateVerticalVehicle, reinterpret_cast<void*>(&threadData2)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread3, nullptr, updateVerticalVehicle, reinterpret_cast<void*>(&threadData3)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread4, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(&threadData4)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread5, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(&threadData5)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread6, nullptr, updateHorizontalVehicle, reinterpret_cast<void*>(&threadData6)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread7, nullptr, reinterpret_cast<void *(*)(void *)>(horizontalVehiclesHandler), nullptr)) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    // Register the display function
    glutDisplayFunc(display);

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

    // Destroy mutexes
    pthread_mutex_destroy(&(threadData1.mutex));
    pthread_mutex_destroy(&(threadData2.mutex));
    pthread_mutex_destroy(&(threadData3.mutex));
    pthread_mutex_destroy(&(threadData4.mutex));
    pthread_mutex_destroy(&(threadData5.mutex));
    pthread_mutex_destroy(&(threadData6.mutex));

    return 0;
}
