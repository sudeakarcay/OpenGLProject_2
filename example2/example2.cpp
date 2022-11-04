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
#include <time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define GAME_DURATION 60 //the player can play the game 60 seconds
#define TURN_VALUE 0.471471471471

//state 
#define START_GAME 0
#define RUN_GAME 1
#define END_GAME 2

bool fish;

float distance;
float xP = 0, yP = 0; //player coordinate
float xF, yF; //fish coordinate
float xW = 0, yW = 0;  //weapon coordinate
int angle = 90, angle1;


int timer = GAME_DURATION; //displaying time

int circ = START_GAME;

int fixtimer = 1;
int xQ, yQ;
int computerPoint = 0;
int playerPoint = 0;

typedef struct {

    float x, y;

}point_t;


typedef struct {

    point_t position;
    float angle;
    bool active;

}fire_t;

typedef struct {

    int r, g, b;

}color_t;

typedef struct {

    int number1, number2;
    int locked[4];
    color_t colorOfQ[4];


}question_t;

question_t question;
fire_t fire = { 0,0,0,false };


int randomLocation;
int answerStatus = 0; //if it is false that means the answer is wrong
int isTargetLocked = 0; // to check if player locks target or not
int hitTarget = 0; // not hit 
int statusOfFire = 0; //to check if the fire is ready or not(false means not ready)
int isEnabled = 0;
int xcloud1, xcloud2;
int cloud1status = 0, cloud2status = 0;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
bool fish = false;
int  winWidth, winHeight; // current Window width and height

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

void vertex(point_t P, point_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

void cloud1()
{
    glColor3ub(xcloud1 + 191, xcloud1 + 62, xcloud1 + 255);
    circle(xcloud1 - 450, 200, 30);
    circle(xcloud1 - 400, 220, 40);
    circle(xcloud1 - 400, 200, 40);
    circle(xcloud1 - 350, 200, 30);
    //smile face on cloud
    glColor3f(0, 0, 0);
    circle(xcloud1 - 420, 230, 5);
    circle(xcloud1 - 380, 230, 5);
    glColor3ub(xcloud1 + 250, xcloud1 + 128, 114);
    circle(xcloud1 - 440, 210, 8);
    circle(xcloud1 - 360, 210, 8);




}

void cloud2()
{

    glColor3ub(xcloud2 + 255, xcloud2 + 20, xcloud2 + 147);
    circle(xcloud2 + 450, 180, 30);
    circle(xcloud2 + 400, 200, 40);
    circle(xcloud2 + 400, 180, 40);
    circle(xcloud2 + 350, 180, 30);
    //smile face on cloud
    glColor3f(0, 0, 0);
    circle(xcloud2 + 420, 210, 5);
    circle(xcloud2 + 380, 210, 5);
    glColor3ub(xcloud2 + 250, xcloud2 + 128, 114);
    circle(xcloud2 + 440, 190, 8);
    circle(xcloud2 + 360, 190, 6);
}

void drawQuestionArea()
{


    glColor3ub(240, 128, 128);
    glBegin(GL_QUADS);
    glVertex2f(-450, -350);
    glVertex2f(-450, -310);
    glVertex2f(450, -310);
    glVertex2f(450, -350);
    glEnd();



    glColor3f(0, 0, 0);
    vprint(-440, -330, GLUT_BITMAP_HELVETICA_18, "%d", question.number1);
    vprint(-415, -330, GLUT_BITMAP_HELVETICA_18, "/");
    vprint(-400, -330, GLUT_BITMAP_HELVETICA_18, "%d", question.number2);

    glColor3f(1, 1, 0);

    //answerpart 1
    glColor3f(question.colorOfQ[0].r, question.colorOfQ[0].g, question.colorOfQ[0].b);
    glBegin(GL_QUADS);
    glVertex2f(-260, -310);
    glVertex2f(-260, -340);
    glVertex2f(-210, -340);
    glVertex2f(-210, -310);
    glEnd();


    //answerpart 2
    glColor3f(question.colorOfQ[1].r, question.colorOfQ[1].g, question.colorOfQ[1].b);
    glBegin(GL_QUADS);
    glVertex2f(-160, -310);
    glVertex2f(-160, -340);
    glVertex2f(-110, -340);
    glVertex2f(-110, -310);
    glEnd();

    //answerpart 3
    glColor3f(question.colorOfQ[2].r, question.colorOfQ[2].g, question.colorOfQ[2].b);
    glBegin(GL_QUADS);
    glVertex2f(-60, -310);
    glVertex2f(-60, -340);
    glVertex2f(-10, -340);
    glVertex2f(-10, -310);
    glEnd();

    //answerpart 4
    glColor3f(question.colorOfQ[3].r, question.colorOfQ[3].g, question.colorOfQ[3].b);
    glBegin(GL_QUADS);
    glVertex2f(40, -310);
    glVertex2f(40, -340);
    glVertex2f(90, -340);
    glVertex2f(90, -310);
    glEnd();

    //answers
    glColor3f(1, 0, 0);
    vprint(-239, -335, GLUT_BITMAP_HELVETICA_18, "%d", question.locked[0]);
    glColor3f(1, 0, 0);
    vprint(-139, -335, GLUT_BITMAP_HELVETICA_18, "%d", question.locked[1]);
    glColor3f(1, 0, 0);
    vprint(-39, -335, GLUT_BITMAP_HELVETICA_18, "%d", question.locked[2]);
    glColor3f(1, 0, 0);
    vprint(61, -335, GLUT_BITMAP_HELVETICA_18, "%d", question.locked[3]);


}

void questionGenerated()
{
    int  resultOfQ;

    //randomizing the question
    question.number1 = rand() % 10 + 1;
    question.number2 = rand() % 10 + 1;


    resultOfQ = question.number1 / question.number2; //question
    randomLocation = rand() % 4; // choosing a location for true answer

    switch (randomLocation) //Inserting the numbers to location
    {

    case 0:

        question.locked[randomLocation] = resultOfQ;
        question.locked[1] = rand() % 81;
        question.locked[2] = rand() % 81;
        question.locked[3] = rand() % 81;
        break;
    case 1:
        question.locked[randomLocation] = resultOfQ;
        question.locked[0] = rand() % 81;
        question.locked[2] = rand() % 81;
        question.locked[3] = rand() % 81;

        break;
    case 2:
        question.locked[randomLocation] = resultOfQ;
        question.locked[0] = rand() % 81;
        question.locked[1] = rand() % 81;
        question.locked[3] = rand() % 81;

        break;
    case 3:

        question.locked[randomLocation] = resultOfQ;
        question.locked[0] = rand() % 81;
        question.locked[1] = rand() % 81;
        question.locked[2] = rand() % 81;
        break;
    }
}

void displaytimer() //timer display
{
    glColor3f(1, 1, 0);
    vprint(300, 320, GLUT_BITMAP_HELVETICA_18, "%d", timer);


}


void drawTarget()
{

    if (!fish)
    {

        srand(time(NULL));
        xF = rand() % 901 - 450;
        yF = rand() % 701 - 350;

        fish = true;

    }


    glBegin(GL_POLYGON);				// draw body
    glColor3ub(238 ,154, 0);
    glVertex2f(xF+40, yF+ 200);
    glVertex2f(xF+120, yF + 250);
    glVertex2f(xF + 320, yF + 200);
    glVertex2f(xF + 100, yF + 160);
    glEnd();
    glColor3ub(238 ,59 ,59);
    circle(xF+200,yF+200,6);
    


    glBegin(GL_POLYGON);			//draw tail
    glColor3ub(188 ,238 ,104);
    glVertex2f(xF + 320, yF + 200);
    glVertex2f(xF+ 350, yF + 240);
    glVertex2f(xF + 340, yF + 200);
    glVertex2f(xF + 350, yF + 160);
    glVertex2f(xF + 320, yF + 200);
    glEnd();

    glBegin(GL_POLYGON);		 //draw top Key
    glColor3ub(151, 255 ,255);
    glVertex2f(xF + 120, yF + 250);
    glVertex2f(xF + 140, yF + 270);
    glVertex2f(xF + 280, yF + 209);
    glVertex2f(xF + 120, yF + 250);
    glEnd();

    glBegin(GL_POLYGON);		 //draw buttom Key
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(xF + 100, yF + 160);
    glVertex2f(xF + 140, yF + 200);
    glVertex2f(xF + 120, yF + 164);
    glVertex2f(xF + 100, yF + 160);
    glEnd();



}

void randomizeTarget()
{

    srand(time(NULL));

    int xrandom = rand();
    int yrandom = rand();

    target.point = rand() % 5 + 1;

    target.colorOfTarget.r = rand() % 300;
    target.colorOfTarget.g = rand() % 300;
    target.colorOfTarget.b = rand() % 300;

    if (xrandom % 2 == 0)

        target.position.x = rand() % 400 * -1 - 20;



    else

        target.position.x = rand() % 400 + 20;


    if (yrandom % 2 == 0)

        target.position.y = rand() % 300 * -1 - 20;

    else


        target.position.y = rand() % 300 + 20;


    isEnabled = 1;


}

void drawPlayer(player_t player)
{

    glColor3ub(255, 147, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(player.position.x, player.position.y);
    glVertex2f(player.position.x + 50 * cos(player.angle * D2R), player.position.y + 50 * sin(player.angle * D2R));
    glEnd();
    glLineWidth(1);

    /*glBegin(GL_TRIANGLES);
    vertex({ 14,10 }, player.position, player.angle);
    vertex({ 22,20 }, player.position, player.angle);
    vertex({ 30,10 }, player.position, player.angle);
    glEnd();
    */
    glColor3ub(255, 127, 0);
    circle(player.position.x, player.position.y, player.radius + 5);

    glColor3f(0, 0, 0);
    vprint(player.position.x - 12, player.position.y - 5, GLUT_BITMAP_HELVETICA_18, "%.0f", player.angle);



}

void drawWeapon()
{
    glColor3f(0.6, 0.1, 0.1);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(xW + 30 * cos(angle * D2R), yW + 30 * sin(angle * D2R));
    glEnd();

    angle1 = atan2(yF, xF) / D2R;
    if (angle1 > 360)
        angle1 -= 360;
    if (angle1 < 0)
        angle1 += 360;


    if (angle > 360)
        angle -= 360;
    if (angle < 0)
        angle += 360;

    if ((angle1 - angle >= -180 && angle1 - angle < 0) || (angle1 - angle > 180 && angle1 - angle < 360))
        angle--;
    else if ((angle1 - angle <= 180 && angle1 - angle > 0) || (angle1 - angle < -180 && angle1 - angle > -360))
        angle++;
    else
    {
        fire.active = true;
    }

    vprint(-5, -15, GLUT_BITMAP_8_BY_13, "%d", angle);
}


void resetFire()
{
    //reseting the fire referencing tip of the barrel

    fire.position.x = (xP + 50) * cos(angle * D2R);
    fire.position.y = (yP + 50) * sin(angle * D2R);


}

void firing()
{
    glColor3f(1, 0, 0);
    circle(xF, yF, 5);

    distance = sqrtf(powf((xW - xF), 2) + powf((yW - yF), 2));

    if (distance != 0)
    {
        xW += 5 * (cos(angle * D2R));
        yW += 5 * (sin(angle * D2R));
    }

    else
    {
        fish = false;
    }
}


void drawbullet(fire_t fire){

    if (isTargetLocked == 0)
    {
        resetFire();

    }
    if (isTargetLocked == 1) {

        glColor3f(0, 1, 1);
        circle(fire.position.x, fire.position.y, 4);

    }

}

//void turnPlayer(float turn) //turn is TURN_VALUE 
//{
//
//    if (isTargetLocked == 0 && target.position.x < player.position.x)
//    {
//
//        if (player.position.y > 0)
//        {
//            player.angle -= turn;
//        }
//        else
//            player.angle += turn;
//    }
//    if (isTargetLocked == 0 && target.position.x > player.position.x)
//    {
//        if (player.position.y < 0)
//            player.angle += turn;
//        else
//        {
//            player.angle += turn;
//        }
//    }
//
//    if (player.angle >= 360)
//        player.angle -= 360;
//    if (player.angle < 0)
//        player.angle += 360;
//
//
//
//}

void drawSea()
{
    glColor3ub(72, 209, 204);
    glBegin(GL_QUADS);
    glVertex2f(-450, 30);
    glVertex2f(450, 30);
    glVertex2f(450, -350);
    glVertex2f(-450, -350);
    glEnd();


}

void drawSky()
{
    glColor3ub(135, 206, 235);
    glBegin(GL_QUADS);
    glVertex2f(-450, 30);
    glVertex2f(450, 30);
    glVertex2f(450, 350);
    glVertex2f(-450, 350);
    glEnd();



}

//void targett(target_t target)
//{
//
//    glColor3ub(target.colorOfTarget.r, target.colorOfTarget.g, target.colorOfTarget.b);
//    circle(target.position.x, target.position.y, 20);
//    glColor3f(1, 1, 1);
//    vprint(target.position.x - 5, target.position.y - 5, GLUT_BITMAP_HELVETICA_18, "%d", target.point);
//
//
//
//}


//void endOfTheGame()
//{
//
//    glColor3ub(175, 238, 238);
//    glBegin(GL_QUADS);
//    glVertex2f(-450, -350);
//    glVertex2f(-450, 350);
//    glVertex2f(450, 350);
//    glVertex2f(450, -350);
//    glEnd();
//
//
//    if (computerPoint > playerPoint)
//    {
//        glColor3f(1, 1, 0);
//        circle(200, 200, 100);
//        vprint(-400, 0, GLUT_BITMAP_HELVETICA_18, "Hello Loser!");
//        vprint(-400, -40, GLUT_BITMAP_HELVETICA_18, "Player score: %d ", playerPoint);
//        vprint(-400, -80, GLUT_BITMAP_HELVETICA_18, "Computer score: %d ", computerPoint);
//    }
//    else if (computerPoint < playerPoint)
//    {
//
//        vprint(-400, 0, GLUT_BITMAP_HELVETICA_18, "Hello Winner!");
//        vprint(-400, -40, GLUT_BITMAP_HELVETICA_18, "Player score: %d ", playerPoint);
//        vprint(-400, -80, GLUT_BITMAP_HELVETICA_18, "Computer score: %d ", computerPoint);
//    }
//    else if (computerPoint == playerPoint)
//    {
//        vprint(-400, 0, GLUT_BITMAP_HELVETICA_18, "At least you didn't lose!");
//        vprint(-400, -40, GLUT_BITMAP_HELVETICA_18, "Player score: %d ", playerPoint);
//        vprint(-400, -80, GLUT_BITMAP_HELVETICA_18, "Computer score: %d ", computerPoint);
//
//    }
//}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (circ == START_GAME)
    {


        glColor3ub(238, 122, 233);
        glBegin(GL_QUADS);
        glVertex2f(-450, 350);
        glVertex2f(-450, -350);
        glVertex2f(450, -350);
        glVertex2f(450, 350);
        glEnd();

        glColor3ub(255, 105, 180);
        glBegin(GL_QUADS);
        glVertex2f(-450, 350);
        glVertex2f(-450, 250);
        glVertex2f(450, 250);
        glVertex2f(450, 350);
        glEnd();


        glColor3f(1, 1, 0);
        vprint(-70, 300, GLUT_BITMAP_HELVETICA_18, "FIRING GAME");
        vprint(290, 300, GLUT_BITMAP_HELVETICA_18, "Sude Akarcay");
        vprint(290, 280, GLUT_BITMAP_HELVETICA_18, "22002065");

        glColor3ub(202, 255, 112);
        vprint(-150, 0, GLUT_BITMAP_HELVETICA_18, "LET'S FIRE! ARE YOU READY?");
        glColor3ub(205, 38, 38);
        vprint(-160, -50, GLUT_BITMAP_HELVETICA_18, "<Press spacebar to start the game>");




    }



    if (circ == RUN_GAME)
    {


        drawSea();

        drawSky();

        cloud1();
        cloud2();

        drawWeapon();

        //drawTarget(target);
        /*targett(target);*/

        drawQuestionArea();

        displaytimer();

        drawbullet(fire);

       /* if (circ == END_GAME)
            endOfTheGame();*/


    }



    glutSwapBuffers();
}

//void calculateHit(fire_t fire)
//{
//
//    if (((target.position.x - 20 <= fire.position.x) && (target.position.x + 20 >= fire.position.x) || (target.position.y - 20 <= fire.position.y) && (target.position.y + 20 >= fire.position.y)) && isEnabled == 1)
//    {
//        hitTarget = 1;
//
//    }
//
//
//}




//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ') //STARTING GAME
        circ = RUN_GAME;

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

    xQ = x - winWidth / 2;
    yQ = winHeight / 2 - y;

    if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= -250 && xQ <= -220) && (yQ >= -340 && yQ <= -310) && randomLocation == 0)
    {
        question.colorOfQ[0].r = 0;
        question.colorOfQ[0].g = 1;
        question.colorOfQ[0].b = 0;
        answerStatus = 1;

    }
    else if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= -150 && xQ <= -120) && (yQ >= -340 && yQ <= -310))
    {

        question.colorOfQ[1].r = 1;
        question.colorOfQ[1].g = 0;
        question.colorOfQ[1].b = 0;
        answerStatus = 0;
    }
    else if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= -50 && xQ <= -20) && (yQ >= -340 && yQ <= -310))
    {
        question.colorOfQ[2].r = 1;
        question.colorOfQ[2].g = 0;
        question.colorOfQ[2].b = 0;
        answerStatus = 0;


    }
    else if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= 50 && xQ <= 80) && (yQ >= -340 && yQ <= -310))
    {
        question.colorOfQ[3].r = 1;
        question.colorOfQ[3].g = 0;
        question.colorOfQ[3].b = 0;
        answerStatus = 0;
    }

    if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= -150 && xQ <= -120) && (yQ >= -340 && yQ <= -310) && randomLocation == 1)
    {
        question.colorOfQ[1].r = 0;
        question.colorOfQ[1].g = 1;
        question.colorOfQ[1].b = 0;
        answerStatus = 1;

    }

    if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= -50 && xQ <= -20) && (yQ >= -340 && yQ <= -310) && randomLocation == 2)
    {
        question.colorOfQ[2].r = 0;
        question.colorOfQ[2].g = 1;
        question.colorOfQ[2].b = 0;
        answerStatus = 1;

    }
    if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (xQ >= 50 && xQ <= 80) && (yQ >= -340 && yQ <= -310) && randomLocation == 3)
    {
        question.colorOfQ[3].r = 0;
        question.colorOfQ[3].g = 1;
        question.colorOfQ[3].b = 0;
        answerStatus = 1;
    }



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

    if ((xQ >= -250 && xQ <= -220) && (yQ >= -340 && yQ <= -310))
    {
        question.colorOfQ[0].r = 0;
        question.colorOfQ[0].g = 0;
        question.colorOfQ[0].b = 1;

    }
    else
    {
        question.colorOfQ[0].r = 1;
        question.colorOfQ[0].g = 1;
        question.colorOfQ[0].b = 0;
    }
    if ((xQ >= -150 && xQ <= -120) && (yQ >= -340 && yQ <= -310))
    {
        question.colorOfQ[1].r = 0;
        question.colorOfQ[1].g = 0;
        question.colorOfQ[1].b = 1;
    }
    else
    {
        question.colorOfQ[1].r = 1;
        question.colorOfQ[1].g = 1;
        question.colorOfQ[1].b = 0;
    }
    if ((xQ >= -50 && xQ <= -20) && (yQ >= -340 && yQ <= -310))
    {
        question.colorOfQ[2].r = 0;
        question.colorOfQ[2].g = 0;
        question.colorOfQ[2].b = 1;

    }
    else
    {
        question.colorOfQ[2].r = 1;
        question.colorOfQ[2].g = 1;
        question.colorOfQ[2].b = 0;
    }
    if ((xQ >= 50 && xQ <= 80) && (yQ >= -340 && yQ <= -310))
    {
        question.colorOfQ[3].r = 0;
        question.colorOfQ[3].g = 0;
        question.colorOfQ[3].b = 1;

    }
    else
    {
        question.colorOfQ[3].r = 1;
        question.colorOfQ[3].g = 1;
        question.colorOfQ[3].b = 0;
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    calculateHit(target, fire);

    if (circ == RUN_GAME)
    {

        //cloud movement 


        xcloud1 += 2;

        if (xcloud1 > 900)

            xcloud1 -= 900;



        xcloud2 -= 3;


        if (xcloud2 < -900)

            xcloud2 -= -900;




        player.angle++;

        if (isEnabled == 1)
            player.angle = player.angle;


        if (player.angle > 360)
            player.angle = 0;


        if (isEnabled == 0)
        {
            isEnabled = 1;
            randomizeTarget();
            questionGenerated();

        }
        findTarget(player, target);

        if (isTargetLocked == 0 && isEnabled == 1)
            turnPlayer(player, TURN_VALUE);

        if (hitTarget == 1)
        {
            computerPoint += target.point;
            isEnabled = 0;
            hitTarget = 0;
            isTargetLocked = 0;
        }
        if (answerStatus == 1)
        {

            if (target.position.y <= 340)
            {
                target.position.y += 5;
            }
            else
            {
                randomizeTarget();
                questionGenerated();
                hitTarget = 0;
                isTargetLocked = 0;
                answerStatus = 1;
                playerPoint += target.point;
            }

        }

        if (isTargetLocked == 1)
        {

            if (statusOfFire == 1)
            {

                resetFire();
                statusOfFire = 0;

            }

            fire.position.x += 10 * cos(player.angle * D2R);
            fire.position.y += 10 * sin(player.angle * D2R);







        }



        if (fixtimer % 60 == 0)
        {
            if (circ == RUN_GAME) {
                timer--;
            }
            if (timer == 0) {
                circ = END_GAME;
            }
        }

        fixtimer++;



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