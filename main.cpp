#include <GL/glut.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

// Structure to hold data needed by each drawing thread
struct ThreadData {
    float objectPositionX;
    float objectPositionY;
    int vehicleNumber;
};

// Global variables to hold data for each rectangle
ThreadData threadData1;
ThreadData threadData2;
ThreadData threadData3;
ThreadData threadData4;
ThreadData threadData5;
ThreadData threadData6;

bool stop = false;

int amountOfHorizontalThreadsActive = 0;
bool vehicleSpawned[3] = {false, false, false};
constexpr static const float speed = 0.01;
float startingPoints[6] = {-0.47, 0.77, -0.4, 0.7, -0.37, 0.67}; // do poprawienia, startuja za nisko?
float path[3][4] = {{0.45, -0.75, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.37, -0.67, -0.37, 0.67}};
float colors[6][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0 },{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };

// Mutex and condition variable for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Function to draw a simple object
void drawObject(float x, float y, int color) {
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

    drawObject(threadData4.objectPositionX, threadData5.objectPositionY, threadData5.vehicleNumber);

    drawObject(threadData4.objectPositionX, threadData6.objectPositionY, threadData6.vehicleNumber);

    // Swap buffers to display the scene
    glutSwapBuffers();

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    // Request a redraw
    glutPostRedisplay();
}

// Function to update object position for the vertical vehicle thread
void* updateVerticalVehicle(void * arg) {
    ThreadData* threadData = reinterpret_cast<ThreadData*>(arg);
    float x = startingPoints[2 * (threadData->vehicleNumber)], y = startingPoints[2 * (threadData->vehicleNumber + 1) + 1];
    while (!stop){
        while (x < path[threadData->vehicleNumber][0] && !stop) {
            x += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y > path[threadData->vehicleNumber][1] && !stop) {
            y -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (x > path[threadData->vehicleNumber][2] && !stop) {
            x -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y < path[threadData->vehicleNumber][3] && !stop) {
            y += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
}

void* updateHorizontalVehicle(void * arg){
    ThreadData* threadData = reinterpret_cast<ThreadData*>(arg);
    amountOfHorizontalThreadsActive++;
    float y = -startingPoints[2 * (threadData->vehicleNumber)], x = -startingPoints[2 * (threadData->vehicleNumber + 1) + 1];
    for(int i = 0; i < 3; i++) {
        while (x < path[threadData->vehicleNumber][3] && !stop) {
            x += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y > path[threadData->vehicleNumber][2] && !stop) {
            y -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (x > path[threadData->vehicleNumber][1] && !stop) {
            x -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
        while (y < path[threadData->vehicleNumber][0] && !stop) {
            y += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData->objectPositionX = x;
            threadData->objectPositionY = y;
        }
    }
    amountOfHorizontalThreadsActive--;

    // Signal the condition variable when a thread exits
    pthread_cond_signal(&cond);
}

void* horizontalVehiclesHandler(){

    while (!stop) {
        // Lock the mutex before accessing shared data
        pthread_mutex_lock(&mutex);

        // Wait until the condition is signaled or timeout occurs
        while (amountOfHorizontalThreadsActive >= 3 && !stop) {
            pthread_cond_wait(&cond, &mutex);
        }

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

            if (pthread_create(&thread, nullptr, reinterpret_cast<void*(*)(void*)>(updateHorizontalVehicle), reinterpret_cast<void*>(threadData)) != 0) {
                std::cerr << "Error: Thread creation failed!" << std::endl;
            }
        }

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }

    return nullptr;
}

void stopFunction(unsigned char key, int x, int y){
    if(key == ' '){
        stop=true;
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Thread Example");
    glutKeyboardFunc(stopFunction);

    // Initialize OpenGL
    initOpenGL();

    // Initialize data for the first object
    threadData1.objectPositionX = 0.0f;
    threadData1.objectPositionY = 0.0f;
    threadData1.vehicleNumber = 0;

    // Initialize data for the second object
    threadData2.objectPositionX = 0.0f;
    threadData2.objectPositionY = 0.0f;
    threadData2.vehicleNumber = 1;

    threadData3.objectPositionX = 0.0f;
    threadData3.objectPositionY = 0.0f;
    threadData3.vehicleNumber = 2;

    threadData4.objectPositionX = 0.0f;
    threadData4.objectPositionY = 0.0f;
    threadData4.vehicleNumber = 0;

    threadData5.objectPositionX = 0.0f;
    threadData5.objectPositionY = 0.0f;
    threadData5.vehicleNumber = 1;

    threadData6.objectPositionX = 0.0f;
    threadData6.objectPositionY = 0.0f;
    threadData6.vehicleNumber = 2;

    // Create the threads for updating and drawing the objects
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;

    if (pthread_create(&thread1, nullptr, reinterpret_cast<void *(*)(void *)>(updateVerticalVehicle), reinterpret_cast<void*>(&threadData1)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, nullptr, reinterpret_cast<void *(*)(void *)>(updateVerticalVehicle), reinterpret_cast<void*>(&threadData2)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread3, nullptr, reinterpret_cast<void *(*)(void *)>(updateVerticalVehicle), reinterpret_cast<void*>(&threadData3)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread4, nullptr, reinterpret_cast<void *(*)(void *)>(updateHorizontalVehicle), reinterpret_cast<void*>(&threadData4)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread5, nullptr, reinterpret_cast<void *(*)(void *)>(updateHorizontalVehicle), reinterpret_cast<void*>(&threadData5)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread6, nullptr, reinterpret_cast<void *(*)(void *)>(updateHorizontalVehicle), reinterpret_cast<void*>(&threadData6)) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread7, nullptr, reinterpret_cast<void *(*)(void *)>(horizontalVehiclesHandler), nullptr) != 0) {
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

    return 0;
}
