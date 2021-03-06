#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>  
#include <string>
#include <time.h>
#include <math.h>

#include "glut.h"
#include "SnakeEatApple.cpp"

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define MAX_WALL 10
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define ACCURACY 15
#define MIN_SLEEP 100

using namespace std;


void generateWalls();
void generateApple();
bool snakeDie();
bool snakeEats();


//****************************************************
// Global Variableso
//****************************************************

//the snake object
snake_head snake;
//the level
int level;
//the score
int score;
//all the walls that exist in the game
wall * game_walls;
//number of walls
int numWall;
//the apple that exists in the game
apple game_apple;
//the changing x and y for the next run
float dx, dy;
//the unit change for the current game (according to the viewport size)
float stepUnit;
//the resolutoin of the game
int resolution;
//the sleep time
int sleep;
//a flag to see if the game still continue
bool gameOver;

Lights light;

//the viewport size
float viewport;

int dKey;


//****************************************************
// keyboard control setup
//****************************************************
void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 32)  //key == "space" 
		exit(0);
	if (key == 'w')
		dKey = UP;
	if (key == 'a')
		dKey = LEFT;
	if (key == 'd')
		dKey = RIGHT;
	if (key == 's')
		dKey = DOWN;
}

void specialKey (int key, int x, int y) {
	if (key == GLUT_KEY_LEFT){
		dKey = LEFT;
	}
	if (key == GLUT_KEY_RIGHT){
		dKey = RIGHT;
	}
	if (key == GLUT_KEY_UP){
		dKey = UP;
	}
	if (key == GLUT_KEY_DOWN){
		dKey = DOWN;
	}
}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
	//notice that the viewport will only be square.
	// Set the viewport
	glViewport ( 0 , 0 ,w, h ) ;
	// Set the projection transform
	glMatrixMode (GL_PROJECTION) ;
	glLoadIdentity ( );

	gluOrtho2D(0,viewport,0, viewport);
	//set the step unit
	stepUnit = w/resolution;

}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Clear to white, fully transparent	

	glViewport(0,0,viewport,viewport);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( ) ;
	gluOrtho2D(0,viewport,0, viewport);
	//glShadeModel(GL_SMOOTH);

	myReshape(viewport,viewport);
}

//***************************************************
// drawing helper functions
//***************************************************
void drawSphere(vec3 posi, rgb ka,rgb kd,rgb ks,float s){
	float radius = stepUnit/2.0;
	float r2 = radius * radius;

	float x_min = posi[0] - radius;
	float x_max = posi[0] + radius;
	float y_min = posi[1] - radius;
	float y_max = posi[1] + radius;
	float center_x = posi[0];
	float center_y = posi[1];

	float step = stepUnit/ACCURACY;

	glBegin(GL_POINTS);
	for(float x = x_min; x <= x_max; x += 1.0){
		for(float y = y_min ; y <= y_max ; y += 1.0){
			float tempX = x - center_x;
			float tempY = y - center_y;
			float temp = tempX * tempX + tempY * tempY;
			if(temp < r2){

				float z = sqrt ( r2 - tempX * tempX - tempY * tempY );
				vec3 point = vec3(x, y, z);
				vec3 center = vec3(center_x, center_y, 0);

				rgb color = light.getFinalColor_pointLight(point, center, ka, kd, ks, s);
				glColor3f(color.r, color.g, color.b);
				glVertex2f(x, y);
			}
		}
	}
	glEnd();

}

void drawSquare(vec3 p0, vec3 p1, vec3 p2, vec3 p3, rgb color){
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POLYGON);
		
	glVertex3f(p0[0], p0[1], 0);
	glVertex3f(p1[0], p1[1], 0);
	glVertex3f(p2[0], p2[1], 0);
	glVertex3f(p3[0], p3[1], 0);
	
	glEnd();
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {
	//draw first...

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ; //Clear Z−Buffer
	// Set the camera orientation :
	glMatrixMode (GL_MODELVIEW) ;
	glLoadIdentity ( ) ;

	//draw snake
	
	drawSphere(snake.getCurrentPosition(), snake.getka(), snake.getkd(), snake.getks(), snake.getspecular());
	
	//cout<<"GameOver is "<<gameOver<<endl;

	

	snake_body* body = snake.getBody();
	for(int m = 0; m < snake.getNumOfBody() ; m++){
		vec3 cp = body[m].getCurrentPosition();
		drawSphere(cp, body[m].getka(), body[m].getkd(), body[m].getks(), body[m].getspecular());
	}

	//draw apple

	drawSphere(game_apple.getCurrentPosition(), game_apple.getka(), game_apple.getkd(), game_apple.getks(), game_apple.getspecular());

	//draw all walls
	
	rgb wallColor;
	wallColor.r = 0.0;
	wallColor.g = 1.0;
	wallColor.b = 0.0;

	for(int k = 0; k < numWall; k++){
		vec3 p0 = game_walls[k].getPoint0();
		vec3 p1 = game_walls[k].getPoint1();
		vec3 p2 = game_walls[k].getPoint2();
		vec3 p3 = game_walls[k].getPoint3();

		drawSquare(p0, p1, p2, p3, wallColor);
	}



	//check game
	if(!gameOver){

		if(dKey == UP){
			dx = 0;
			dy = stepUnit;
		}
		else if(dKey == DOWN){
			dx = 0;
			dy = -stepUnit;
		}
		else if(dKey == LEFT){
			dx = -stepUnit;
			dy = 0;
		}
		else if(dKey == RIGHT){
			dx = stepUnit;
			dy = 0;
		}
		snake.move(dy, dx);

		if(snakeDie()){
			snake.undoMove();
			gameOver = true;
		}
		else if(snakeEats()){
			//generate new body, new walls and new apple
			if(snake.getNumOfBody() <= snake.getMaxBody()){
				snake.generateNewBody();
			}
			if(numWall <= MAX_WALL){
				generateWalls();
			}
			score = score + game_apple.getScore();
			level++;
			if(sleep > MIN_SLEEP){
				sleep = sleep -50;
			}
			generateApple();
		}
	}
	
	glFlush();
	glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}



//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
	//nothing here for now
#ifdef _WIN32
	Sleep(sleep);						//give ~10ms back to OS (so as not to waste the CPU)
#endif
	glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}



//****************************************************
// check if a wall hits any other walls
//****************************************************
bool wallOverlap(wall w){
	//check the walls one by one.
	for(int i=0; i<numWall; i++){
		if(game_walls[i].hitWall(w)){
			return true;
		}
	}

	return false;
	
}

//****************************************************
// check if snake hits anything
//****************************************************
bool snakeEats(){
	return (snake.ifEatApple(game_apple));
}

bool snakeDie(){
	//There are three possibilities that the snake will die.
	//1. snake hits its own body
	if(snake.ifHitBody()){
		return true;
	}
	//2. snake hist one of the walls
	for(int i = 0; i < numWall ; i++){
		if(snake.ifHitWall(game_walls[i])){
			return true;
		}
	}
	//3. snake hits the boundaries of the game screen
	vec3 cp = snake.getCurrentPosition();
	if(cp[0] <= 0.0 || cp[0] >= viewport || cp[1] <= 0.0 || cp[1] >= viewport){
		return true;
	}
	return false;
}


//****************************************************
// the generate wall
//****************************************************
void generateWalls(){
		//generate new length of this wall
		//let the range of wall lenght be from 2 to 4 
		int nLow=2, nHigh=4;
		int random_length = (rand() % (nHigh - nLow + 1)) + nLow;
		float length = random_length * stepUnit;

		//generate random direction
		nLow = 0;
		nHigh = 3;
		int direction = (rand() % (nHigh - nLow + 1)) + nLow;

		//generate random position
		nHigh = resolution-1;
		int random_1 = (rand() % (nHigh - nLow + 1)) + nLow;
		int random_2 = (rand() % (nHigh - nLow + 1)) + nLow;
		float random_x = random_1 * stepUnit;
		float random_y = random_2 * stepUnit;

		vec3 * position;
		position = (vec3*) malloc (4 * sizeof(vec3));

		//set the four points of this wall
		if(direction == UP){
			position[0] = vec3(random_x, random_y, 0);
			position[1] = vec3(random_x, random_y + length, 0);
			position[2] = vec3(random_x + stepUnit , random_y + length, 0);
			position[3] = vec3(random_x + stepUnit , random_y, 0);
		}
		else if(direction == RIGHT){
			position[0] = vec3(random_x, random_y, 0);
			position[1] = vec3(random_x, random_y + stepUnit, 0);
			position[2] = vec3(random_x + length , random_y + stepUnit, 0);
			position[3] = vec3(random_x + length , random_y, 0);
		}
		else if(direction == LEFT){
			position[0] = vec3(random_x - length , random_y, 0);
			position[1] = vec3(random_x - length , random_y + stepUnit, 0);
			position[2] = vec3(random_x, random_y + stepUnit, 0);
			position[3] = vec3(random_x, random_y, 0);
		}
		else if(direction == DOWN){
			position[0] = vec3(random_x, random_y - length, 0);
			position[1] = vec3(random_x, random_y, 0);
			position[2] = vec3(random_x + stepUnit , random_y, 0);
			position[3] = vec3(random_x + stepUnit , random_y - length, 0);
		}

		rgb wall_ka = {0.3, 0.4, 0.8};
		rgb wall_kd = {0.8, 0.1, 0.0};
		rgb wall_ks = {0.0, 0.7, 0.0};
		float wall_s = 4;

		game_walls[numWall] = wall(position[0], position[1], position[2], position[3], wall_ka, wall_kd, wall_ks, wall_s);
		//check whether there is wall overlapping.
		//if it happens then re-generate a wall.
		if(wallOverlap(game_walls[numWall])){
			generateWalls();
			return;
		}
		if(game_walls[numWall].ifHitSnake(snake)){
			generateWalls();
			return;
		}

		numWall++;

}

//****************************************************
// the generate apple
//****************************************************
void generateApple(){

	//generate random position
	int nLow = 0;
	int nHigh = resolution -1;
	int random_1 = (rand() % (nHigh - nLow + 1)) + nLow;
	int random_2 = (rand() % (nHigh - nLow + 1)) + nLow;
	float random_x = (random_1 * stepUnit) + (stepUnit/2);
	float random_y = (random_2 * stepUnit) + (stepUnit/2);

	//initialize the apple by using the constructor
	vec3 posi = vec3(random_x, random_y, 0);
	int s = level*10;

	//color of apple
	rgb appleKa = {1.0f, 0.0f, 0.0f};
	rgb appleKd = {0.0f, 0.0f, 1.0f};
	rgb appleKs = {0.0f, 1.0f, 0.0f};
	float appleS = 50;

	game_apple = apple(stepUnit/2, posi, s, appleKa, appleKd, appleKs, appleS);

	//check if the apple hits the snake
	if(game_apple.ifHitSnake(snake)){
		generateApple();
	}
	//check if the apple hits the walls
	for(int i = 0; i < numWall; i++){
		if(game_apple.ifHitWall(game_walls[i])){
			generateApple();
		}
	}

}

//****************************************************
// the game initialization
//****************************************************
void gameInitialize(){
	sleep = 500;
	
	dKey = 5;  //initialize it to a int that does not repersent any directions

	stepUnit = viewport/resolution;
	int middle = resolution / 2;
	float initial_xy = middle * stepUnit + stepUnit/2;
	vec3 initialize_posi = vec3(initial_xy, initial_xy, 0);

	rgb snakeKa = {0.0f, 0.0f, 1.0f};
	rgb snakeKd = {0.2f, 0.1f, 0.0f};
	rgb snakeKs = {0.0f, 0.0f, 0.1f};
	float snakeS = 256;

	snake = snake_head(initialize_posi, stepUnit/2 , 2, snakeKa, snakeKd, snakeKs, snakeS);

	game_walls = (wall*) malloc (MAX_WALL * sizeof(wall));
	for(int i = 0; i < MAX_WALL; i++){
		game_walls[i] = wall();
	}
	
	score = 0;
	level = 1;

	dy = 0;
	dx = 0;

	light = Lights();

	gameOver = false;
	
	generateApple();
}




//****************************************************
// the main function
//****************************************************
int main ( int argc , char** argv ) {

	//getting basic information from players
	/*cout<< "What do you want the snake color be?" << endl;
	cout<< "Make sure that the color is three float values between 0-1!" << endl;
	cout<< "Now, enter three float values between 0-1 as rgb values: " << endl;
	cin >> snakeColor.r;
	cin >> snakeColor.g;
	cin >> snakeColor.b;

	cout<< "How about the size of the screen?" << endl;
	cin >> viewport;
	cout<< "The resolution of the game?" << endl;
	cin >> resolution;
	*/
	

	viewport = 640;
	resolution = 20;

	glutInit (&argc , argv ) ;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Initalize theviewport size
	//viewport = 640;
  	//The size and position of the window
  	glutInitWindowSize(viewport, viewport);
  	glutInitWindowPosition(0, 0);
	
	glutCreateWindow ( " Snake Game " ) ;

	//initialize the game
	gameInitialize();

	initScene();							// quick function to set up scene
	glutDisplayFunc(myDisplay);				// function to run when its time to draw something
	glutReshapeFunc(myReshape);				// function to run when the window gets resized
	glutIdleFunc(myFrameMove);				// function to run when not handling any other task


	glutKeyboardFunc(processNormalKeys);    //normal key function
	glutSpecialFunc(specialKey);			//special key function
	
//	initLights ( ) ;
//	glEnable (GL_DEPTH_TEST) ;
	glutMainLoop ( ) ;						// infinite loop that will keep drawing and resizing and whatever else

	return 0;
}
