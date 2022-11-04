/*********
   CTIS164 - Template Source Program
----------
STUDENT :
SECTION :
HOMEWORK:
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define START 0;
#define RUN 1;
#define END 2;
#define DURATION 60;
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebarKey=true, fireActive=true;
int  winWidth, winHeight;
int state = START; 
int timeCalculate = DURATION;
int totalPoint = 0; // current Window width and height

//
// to draw circle, center at (x,y)
// radius r
//

typedef struct {
    float x, y;
} point_t;

typedef struct {
    point_t pos;    // position of the object
    float   angle;  // view angle 
    float   r;
} player_t;

typedef struct {
    point_t pos;
    float angle;
    bool active;
} fire_t;

typedef struct {
    int r, g, b;
} color_t;

typedef struct {
    point_t center;
    color_t color;
    float radius;
    float speed;
} target_t;

player_t player = { {0,0}, 45, 20 };
fire_t  fire;
int fire_rate = 0;
target_t target;


int randLocation;
int targetLocked = 0;
int hit = 0;
int fireStatus = 0;

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


void drawSea()
{
    glColor3ub(72 ,209 ,204);
    glBegin(GL_QUADS);
    glVertex2f(-450,30);
    glVertex2f(450,30);
    glVertex2f(450, -350);
    glVertex2f(-450, -350);
    glEnd();


}

void drawSky()
{
    glColor3ub(135, 206 ,235);
    glBegin(GL_QUADS);
    glVertex2f(-450, 30);
    glVertex2f(450, 30);
    glVertex2f(450, 350);
    glVertex2f(-450, 350);
    glEnd();



}


void drawFish(target_t target)  //target
{

    glBegin(GL_POLYGON);				// draw body
    glColor3ub(target.color.r, target.color.g, target.color.b);
    glVertex2f(40, 200);
    glVertex2f(120, 250);
    glVertex2f(320, 200);
    glVertex2f(100, 160);
    glEnd();
    glColor3ub(target.color.r, target.color.g, target.color.b);
    circle(120, 230, 10);
    glColor3ub(target.color.r, target.color.g, target.color.b);
    circle(90, 200, 10);
    circle(160, 190, 12);
    circle(250, 205, 10);
    circle(220, 204, 7);
    circle(180, 220, 8);


    glBegin(GL_POLYGON);			//draw tail
    glColor3ub(255, 105, 180);
    glVertex2f(320, 200);
    glVertex2f(350, 240);
    glVertex2f(340, 200);
    glVertex2f(350, 160);
    glVertex2f(320, 200);
    glEnd();

    glBegin(GL_POLYGON);		 //draw top Key
    glColor3ub(target.color.r, target.color.g, target.color.b);
    glVertex2f(120, 250);
    glVertex2f(140, 270);
    glVertex2f(280, 209);
    glVertex2f(120, 250);
    glEnd();

    glBegin(GL_POLYGON);		 //draw buttom Key
    glColor3ub(target.color.r, target.color.g, target.color.b);
    glVertex2f(100, 160);
    glVertex2f(140, 200);
    glVertex2f(120, 164);
    glVertex2f(100, 160);
    glEnd();


}

void drawPlayer(player_t player)
{

    glColor3f(1, 1, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(player.pos.x, player.pos.y);
    glVertex2f(player.pos.x + 50 * cos(player.angle * D2R),
        player.pos.y + 50 * sin(player.angle * D2R));

    glEnd();
    glLineWidth(1);
    glColor3f(0.3, 0.3, 1);
    circle(player.pos.x, player.pos.y, player.r);
    glColor3f(1, 0.3, 0.3);
    circle(player.pos.x, player.pos.y, player.r - 4);

    glColor3f(0, 0, 0);
    vprint(player.pos.x - 12, player.pos.y - 5, GLUT_BITMAP_8_BY_13, "&0.f", player.angle);

}


void drawFire(fire_t fire)
{

    if (fireActive)
    {

        glColor3f(0,0,0);
        circle(fire.pos.x, fire.pos.y, 5);


    }


}

void Timer()
{

    glColor3f(1, 1, 0);
    vprint(300, 320, GLUT_BITMAP_HELVETICA_18, "%d", timeCalculate);


}

void menu() //Menu is extra
{
    glColor3f(1, 1, 0);
    vprint(-50, 0, GLUT_BITMAP_HELVETICA_18, "Fish Firing Game");
    vprint(-50, -40, GLUT_BITMAP_HELVETICA_18, "Press Spacebar to start");

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

    

   
    drawSea();

    drawPlayer(player);

    drawSky();

    drawFish(target);
    
    Timer();

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


    if (key == ' ')

        spacebarKey = false;


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


    if (spacebarKey && !fireActive)
    {

        fireActive = true;
        fire.pos = player.pos;
        fire.angle = player.angle;



    }

    if (fireActive)
    {

        fire.pos.x += 10 * cos(fire.angle * D2R);
        fire.pos.y += 10 * sin(fire.angle * D2R);

        if ((fire.pos.x > 250 || fire.pos.y < -250) || (fire.pos.y > 250 || fire.pos.y < -250))

            fireActive = false;



    }

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
    glutCreateWindow("Template File");

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