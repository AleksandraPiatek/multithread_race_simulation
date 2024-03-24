#include <GL/glut.h>
#include <pthread.h>
#include <unistd.h> // For sleep function
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

constexpr static const float speed = 0.01;
constexpr static const float startingPoints[6] = {-0.45, 0.75, -0.4, 0.7, -0.35, -0.65};
constexpr static const float path[3][4] = {{0.45, -0.75, -0.45, 0.75}, {0.4, -0.7, -0.4, 0.7}, {0.35, -0.65, -0.35, 0.65}};

// Function to draw a simple object
void drawObject(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f); // Red color
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
    drawObject(threadData1.objectPositionX, threadData1.objectPositionY);

    // Draw the second object
    drawObject(threadData2.objectPositionX, threadData2.objectPositionY);

    // Swap buffers to display the scene
    glutSwapBuffers();

    // Request a redraw
    glutPostRedisplay();
}


// Function to update object position for the first thread
void* updateObject1(void* arg) {
    float x = startingPoints[threadData1.vehicleNumber], y = startingPoints[threadData1.vehicleNumber+1];
    while(true) {
        while (x < path[0][0]) {
            x += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData1.objectPositionX = x;
            threadData1.objectPositionY = y;
        }
        while (y > path[0][1]) {
            y -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData1.objectPositionX = x;
            threadData1.objectPositionY = y;
        }
        while (x > path[0][2]) {
            x -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData1.objectPositionX = x;
            threadData1.objectPositionY = y;
        }
        while (y < path[0][3]) {
            y += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData1.objectPositionX = x;
            threadData1.objectPositionY = y;
        }
    }
}

// Function to update object position for the second thread
void* updateObject2(void* arg) {
    float x = startingPoints[2], y = startingPoints[3];
    while(true) {
        while (x < path[1][0]) {
            x += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData2.objectPositionX = x;
            threadData2.objectPositionY = y;
            std::cout << "Object 1 Position: (" << x << ", " << y << ")" << std::endl;
        }
        while (y > path[1][1]) {
            y -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData2.objectPositionX = x;
            threadData2.objectPositionY = y;
        }
        while (x > path[1][2]) {
            x -= speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData2.objectPositionX = x;
            threadData2.objectPositionY = y;
        }
        while (y < path[1][3]) {
            y += speed;
            usleep(10000); // Sleep for 10 milliseconds
            threadData2.objectPositionX = x;
            threadData2.objectPositionY = y;
        }
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Thread Example");

    // Initialize OpenGL
    initOpenGL();

    // Initialize data for the first object
    threadData1.objectPositionX = 0.0f;
    threadData1.objectPositionY = 0.0f;
    threadData1.vehicleNumber = 0;

    // Initialize data for the second object
    threadData2.objectPositionX = 0.0f;
    threadData2.objectPositionY = 0.0f;
    threadData2.vehicleNumber = 2;

    // Create the first thread for updating and drawing the first object
    pthread_t thread1;
    if (pthread_create(&thread1, nullptr, updateObject1, nullptr) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    // Create the second thread for updating and drawing the second object
    pthread_t thread2;
    if (pthread_create(&thread2, nullptr, updateObject2, nullptr) != 0) {
        std::cerr << "Error: Thread creation failed!" << std::endl;
        return EXIT_FAILURE;
    }

    // Register the display function
    glutDisplayFunc(display);

    // Enter GLUT event processing loop
    glutMainLoop();

    return 0;
}
