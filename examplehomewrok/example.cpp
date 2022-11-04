/*********
CTIS164 - Template Source Program
----------
STUDENT : Ali Yaðýz Mumcu
SECTION : 02
HOMEWORK: 3
----------
PROBLEMS:
 1) In one click, choice does not stay greeen or red.
 2) After choosing the wrong choice, you can still pick right choice
 3) Barrel can be rotated from long angle time to time
 ----------
ADDITIONAL FEATURES:
End screen is added
Menu is added
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

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define DURATION 40 //seconds to play

#define START 0
#define RUN 1
#define END 2
#define TARGET_RADIUS 20
#define TURN_VALUE 0.53213123312321312312231312321312321321312 // This value is for rotation. Because of the precision of fractions during calculation of tangent 

int state = START;

int timeC = DURATION; // Global variable for displaying time

int timerFixer = 1;
int x2, y2;
int tempx, tempy;
int computer_p = 0; // computer points
int player_p = 0;

typedef struct {
	float x, y;
} point_t; // points for x and y

typedef struct {
	point_t pos;    // position of the gun
	float   angle;
	float   r;
} gun_t;

typedef struct {
	point_t pos;
	float angle;

} fire_t;

typedef struct {
	int r, g, b;
} color_t;

typedef struct {
	point_t center;
	color_t color;
	float radius;
	float speed;
	int point;
	int enabled = 0;
} target_t;



typedef struct {

	int num1, num2;
	int loc[4];
	color_t color_q[4];
}question_t;


//Initialization of target, gun and fire
target_t target;
gun_t p = { {0,0}, 45, 20 };
question_t question;
fire_t f;

int rand_loc;
int choicestatus = 0; //0: Wrong 1: Right
int targetlock = 0; //0: Not Locked 1: Locked
int isHit = 0; //0: Not Hit 1: Hit
int firestatus = 0; // 0: Ready to fire 1: not ready to fire






/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
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

void question_area()
{
	//QUESTION AREA
	glColor3f(0, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f(-450, -350);
	glVertex2f(-450, -300);
	glVertex2f(450, -300);
	glVertex2f(450, -350);
	glEnd();

	//Question
	glColor3f(0, 0, 0);
	vprint(-440, -330, GLUT_BITMAP_TIMES_ROMAN_10, "%d", question.num1);
	vprint(-420, -330, GLUT_BITMAP_TIMES_ROMAN_10, "*");
	vprint(-400, -330, GLUT_BITMAP_TIMES_ROMAN_10, "%d", question.num2);

	glColor3f(1, 1, 0);

	//choice 1
	glColor3f(question.color_q[0].r, question.color_q[0].g, question.color_q[0].b);
	glBegin(GL_QUADS);
	glVertex2f(-250, -310);
	glVertex2f(-250, -340);
	glVertex2f(-220, -340);
	glVertex2f(-220, -310);
	glEnd();


	//choice 2
	glColor3f(question.color_q[1].r, question.color_q[1].g, question.color_q[1].b);
	glBegin(GL_QUADS);
	glVertex2f(-150, -310);
	glVertex2f(-150, -340);
	glVertex2f(-120, -340);
	glVertex2f(-120, -310);
	glEnd();

	//choice 3
	glColor3f(question.color_q[2].r, question.color_q[2].g, question.color_q[2].b);
	glBegin(GL_QUADS);
	glVertex2f(-50, -310);
	glVertex2f(-50, -340);
	glVertex2f(-20, -340);
	glVertex2f(-20, -310);
	glEnd();

	//choice 4
	glColor3f(question.color_q[3].r, question.color_q[3].g, question.color_q[3].b);
	glBegin(GL_QUADS);
	glVertex2f(50, -310);
	glVertex2f(50, -340);
	glVertex2f(80, -340);
	glVertex2f(80, -310);
	glEnd();

	//choices display
	glColor3f(0, 0, 0);
	vprint(-239, -335, GLUT_BITMAP_TIMES_ROMAN_10 "%d", question.loc[0]);
	glColor3f(0, 0, 0);
	vprint(-139, -335, GLUT_BITMAP_TIMES_ROMAN_10, "%d", question.loc[1]);
	glColor3f(0, 0, 0);
	vprint(-39, -335, GLUT_BITMAP_TIMES_ROMAN_10, "%d", question.loc[2]);
	glColor3f(0, 0, 0);
	vprint(61, -335, GLUT_BITMAP_TIMES_ROMAN_10, "%d", question.loc[3]);


}

void end_screen()
{
	//End screen is extra
	// Display the win, lose or draw status from the points
	glColor3f(0, 0, 1);
	if (computer_p > player_p)
	{
		vprint(-400, 0, GLUT_BITMAP_HELVETICA_18, "You Lose!");
		vprint(-400, -40, GLUT_BITMAP_HELVETICA_18, "Your score: %d ", player_p);
		vprint(-400, -80, GLUT_BITMAP_HELVETICA_18, "Computer score: %d ", computer_p);
	}
	else if (computer_p < player_p)
	{

		vprint(-400, 0, GLUT_BITMAP_HELVETICA_18, "You Win!");
		vprint(-400, -40, GLUT_BITMAP_HELVETICA_18, "Your score: %d ", player_p);
		vprint(-400, -80, GLUT_BITMAP_HELVETICA_18, "Computer score: %d ", computer_p);
	}
	else if (computer_p == player_p)
	{
		vprint(-400, 0, GLUT_BITMAP_HELVETICA_18, "Draw!");
		vprint(-400, -40, GLUT_BITMAP_HELVETICA_18, "Your score: %d ", player_p);
		vprint(-400, -80, GLUT_BITMAP_HELVETICA_18, "Computer score: %d ", computer_p);

	}
}


void generate_question()
{
	int  result;

	//randomizing the question
	question.num1 = rand() % 10;
	question.num2 = rand() % 10;


	result = question.num1 * question.num2; //question
	rand_loc = rand() % 4; // choosing a location for true answer

	switch (rand_loc) //Inserting the numbers to location
	{

	case 0:

		question.loc[rand_loc] = result;
		question.loc[1] = rand() % 81;
		question.loc[2] = rand() % 81;
		question.loc[3] = rand() % 81;
		break;
	case 1:
		question.loc[rand_loc] = result;
		question.loc[0] = rand() % 81;
		question.loc[2] = rand() % 81;
		question.loc[3] = rand() % 81;

		break;
	case 2:
		question.loc[rand_loc] = result;
		question.loc[0] = rand() % 81;
		question.loc[1] = rand() % 81;
		question.loc[3] = rand() % 81;

		break;
	case 3:

		question.loc[rand_loc] = result;
		question.loc[0] = rand() % 81;
		question.loc[1] = rand() % 81;
		question.loc[2] = rand() % 81;
		break;
	}
}


void displaytimer() //timer display
{
	glColor3f(1, 1, 0);
	vprint(300, 320, GLUT_BITMAP_HELVETICA_18, "%d", timeC);


}

void menu() //Menu is extra
{
	glColor3f(1, 1, 0);
	vprint(-50, 0, GLUT_BITMAP_HELVETICA_18, "Angular Firing Game");
	vprint(-50, -40, GLUT_BITMAP_HELVETICA_18, "Press Spacebar to start");

}

void target_shape(target_t t)
{

	glColor3ub(t.color.r, t.color.g, t.color.b); //random colors
	circle(t.center.x, t.center.y, TARGET_RADIUS);
	glColor3f(0, 0, 0);
	vprint(t.center.x - 5, t.center.y - 5, GLUT_BITMAP_HELVETICA_18, "%d", target.point); //displaying target point

}
void spawn_target() 	//Randomizing target location
{



	srand(time(NULL));

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


void barrel_shape(gun_t tp)
{
	//barrel shape
	glColor3f(1, 1, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(tp.pos.x, tp.pos.y);
	glVertex2f(tp.pos.x + 50 * cos(tp.angle * D2R), tp.pos.y + 50 * sin(tp.angle * D2R)); 
	glEnd();
	glLineWidth(1);

	//static object
	glColor3f(1, 0, 0);
	circle(tp.pos.x, tp.pos.y, tp.r + 5);

	glColor3f(0, 0, 0);
	vprint(tp.pos.x - 12, tp.pos.y - 5, GLUT_BITMAP_HELVETICA_18, "%.0f", tp.angle); 




}

void lock_target(gun_t p, target_t tg)
{


	float a = (sin(p.angle * D2R) / cos(p.angle * D2R)); // tangent of angle
	float b = (tg.center.y / tg.center.x); 


	if ((a <= b + 0.1 && a >= b - 0.1) && (targetlock == 0)) //If the tangents are equal, lock to target
		targetlock = 1;

	
	if (p.pos.y + 50 * sin(p.angle * D2R) > 0 && tg.center.y < 0)
		targetlock = 0;
	if (p.pos.y + 50 * sin(p.angle * D2R) < 0 && tg.center.y > 0)
		targetlock = 0;


}

void drawFires(gun_t p, fire_t f) {


	if (targetlock == 0)
	{
		resetFire(); //reseting the value of fire in onTimer function

	}
	if (targetlock == 1) {

		glColor3f(0, 1, 1);
		circle(f.pos.x, f.pos.y, 4); //shape of fire

	}


}

void turnBarrel(gun_t* tp, float increase) {

	if (targetlock == 0 && target.center.x < tp->pos.x)
	{

		if (tp->pos.y > 0)
		{
			tp->angle -= increase;
		}
		else
			tp->angle += increase;
	}
	if (targetlock == 0 && target.center.x > tp->pos.x)
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
void display_points()
{
	//displaying scores  
	glColor3f(1, 1, 0);
	vprint(-180, 320, GLUT_BITMAP_HELVETICA_18, "COMPUTER: %d ", computer_p);
	vprint(-30, 320, GLUT_BITMAP_HELVETICA_18, "PLAYER: %d", player_p);


}
//
// To display onto window using OpenGL commands
//
void display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	if (state == START)
		menu(); //Displaying menu

	if (state == RUN)
	{
		barrel_shape(p); //gun

		target_shape(target); //Displaying target
		question_area(); //displaying question area

		displaytimer(); // timer

		drawFires(p, f); //fire
		display_points(); //scores
	}
	if (state == END)
		end_screen();

	glutSwapBuffers();
}

void calcHit(target_t 
, fire_t fir)
{
	//calculating hitbox for target
	if (((target_c.center.x - 15 <= fir.pos.x) && (target_c.center.x + 15 >= fir.pos.x) || (target_c.center.y - 15 <= fir.pos.y) && (target_c.center.y + 15 >= fir.pos.y)) && target.enabled == 1)
	{
		isHit = 1;

	}
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ') //STARTING GAME
		state = RUN;
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
	x2 = x - winWidth / 2;
	y2 = winHeight / 2 - y;

	//Displaying the colors for button red for false and green for true. This part has a problem

	// Write your codes here.
	if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= -250 && x2 <= -220) && (y2 >= -340 && y2 <= -310) && rand_loc == 0)
	{
		question.color_q[0].r = 0;
		question.color_q[0].g = 1;
		question.color_q[0].b = 0;
		choicestatus = 1;

	}
	else if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= -150 && x2 <= -120) && (y2 >= -340 && y2 <= -310))
	{

		question.color_q[1].r = 1;
		question.color_q[1].g = 0;
		question.color_q[1].b = 0;
		choicestatus = 0;
	}
	else if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= -50 && x2 <= -20) && (y2 >= -340 && y2 <= -310))
	{
		question.color_q[2].r = 1;
		question.color_q[2].g = 0;
		question.color_q[2].b = 0;
		choicestatus = 0;


	}
	else if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= 50 && x2 <= 80) && (y2 >= -340 && y2 <= -310))
	{
		question.color_q[3].r = 1;
		question.color_q[3].g = 0;
		question.color_q[3].b = 0;
		choicestatus = 0;
	}

	if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= -150 && x2 <= -120) && (y2 >= -340 && y2 <= -310) && rand_loc == 1)
	{
		question.color_q[1].r = 0;
		question.color_q[1].g = 1;
		question.color_q[1].b = 0;
		choicestatus = 1;

	}

	if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= -50 && x2 <= -20) && (y2 >= -340 && y2 <= -310) && rand_loc == 2)
	{
		question.color_q[2].r = 0;
		question.color_q[2].g = 1;
		question.color_q[2].b = 0;
		choicestatus = 1;

	}
	if ((GLUT_RIGHT_BUTTON && GLUT_KEY_DOWN) && (x2 >= 50 && x2 <= 80) && (y2 >= -340 && y2 <= -310) && rand_loc == 3)
	{
		question.color_q[3].r = 0;
		question.color_q[3].g = 1;
		question.color_q[3].b = 0;
		choicestatus = 1;
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
	// Write your codes here


	//Changing color to blue when mouse is on choice but not selected

	if ((x2 >= -250 && x2 <= -220) && (y2 >= -340 && y2 <= -310))
	{
		question.color_q[0].r = 0;
		question.color_q[0].g = 0;
		question.color_q[0].b = 1;

	}
	else
	{
		question.color_q[0].r = 1;
		question.color_q[0].g = 0;
		question.color_q[0].b = 0;
	}
	if ((x2 >= -150 && x2 <= -120) && (y2 >= -340 && y2 <= -310))
	{
		question.color_q[1].r = 0;
		question.color_q[1].g = 0;
		question.color_q[1].b = 1;
	}
	else
	{
		question.color_q[1].r = 1;
		question.color_q[1].g = 0;
		question.color_q[1].b = 0;
	}
	if ((x2 >= -50 && x2 <= -20) && (y2 >= -340 && y2 <= -310))
	{
		question.color_q[2].r = 0;
		question.color_q[2].g = 0;
		question.color_q[2].b = 1;

	}
	else
	{
		question.color_q[2].r = 1;
		question.color_q[2].g = 0;
		question.color_q[2].b = 0;
	}
	if ((x2 >= 50 && x2 <= 80) && (y2 >= -340 && y2 <= -310))
	{
		question.color_q[3].r = 0;
		question.color_q[3].g = 0;
		question.color_q[3].b = 1;

	}
	else
	{
		question.color_q[3].r = 1;
		question.color_q[3].g = 0;
		question.color_q[3].b = 0;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	// Write your codes here.

	calcHit(target, f);

	if (state == RUN)
	{
		if (target.enabled == 0) //checking if the target is enabled or not
		{
			target.enabled = 1; //enabling target
			spawn_target(); //spawning target
			generate_question(); //question generator

		}
		lock_target(p, target); //locking target

		if (targetlock == 0 && target.enabled == 1)
			turnBarrel(&p, TURN_VALUE); //turning barrel

		if (isHit == 1) // if the target is hit
		{
			computer_p += target.point;
			target.enabled = 0;
			isHit = 0;
			targetlock = 0;
		}
		if (choicestatus == 1) // if the choice is right
		{

			if (target.center.y <= 340) //checking if the target is end of the screen
			{
				target.center.y += 5; // moving the target to the end of screen
			}
			else
			{
				spawn_target();
				generate_question();
				isHit = 0;
				targetlock = 0;
				choicestatus = 1;
				player_p += target.point; //adding points to player
			}

		}

		if (targetlock == 1) //if target is locked 
		{

			if (firestatus == 1)
			{
				resetFire();
				firestatus = 0;
			}
			f.pos.x += 10 * cos(p.angle * D2R);
			f.pos.y += 10 * sin(p.angle * D2R);
		}



		if (timerFixer % 50 == 0) //Fixing timer for seconds related to TIMER_PERIOD 
		{
			if (state == RUN) {
				timeC--;
			}
			if (timeC <= 0) {
				state = END;
			}
		}

		timerFixer++;
	}
	//to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW3 : Angular Firing by Ali Yaðýz Mumcu");

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
