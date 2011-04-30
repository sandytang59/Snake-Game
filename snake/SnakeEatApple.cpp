#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>  
#include <string>
#include <time.h>
#include <math.h>

#include "glut.h"
#include "Shading.cpp"

#define MAX_BODY 20


class snake_body;
class snake_head;
class wall;
class apple;


/*************************************************************
** Snake Body Class declaration                               *
**************************************************************/

class snake_body{

protected:
	vec3 prev_position;
	vec3 current_position;
	float radius;
	rgb ka;
	rgb kd;
	rgb ks;
	float spe;

public:
	// Constructors
	snake_body();
	snake_body(vec3 posi, float r, rgb ka, rgb kd, rgb ks, float s);
	// Functions
	float getRadius();
	vec3 getCurrentPosition();
	vec3 getPreviousPosition();
	void setCurrentPosition(vec3 posi);
	void setPreviousPosition(vec3 posi);
	rgb getka();
	rgb getkd();
	rgb getks();
	float getspecular();
};


/*************************************************************
** Snake Head Class declaration                               *
**************************************************************/

class snake_head{

protected:
	vec3 prev_position;
	vec3 current_position;
	float radius;
	int numberOfBody;
	snake_body* body; // the maximum 20
	rgb ka;
	rgb kd;
	rgb ks;
	float spe;

public:
	// Constructors
	snake_head();
	snake_head(vec3 posi, float r, int numBody, rgb ka, rgb kd, rgb ks, float s);
	// Functions
	void move(float dy, float dx);	
	void undoMove();
	float getRadius();	
	int getNumOfBody();
	int getMaxBody();
	vec3 getCurrentPosition();
	vec3 getPreviousPosition();
	snake_body* getBody();
	void setCurrentPosition(vec3 posi);
	void setPreviousPosition(vec3 posi);
	void generateNewBody();
	bool ifHitWall(wall w);
	bool ifEatApple(apple a);
	bool ifHitBody();
	rgb getka();
	rgb getkd();
	rgb getks();
	float getspecular();
};


/*************************************************************
** Wall Class definition                                     *
**************************************************************/

class wall{

protected:
	vec3 position[4];   //the four vertices of the wall
	rgb ka;
	rgb kd;
	rgb ks;
	float spe;

public:
	//constructor
	wall();
	wall(vec3 posi1, vec3 posi2, vec3 posi3, vec3 posi4, rgb ka, rgb kd, rgb ks, float s);
	//function
	vec3* getCurrentPosition();	
	vec3 getPoint0();
	vec3 getPoint1();	
	vec3 getPoint2();	
	vec3 getPoint3();
	bool hitWall(wall w);
	bool ifHitSnake(snake_head s);
	rgb getka();
	rgb getkd();
	rgb getks();
	float getspecular();
};


/*************************************************************
** Apple Class declaration                                    *
**************************************************************/

class apple{

protected:
	vec3 position;
	float radius;			//radius of apple
	int score;			//score of this particular apple
	rgb ka;
	rgb kd;
	rgb ks;
	float spe;

public:
	//constructor
	apple();
	apple(float r, vec3 posi, int scor, rgb ka, rgb kd, rgb ks, float s);
	//functions
	vec3 getCurrentPosition();
	float getRadius();
	int getScore();
	bool ifHitWall(wall w);
	bool ifHitSnake(snake_head s);
	rgb getka();
	rgb getkd();
	rgb getks();
	float getspecular();
};


/*************************************************************
** Snake Body Class definition                               *
**************************************************************/

//default constructor
inline snake_body::snake_body(){}

//explicit constructor
inline snake_body::snake_body(vec3 posi, float r, rgb a, rgb d, rgb s, float sp){
	prev_position = NULL;
	current_position = posi;
	radius = r;
	ka = a;
	kd = d;
	ks = s;
	spe = sp;
}

//get the radius
inline float snake_body::getRadius(){
	return radius;
}

//get the current position
inline vec3 snake_body::getCurrentPosition(){
	return current_position;
}

//get the previous position
inline vec3 snake_body::getPreviousPosition(){
	return prev_position;
}

//set the current position
inline void snake_body::setCurrentPosition(vec3 posi){
	current_position = posi;
}

//set the previous position
inline void snake_body::setPreviousPosition(vec3 posi){
	prev_position = posi;
}

inline rgb snake_body::getka(){
	return ka;
}

inline rgb snake_body::getkd(){
	return kd;
}

inline rgb snake_body::getks(){
	return ks;
}

inline float snake_body::getspecular(){
	return spe;
}

/*************************************************************
** Snake Head Class definition                               *
**************************************************************/

//default constructor
inline snake_head::snake_head(){}

//explicit constructor
// Notes that the initial snake should have ONE head and TWO body.
inline snake_head::snake_head(vec3 posi, float r, int numBody, rgb a, rgb d, rgb s, float sp){
	prev_position = NULL;
	current_position = posi;
	radius = r;
	ka = a;
	kd = d;
	ks = s;
	spe = sp;


	//malloc spaces for array of bodies with max of maxBody
	body = (snake_body*)malloc(MAX_BODY * sizeof(snake_body));

	for(int i = 0; i < MAX_BODY; i++){
		body[i] = snake_body();
	}

	//initial two bodies
	vec3 body_posi_one = vec3(posi[0], (posi[1] - 2*r), posi[2]);
	body[0] = snake_body(body_posi_one, r, ka, kd, ks, spe);
	vec3 body_posi_two = vec3(posi[0], (posi[1] - 4*r), posi[2]);
	body[1] = snake_body(body_posi_two, r, ka, kd, ks, spe);

	//if(body[0].getka().r == body[1].getka().r && body[0].getka().g == body[1].getka().g && body[0].getka().b == body[1].getka().b){
	//	cout<<" body0.ka == body1.ka"<<endl;
	//}
	//else{
	//	cout<<"body0.ka : "<<body[0].getka().r<<" "<<body[0].getka().g<<" "<<body[0].getka().b<<endl;
	//	cout<<"body1.ka : "<<body[1].getka().r<<" "<<body[1].getka().g<<" "<<body[1].getka().b<<endl;
	//}

	//if(body[0].getkd().r == body[1].getkd().r && body[0].getkd().g == body[1].getkd().g && body[0].getkd().b == body[1].getkd().b){
	//	cout<<" body0.kd == body1.kd"<<endl;
	//}
	//if(body[0].getks().r == body[1].getks().r && body[0].getks().g == body[1].getks().g && body[0].getks().b == body[1].getks().b){
	//	cout<<" body0.ks == body1.ks"<<endl;
	//}
	//if(body[0].getspecular() == body[1].getspecular()){
	//	cout<<" body0.specular == body1.specular"<<endl;
	//}

	numberOfBody = 2;
}

// the move function for the whole snake
inline void snake_head::move(float dy, float dx){
	if(dx == 0 && dy == 0){
		return;
	}

	//update the snake head position
	vec3 pp = getCurrentPosition();
	setPreviousPosition(pp);
	vec3 cp = vec3(pp[0] + dx, pp[1] + dy, pp[2]);
	setCurrentPosition(cp);

	//update the first body position
	pp = body[0].getCurrentPosition();
	body[0].setPreviousPosition(pp);
	cp = getPreviousPosition();
	body[0].setCurrentPosition(cp);

	//update the rest of body position if exist
	for(int i = 1; i < numberOfBody; i++){
		vec3 old_cp = body[i].getCurrentPosition();
		body[i].setPreviousPosition(old_cp);
		vec3 new_cp = body[i-1].getPreviousPosition();
		body[i].setCurrentPosition(new_cp);
	}

}

//undo the move. Called when the game is over and hold the last picture.
inline void snake_head::undoMove(){
	vec3 temp;
	current_position = prev_position;
	for(int j = 0; j < numberOfBody ; j++){
		temp = body[j].getPreviousPosition();
		body[j].setCurrentPosition(temp);
	}
}

//get body array
inline snake_body* snake_head::getBody(){
	return body;
}

//get the radiius
inline float snake_head::getRadius(){
	return radius;
}

//get the number of body
inline int snake_head::getNumOfBody(){
	return numberOfBody;
}

inline int snake_head::getMaxBody(){
	return MAX_BODY;
}

//get the current position
inline vec3 snake_head::getCurrentPosition(){
	return current_position;
}

//get the previous position
inline vec3 snake_head::getPreviousPosition(){
	return prev_position;
}

//set the current position
inline void snake_head::setCurrentPosition(vec3 posi){
	current_position = posi;
}

//set the previous position
inline void snake_head::setPreviousPosition(vec3 posi){
	prev_position = posi;
}

//generate a new body to the end of the snake
inline void snake_head::generateNewBody(){
	//check if the numberOfBody reaches the MAX_BODY
	if(numberOfBody >= MAX_BODY){
		return;
	}

	//get the previous position of the last body and assign to the current position of the new body.
	vec3 posi = body[numberOfBody-1].getPreviousPosition();
	body[numberOfBody] = snake_body(posi,radius, ka, kd, ks, spe);
	//increase numberOfBody by 1
	numberOfBody++;
}

//check if the snake head hits the wall w.
inline bool snake_head::ifHitWall(wall w){
	vec3* posi = w.getCurrentPosition();
	if(current_position[0] > posi[0][0] && current_position[0] < posi[3][0]){
		if(current_position[1] > posi[0][1] && current_position[1] < posi[1][1]){
			return true;
		}
	}

	return false;
}

//check if the snake head eats the apple
inline bool snake_head::ifEatApple(apple a){
	if(current_position == a.getCurrentPosition()){
		return true;
	}
	return false;
}

//check if the snake head hits its own body
inline bool snake_head::ifHitBody(){
	for(int i = 0; i < numberOfBody; i++){
		if(current_position == body[i].getCurrentPosition()){
			cout << "posi1 " << current_position << "body position " << body[i].getCurrentPosition() << "i " << i << endl;
			return true;
		}
	}
	return false;
}


inline rgb snake_head::getka(){
	return ka;
}

inline rgb snake_head::getkd(){
	return kd;
}

inline rgb snake_head::getks(){
	return ks;
}

inline float snake_head::getspecular(){
	return spe;
}

/*************************************************************
** Wall Class definition                                     *
**************************************************************/

//default constructor
inline wall::wall(){}

//default constructor
inline wall::wall(vec3 posi1, vec3 posi2, vec3 posi3, vec3 posi4, rgb a, rgb d, rgb s, float sp){
	position[0] = posi1;
	position[1] = posi2;
	position[2] = posi3;
	position[3] = posi4;
	ka = a;
	kd = d;
	ks = s;
	spe = sp;
}

//get the current position
inline vec3* wall::getCurrentPosition(){
	return position;
}

//get the first point, position[0]
inline vec3 wall::getPoint0(){
	return position[0];
}

//get the second point, position[1]
inline vec3 wall::getPoint1(){
	return position[1];
}

//get the third point, position[2]
inline vec3 wall::getPoint2(){
	return position[2];
}

//get the fourth point, position[3]
inline vec3 wall::getPoint3(){
	return position[3];
}

//check if the wall overlaps the other wall w.
inline bool wall::hitWall(wall w){
	//it doesn't hit the wall only when the four points are on the same side of the wall
	//when all four points are on the left side of the wall
	if(position[0][0] >= w.position[0][0] && position[0][0] >= w.position[1][0]	&& position[0][0] >= w.position[2][0] && position[0][0] >= w.position[3][0]){
		return false;
	}
	//when all four points are on the right side of the wall
	if(position[3][0] <= w.position[0][0] && position[3][0] <= w.position[1][0]	&& position[3][0] <= w.position[2][0] && position[3][0] <= w.position[3][0]){
		return false;
	}
	//when all four points are on the down side of the wall
	if(position[0][1] >= w.position[0][1] && position[0][1] >= w.position[1][1] && position[0][1] >= w.position[2][1] && position[0][1] >= w.position[3][1]){
		return false;
	}
	//when all four points are on the up side of the wall
	if(position[1][1] <= w.position[0][1] && position[1][1] <= w.position[1][1] && position[1][1] <= w.position[2][1] && position[1][1] <= w.position[3][1]){
		return false;
	}

	//else return true
	return true;
}

//check if the wall overlaps the other wall w.
inline bool wall::ifHitSnake(snake_head s){
	//check if the head is inside the wall
	vec3 posi = s.getCurrentPosition();
	if(posi[0] > position[0][0] && posi[0] < position[3][0]){
		return true;
	}
	if(posi[1] > position[0][1] && posi[1] < position[1][1]){
		return true;
	}

	//check if one of the bodies is inside the wall
	snake_body* body;
	body = s.getBody();
	for(int i = 0; i < s.getNumOfBody() ; i++){
		posi = body[i].getCurrentPosition();
		if(posi[0] > position[0][0] && posi[0] < position[3][0]){
			return true;
		}
		if(posi[1] > position[0][1] && posi[1] < position[1][1]){
			return true;
		}
	}
	return false;
}

inline rgb wall::getka(){
	return ka;
}

inline rgb wall::getkd(){
	return kd;
}

inline rgb wall::getks(){
	return ks;
}

inline float wall::getspecular(){
	return spe;
}

/*************************************************************
** Apple Class definition                                    *
**************************************************************/

//the default constructor
inline apple::apple(){}

//the explicit constructor
inline apple::apple(float r, vec3 posi, int scor, rgb a, rgb d, rgb s, float sp){
	position = posi;
	radius = r;
	score = scor;
	ka = a;
	kd = d;
	ks = s;
	spe = sp;
}

//get the current position
inline vec3 apple::getCurrentPosition(){
	return position;	
}

//get the radius
inline float apple::getRadius(){
	return radius;
}

//get the score of the apple
inline int apple::getScore(){
	return score;
}

//check if the apple overlaps the wall w.
inline bool apple::ifHitWall(wall w){
	vec3 * posi = w.getCurrentPosition();
	if(position[0] > posi[0][0] && position[0] < posi[3][0]){
		return true;
	}
	if(position[1] > posi[0][1] && position[1] < posi[1][1]){
		return true;
	}

	return false;
}

//check if the apple overlaps the snake s.
inline bool apple::ifHitSnake(snake_head s){
	snake_body* body;
	body = s.getBody();
	if(s.getCurrentPosition() == position){
		return true;
	}
	for(int i = 0; i < s.getNumOfBody() ; i++){
		if(body[i].getCurrentPosition() == position){
			return true;
		}
	}
	return false;
}

inline rgb apple::getka(){
	return ka;
}

inline rgb apple::getkd(){
	return kd;
}

inline rgb apple::getks(){
	return ks;
}

inline float apple::getspecular(){
	return spe;
}