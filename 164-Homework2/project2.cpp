/*********
   CTIS164 - Template Source Program
----------
STUDENT : Sude Akarcay
SECTION : 004
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
**********************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


#define TURN 0.532132132132132132131  //buray� d�zelt

#define DURATION 60

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int time = DURATION; // variable to display the time
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}


typedef struct {
    float x, y;
} point_t;

typedef struct {
    point_t pos;    // position of the object
    float   angle; // view angle 
    float   r;
} player_t;

typedef struct {
    point_t pos;
    float angle;
    bool active;
} fire_t;

typedef struct {
    unsigned char r, g, b;
} color_t;

typedef struct {
    point_t center;
    color_t color;
    float radius;
    float speed;
    int point;
    int enabled = 0;
} target_t;


player_t player = { {0,0}, 45, 20 };
target_t target;
int count = 0;


void drawFish()  //target
{

    glBegin(GL_POLYGON);				// draw body
    glColor3ub(255 ,185 ,15);
    glVertex2f(40, 200);
    glVertex2f(120, 250);
    glVertex2f(320, 200);
    glVertex2f(100, 160);
    glEnd();
    glColor3f(0, 0, 0);
    circle(120, 230, 10);
    glColor3ub(147, 112, 219);
    circle(90, 200, 10);
    circle(160, 190, 12);
    circle(250, 205, 10);
    circle(220, 204, 7);
    circle(180, 220, 8);
    

    glBegin(GL_POLYGON);			//draw tail
    glColor3ub(255, 105 ,180);
    glVertex2f(320, 200);
    glVertex2f(350, 240);
    glVertex2f(340, 200);
    glVertex2f(350, 160);
    glVertex2f(320, 200);
    glEnd();

    glBegin(GL_POLYGON);		 //draw top Key
    glColor3ub(102, 205 ,170);
    glVertex2f(120, 250);
    glVertex2f(140, 270);
    glVertex2f(280, 209);
    glVertex2f(120, 250);
    glEnd();

    glBegin(GL_POLYGON);		 //draw buttom Key
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(100, 160);
    glVertex2f(140, 200);
    glVertex2f(120, 164);
    glVertex2f(100, 160);
    glEnd();


}

void target_shape(target_t t)
{

    glColor3ub(t.color.r, t.color.g, t.color.b); //random colors
    circle(t.center.x, t.center.y, 20);
    glColor3f(0, 0, 0);
    /* vprint(t.center.x - 5, t.center.y - 5, GLUT_BITMAP_HELVETICA_18, "%d", target.point);*/ //displaying target point
}

void spawn_target() 	//Randomizing target location
{



    /*srand(time(NULL));*/

    //randomX and RandomY are for determining if the x or y is negative or positive
    int randomX = rand();
    int randomY = rand();

    target.point = rand() % 5 + 1; //giving a random score to target between 1 and 5  

    //randomizing the color of target
    target.color.r = rand() % 300;
    target.color.g = rand() % 300;
    target.color.b = rand() % 300;

    //Giving random location to target 
    if (randomX % 2 == 0)
        target.center.x = rand() % 400 * -1 - 20;
    else
        target.center.x = rand() % 400 + 20;
    if (randomY % 2 == 0)
        target.center.y = rand() % 300 * -1 - 20;
    else
        target.center.y = rand() % 300 + 20;

    target.enabled = 1; // enabling target to be locked on

}

void drawPlayer(player_t tp) {
    glColor3f(1, 1, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(tp.pos.x, tp.pos.y);
    glVertex2f(tp.pos.x + 50 * cos(tp.angle * D2R), tp.pos.y + 50 * sin(tp.angle * D2R));
    glEnd();
    glLineWidth(1);

    glColor3f(0.3, 0.3, 1.0);
    circle(tp.pos.x, tp.pos.y, tp.r);
    glColor3f(1.0, 0.3, 0.3);
    circle(tp.pos.x, tp.pos.y, tp.r - 4);

    glColor3f(1, 1, 1);
    vprint(tp.pos.x - 12, tp.pos.y - 5, GLUT_BITMAP_8_BY_13, "%.0f", tp.angle);
}

void displaytimer() //timer display
{
    glColor3f(1, 1, 0);
    vprint(300, 320, GLUT_BITMAP_HELVETICA_18, "%d", time);


}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    target_shape(target);
   

  /*  drawFish();*/

    displaytimer();


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    spawn_target();


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Homework 3: Firing ");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}