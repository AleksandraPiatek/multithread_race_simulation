#include "graphics.h"
#include <GL/glx.h>
#include "verticalVehicles.h"
#include <iostream>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <csignal>
#include <unistd.h>



verticalVehicles verticalVehicles;
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

void graphics::drawVehicle(int vehicleNumber, float oldX, float oldY, float x, float y) {
    if(oldX!=0 &oldY!=0) {
        glBegin(GL_QUADS);
        std::cout << "print";
        glColor3f(0.6,0.6,1);
        glVertex2f(oldX + 0.01, oldY + 0.01);
        glVertex2f(oldX + 0.01, oldY - 0.01);
        glVertex2f(oldX - 0.01, oldY - 0.01);
        glVertex2f(oldX - 0.01, oldY + 0.01);
        glEnd();
        glFlush();
    }

    glBegin(GL_QUADS);
    std::cout << "print";
    glColor3f(0,0,0);
    glVertex2f(x+0.01, y+0.01);
    glVertex2f(x+0.01, y-0.01);
    glVertex2f(x-0.01, y-0.01);
    glVertex2f(x-0.01, y+0.01);
    glEnd();
    glFlush();

}

void graphics::drawScene() {
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
        glFlush();

        //pthread_t thread;
        //pthread_create(&thread, NULL, reinterpret_cast<void *(*)(void *)>(movingVehicle), 0);

}

void graphics::initialize(int argc, char **argv, graphics graphics) {

//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_SINGLE);
//    glutInitWindowSize(1680, 1200);
//    glutInitWindowPosition(250, 100);
//    auto pGLctx = glutCreateWindow("");
//    glXMakeCurrent()
//    glutDisplayFunc(drawScene);
//    pthread_t thread;
//    pthread_create(&thread, NULL, reinterpret_cast<void *(*)(void *)>(movingVehicle), 0);
//
//    glutMainLoop();

    Display *display = XOpenDisplay(0);

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

    const char *extensions = glXQueryExtensionsString(display, DefaultScreen(display));
    std::cout << extensions << std::endl;

    static int visual_attribs[] =
            {
                    GLX_RENDER_TYPE, GLX_RGBA_BIT,
                    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                    GLX_DOUBLEBUFFER, true,
                    GLX_RED_SIZE, 1,
                    GLX_GREEN_SIZE, 1,
                    GLX_BLUE_SIZE, 1,
                    None
            };

    std::cout << "Getting framebuffer config" << std::endl;
    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
    if (!fbc)
    {
        std::cout << "Failed to retrieve a framebuffer config" << std::endl;
        return;
    }

    std::cout << "Getting XVisualInfo" << std::endl;
    XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[0]);

    XSetWindowAttributes swa;
    std::cout << "Creating colormap" << std::endl;
    swa.colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    std::cout << "Creating window" << std::endl;
    Window win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 1680, 1200, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
    if (!win)
    {
        std::cout << "Failed to create window." << std::endl;
        return;
    }

    std::cout << "Mapping window" << std::endl;
    XMapWindow(display, win);

    // Create an oldstyle context first, to get the correct function pointer for glXCreateContextAttribsARB
    GLXContext ctx_old = glXCreateContext(display, vi, 0, GL_TRUE);
    glXCreateContextAttribsARB =  (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx_old);

    if (glXCreateContextAttribsARB == NULL)
    {
        std::cout << "glXCreateContextAttribsARB entry point not found. Aborting." << std::endl;
        return;
    }

    static int context_attribs[] =
            {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                    None
            };

    std::cout << "Creating context" << std::endl;
    GLXContext ctx = glXCreateContextAttribsARB(display, fbc[0], NULL, true, context_attribs);
    if (!ctx)
    {
        std::cout << "Failed to create GL3 context." << std::endl;
        return;
    }

    std::cout << "Making context current" << std::endl;
    glXMakeCurrent(display, win, ctx);

    drawScene();
    //glClear (GL_COLOR_BUFFER_BIT);
    glXSwapBuffers (display, win);


    //sleep(10);

//    ctx = glXGetCurrentContext();
//    glXMakeCurrent(display, 0, 0);
//    glXDestroyContext(display, ctx);
}

void graphics::movingVehicle(int pathNumber) {
    float x = startingPoints[pathNumber], y = startingPoints[pathNumber+1];
    glXGetCurrentContext();
    drawVehicle(pathNumber,0,0, x, y);
    float oldY, oldX;
    //std::cout <<" printed";
    //while(true){
        while(x<path[pathNumber][0]){
            oldX = x;
            oldY = y;
            x+=speed;
            sleep(1);
            drawVehicle(pathNumber,oldX, oldY, x, y);
        }
        while(y>path[pathNumber][1]){
            oldX = x;
            oldY = y;
            y-=speed;
            sleep(1);
            drawVehicle(pathNumber,oldX, oldY, x, y);
        }
        while(x>path[pathNumber][2]){
            oldX = x;
            oldY = y;
            x-=speed;
            sleep(1);
            drawVehicle(pathNumber,oldX, oldY, x, y);
        }
        while(y<path[pathNumber][3]){
            oldX = x;
            oldY = y;
            y+=speed;
            sleep(1);
            drawVehicle(pathNumber,oldX, oldY, x, y);
        }
    //}
}
