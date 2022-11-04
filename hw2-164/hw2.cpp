#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#include<time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON      1  // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define TURN_VALUE 0.53213123312321312312231312321312321321312 // This value is for rotation. Because of the precision of fractions during calculation of tangent 


bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool firestatus = false;

int isHit = 0;
typedef struct {
    double x, y;
} point_t;

typedef struct {
    point_t pos;
    double angle;
    double radius; // circular collision boundary
    bool hit;      // collision state
} player_t;



typedef struct {
    point_t pos;
    double angle;
    unsigned char r, g, b;
    double radius; // circular collision boundary
    bool hit;      // collision state
    int  enabled;
} entity_t;

typedef struct {
    point_t pos;
    float angle;
    bool active;
} fire_t;

#define MAX 20 //maximum number of icons

entity_t icons[MAX];
int count = 0;
fire_t fire = { { 0, 0 }, 45, 20 };
player_t p ;
entity_t target;
int targetlock = 0;

//
// to draw circle, center at (x,y)
//  radius r
//
void circle(int x, int y, int r)
{
    double angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 5)
    {
        angle = i * D2R;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
void circle_wire(int x, int y, int r)
{
    float angle;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 5)
    {
        angle = i * D2R;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
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

// Apply translate (move) and rotation transformations
void vertex(point_t P, point_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

//void player() {
//    p.pos.x = 30;
//    p.pos.y = 0;
//    glColor3ub(56, 15, 40);
//    circle(0, 0, 30);
//    glColor3ub(255, 0, 0);
//    glBegin(GL_LINES);
//    glVertex2f(0, 0);
//    glVertex2f(p.pos.x * cos(p.angle) - p.pos.y * sin(p.angle), p.pos.x * sin(p.angle) + p.pos.y * cos(p.angle));
//    glEnd();
//}

void calcHit(entity_t target_c, fire_t fir)
{
    //calculating hitbox for target
    if (((target_c.pos.x - 15 <= fir.pos.x) && (target_c.pos.x + 15 >= fir.pos.x) || (target_c.pos.y - 15 <= fir.pos.y) && (target_c.pos.y + 15 >= fir.pos.y)) && target.enabled == 1)
    {
        isHit = 1;
    }
}

void drawPlayer() {
    p.pos.x = 30;
    p.pos.y = 0;
    glColor3f(1, 1, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x, p.pos.y);
    glVertex2f(p.pos.x + 50 * cos(p.angle * D2R), p.pos.y + 50 * sin(p.angle * D2R));
    glEnd();
    glLineWidth(1);

    glColor3f(0.3, 0.3, 1.0);
    circle(p.pos.x, p.pos.y, p.radius);
    glColor3f(1.0, 0.3, 0.3);
    circle(p.pos.x, p.pos.y, p.radius - 4);

    glColor3f(1, 1, 1);
    vprint(p.pos.x - 12, p.pos.y - 5, GLUT_BITMAP_8_BY_13, "%.0f", p.angle);
}


//void drawPlayer(player_t p) {
//    double angle = p.angle * D2R;
//    glColor3f(0.5, 0.5, 1.0);
//    glBegin(GL_QUADS);
//    vertex({ 8, 25 }, p.pos, angle);
//    vertex({ 8, -25 }, p.pos, angle);
//    vertex({ -8, -20 }, p.pos, angle);
//    vertex({ -8, 20 }, p.pos, angle);
//    glEnd();
//    glColor3f(0.1, 0.1, 1.0);
//    glLineWidth(2);
//    glBegin(GL_LINE_LOOP);
//    vertex({ 8, 25 }, p.pos, angle);
//    vertex({ 8, -25 }, p.pos, angle);
//    vertex({ -8, -20 }, p.pos, angle);
//    vertex({ -8, 20 }, p.pos, angle);
//    glEnd();
//
//    glColor3f(0.8, 0.1, 0.1);
//    circle(p.pos.x, p.pos.y, 15);
//
//    glColor3f(0.1, 0.1, 1.0);
//    circle_wire(p.pos.x, p.pos.y, 15);
//    glLineWidth(1);
//
//    // collision boundary
//    if (p.hit) {
//        glColor3f(0, 1, 0);
//        circle_wire(p.pos.x, p.pos.y, p.radius);
//    }
//
//    glColor3f(1, 1, 0);
//    vprint(p.pos.x - 7, p.pos.y - 5, GLUT_BITMAP_HELVETICA_10,"%.0f", p.angle);
//}

void drawFish(entity_t t)  //target
{

    /*srand(time(NULL));*/ 

        glBegin(GL_POLYGON);				// draw body
        glColor3ub(255, 185, 15);
        glVertex2f(t.pos.x+40, t.pos.y+200);
        glVertex2f(t.pos.x + 120, t.pos.y + 250);
        glVertex2f(t.pos.x + 320, t.pos.y + 200);
        glVertex2f(t.pos.x + 100, t.pos.y + 160);
        glEnd();
        glColor3f(0, 0, 0);
        circle(t.pos.x + 120, t.pos.y + 230, t.radius+10);
        glColor3ub(147, 112, 219);
        circle(t.pos.x + 90, t.pos.y + 200, t.radius + 10);
        circle(t.pos.x + 160, t.pos.y + 190, t.radius + 12);
        circle(t.pos.x + 250, t.pos.y + 205, t.radius + 10);
        circle(t.pos.x + 220, t.pos.y + 204, t.radius + 7);
        circle(t.pos.x + 180, t.pos.y + 220, t.radius + 8);


        glBegin(GL_POLYGON);			//draw tail
        glColor3ub(255, 105, 180);
        glVertex2f(t.pos.x + 320, t.pos.y + 200);
        glVertex2f(t.pos.x + 350, t.pos.y + 240);
        glVertex2f(t.pos.x + 340, t.pos.y + 200);
        glVertex2f(t.pos.x + 350, t.pos.y + 160);
        glVertex2f(t.pos.x + 320, t.pos.y + 200);
        glEnd();

        glBegin(GL_POLYGON);		 //draw top Key
        glColor3ub(102, 205, 170);
        glVertex2f(t.pos.x + 120, t.pos.y + 250);
        glVertex2f(t.pos.x + 140, t.pos.y + 270);
        glVertex2f(t.pos.x + 280, t.pos.y + 209);
        glVertex2f(t.pos.x + 120, t.pos.y + 250);
        glEnd();

        glBegin(GL_POLYGON);		 //draw buttom Key
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(t.pos.x + 100, t.pos.y + 160);
        glVertex2f(t.pos.x + 140, t.pos.y + 200);
        glVertex2f(t.pos.x + 120, t.pos.y + 164);
        glVertex2f(t.pos.x + 100, t.pos.y + 160);
        glEnd();


    }



void resetFire()
{
    //reseting the fire referencing tip of the barrel

    fire.pos.x = (p.pos.x + 50) * cos(p.angle * D2R);
    fire.pos.y = (p.pos.y + 50) * sin(p.angle * D2R);


}

void drawFire(fire_t f) {

    if (targetlock == 0)
        resetFire();
    else {

        glColor3f(1, 1, 1);
        circle(f.pos.x, f.pos.y, 6);
    }
}
void lock_target(player_t f, entity_t p)
{
    //In this part, I have calculated locking to shape from tangent value (slope)

    float a = (sin(f.angle * D2R) / cos(f.angle * D2R)); // tangent of angle
    float b = (p.pos.y / p.pos.x); //tangent (slope) of the target referencing coordinate plane
    if ((a <= b + 0.1 && a >= b - 0.1) && (targetlock == 0)) //If the tangents are equal, lock to target
        targetlock = 1;
    //The problem with the tangent is it has same two value like tan(45)А and tan(225)А, if checks prevents this problem
    if (f.pos.y + 50 * sin(f.angle * D2R) > 0 && p.pos.y < 0){
        /*printf("test1");*/
            targetlock = 0; }

    if (f.pos.y + 50 * sin(f.angle * D2R) < 0 && p.pos.y > 0) {
        /*printf("test2");*/
        targetlock = 0;
      }

}
void target_shape(entity_t t)
{

    glColor3ub(t.r,t.g,t.b); //random colors
    circle(t.pos.x, t.pos.y, 20);
  /*  glColor3f(0, 0, 0);*/

}
//void drawIcon(entity_t e) {
//    glColor3ub(e.r, e.g, e.b);
//    glBegin(GL_TRIANGLES);
//    vertex({ 0, 20 }, e.pos, e.angle);
//    vertex({ -17.3, -10 }, e.pos, e.angle);
//    vertex({ 17.3, -10 }, e.pos, e.angle);
//    glEnd();
//    glColor3f(0, 0, 0);
//    glLineWidth(2);
//    glBegin(GL_LINE_LOOP);
//    vertex({ 0, 20 }, e.pos, e.angle);
//    vertex({ -17.3, -10 }, e.pos, e.angle);
//    vertex({ 17.3, -10 }, e.pos, e.angle);
//    glEnd();
//    glLineWidth(1);
//
//    // collision boundary
//    if (e.hit) {
//        glColor3f(0, 1, 0);
//        circle_wire(e.pos.x, e.pos.y, e.radius);
//    }
//}

//void drawAllIcons() {
//    for (int i = 0; i < count; i++) {
//        drawFish(icons[i]);
//    }
//}
// display text with variables.

//
// To display onto window using OpenGL commands
//
void display()
{
    //
    // clear window to black
    //
    target.pos.x = 100;
    target.pos.y = -200;
    
   /* drawFish(target);*/
   

    glClearColor(0.3, 0.3, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawPlayer();
   
    //target_shape(target);
    drawFish(target);

    /*target.enabled = 1;*/
    if (targetlock == 1)
        drawFire(fire);

    
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
   //glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    //glutPostRedisplay() ;
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
    //glutPostRedisplay() ;
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
    //glutPostRedisplay() ;
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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && count < MAX) {
        icons[count].pos = { x - winWidth / 2.0 , winHeight / 2.0 - y };
        icons[count].r = rand() % 256;
        icons[count].g = rand() % 256;
        icons[count].b = rand() % 256;
        icons[count].radius = 20;
        count++;
    }

    // to refresh the window it calls display() function
    //glutPostRedisplay() ; 
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    // To setup coordinate system
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    //glutPostRedisplay() ;
}

void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    //glutPostRedisplay() ;
}

// distance between two points.
double dist(point_t p1, point_t p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// if the player hits anyone of the icons, it returns true.
bool hitToIcons(player_t* p, entity_t icons[], int size) {
    for (int i = 0; i < size; i++) {
        double d = dist(p->pos, icons[i].pos);
        if (d <= (p->radius + icons[i].radius)) {
            icons[i].hit = true;
            p->hit = true;
        }
    }
    return p->hit;
}

void movePlayer(player_t* p, double speed) {
    double dx = speed * cos(p->angle * D2R);
    double dy = speed * sin(p->angle * D2R);
    point_t oldPos = p->pos;
    p->pos = { p->pos.x + dx, p->pos.y + dy };
    // if the player hits any icons, do not move
    if (hitToIcons(p, icons, count)) {
        p->pos = oldPos;
    }
    // check rectangular boundary
    if (p->pos.x > (300 - p->radius) || p->pos.x < (-300 + p->radius) ||
        p->pos.y >(300 - p->radius) || p->pos.y < (-300 + p->radius)) {
        p->pos = oldPos;
    }
}

void turnPlayer(player_t* p, double turn) {
    p->angle += turn;
    if (p->angle < 0) p->angle += 360;
    if (p->angle >= 360) p->angle -= 360;
}

// At the beginning of a frame, none of the objects are colliding.
void resetCollision() {
    p.hit = false;
    for (int i = 0; i < count; i++) {
        icons[i].hit = false;
    }
}



void turnBarrel(player_t* tp, float increase) {

    if (targetlock == 0 && target.pos.x < tp->pos.x)
    {

        if (tp->pos.y > 0)
        {
            tp->angle -= increase;
        }
        else
            tp->angle += increase;
    }
    if (targetlock == 0 && target.pos.x > tp->pos.x)
    {
        if (tp->pos.y < 0)
            tp->angle += increase;
        else
        {
            tp->angle += increase;
        }
    }

    if (tp->angle > 360) tp->angle -= 360;
    if (tp->angle < 0) tp->angle += 360;
}
//void spawn_target()
//{
//
//    srand(time(NULL));
//
//    //randomX and RandomY are for determining if the x or y is negative or positive
//    int randomX = rand();
//    int randomY = rand();
//
//    //randomizing the color of target
//    target.r= rand() % 300;
//    target.g = rand() % 300;
//    target.b = rand() % 300;
//
//    //Giving random location to target 
//    if (randomX % 2 == 0)
//        target.pos.x = rand() % 300 * -1 - 20;
//    else
//        target.pos.x = rand() % 400 + 20;
//    if (randomY % 2 == 0)
//        target.pos.y = rand() % 300 * -1 - 20;
//    else
//        target.pos.y = rand() % 300 + 20;
//
//    target.enabled = 1; // enabling target to be locked on
//
//
//
//
//
//}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    calcHit(target, fire);

    if (targetlock == 0)
        turnBarrel(&p, TURN_VALUE);

   // resetCollision();
    // turn the player clockwise direction

    /*if (target.enabled == 0)
    {

        target.enabled = 1;
        spawn_target();


    }*/

    lock_target(p, target);

    //if (targetlock == 0 && target.enabled == 1)
    //    turnBarrel(&p, TURN_VALUE); //turning barrel

    if (targetlock) //if target is locked 
    {
        if (firestatus == 1)
        {
            resetFire();
            firestatus = 0;
        }
        fire.pos.y += 10 * sin(p.angle * D2R);
        fire.pos.x += 10 * cos(p.angle * D2R);
      
    }
    if(isHit){
        printf("SUннннн");
        target.enabled = 0;
        isHit = 0;
        targetlock = 0;
        
    }

   
    
    // turn the player counter-clockwise direction
       // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Rotation");

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

    glutMainLoop();
}