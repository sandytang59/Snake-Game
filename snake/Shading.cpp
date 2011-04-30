#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>  
#include <string>
#include <time.h>
#include <math.h>

#include "glut.h"
#include "Algebra3h.cpp"

typedef struct rgb
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
}rgb;


class Lights{

protected:

rgb pl_i;  //array of point lights' intensities
rgb dl_i;  //array of directional lights' intensities
vec3 pl;  //array of point lights' positions
vec3 dl;  //array of directional lights' positions

vec3 l; //light coming vector
vec3 r; //relection vector
vec3 v; //viewer vector
vec3 n; //normal vector

vec3 viewer_position; //the position of the viewer


public:

	//constructor
	Lights(){
		init_light();
	};
	
//****************************************************
// Phong shading for point lights
//***************************************************

	rgb getFinalColor_pointLight (vec3 p, vec3 center, rgb ka, rgb kd, rgb ks, float s)
	{

		//cout << "center " << center << " point " << p << endl;

		//final return color.
		rgb finalColor = {0, 0, 0};
		rgb ambientColor = {0, 0, 0};
		rgb diffuseColor = {0, 0, 0};
		rgb specularColor = {0, 0, 0};

		//compute the light unit vector
		l = pl - p;
		l = l/ (l.length());

		//cout << "l " << l<< endl;

		//compute the viewer unit vector
		v = viewer_position - p;
		v = v/ (v.length());

		//compute the normal vector
		vec3 temp = p - center;
		n = p / (temp.length());

		//cout << "normal " << n << endl;

		//compute the reflection light unit vector
		r = -(2 * ( l * n ) * n - l) ;
		
		//compute the ambient color
		ambientColor.r = ka.r * pl_i.r;
		ambientColor.g = ka.g * pl_i.g;
		ambientColor.b = ka.b * pl_i.b;

		//compute the diffuse color
		GLfloat ln = l * n;
		GLfloat scaler_d = max( ln , 0.0f);

		diffuseColor.r = kd.r * pl_i.r * scaler_d;
		diffuseColor.g = kd.g * pl_i.g * scaler_d;
		diffuseColor.b = kd.b * pl_i.b * scaler_d;

		//cout<<"kd.r :"<<kd.r<<" kd.g :"<<kd.g<<" kd.b :"<<kd.b<<endl;
		//cout<<"diffuse.r :"<<diffuseColor.r<<" diffuse.g :"<<diffuseColor.g<<" diffuse.b :"<<diffuseColor.b<<endl;

		//compute the specular color
		GLfloat rv = r * v;
		GLfloat scaler_s = max( rv , 0.0f );
		scaler_s = pow(scaler_s, s);

		specularColor.r = ks.r * pl_i.r * scaler_s;
		specularColor.g = ks.g * pl_i.g * scaler_s;
		specularColor.b = ks.b * pl_i.b * scaler_s;

		//cout<<"ks.r :"<<ks.r<<" ks.g :"<<ks.g<<" ks.b :"<<ks.b<<endl;
		//cout<<"specular.r :"<<specularColor.r<<" specular.g :"<<specularColor.g<<" specular.b :"<<specularColor.b<<endl;

		//compute final color
		finalColor.r = ambientColor.r + diffuseColor.r + specularColor.r;
		finalColor.g = ambientColor.g + diffuseColor.g + specularColor.g;
		finalColor.b = ambientColor.b + diffuseColor.b + specularColor.b;

		return finalColor;
	}


	
	rgb getFinalColor_dLight (vec3 p, vec3 center, rgb ka, rgb kd, rgb ks, float s)
	{
		//final return color.
		rgb finalColor = {0, 0, 0};
		rgb ambientColor = {0, 0, 0};
		rgb diffuseColor = {0, 0, 0};
		rgb specularColor = {0, 0, 0};

		//compute the light unit vector
		l = dl - center;
		vec3 temp = dl - p;
		l = l/ (temp.length());

		//compute the viewer unit vector
		v = viewer_position - p;
		v = v/(v.length());

		//compute the normal vector
		temp = p - center;
		n = p / (temp.length());

		//compute the reflection light unit vector
		r =  -(2 * n * ( l * n ) - l);

		//compute the ambient color
		ambientColor.r = ka.r * dl_i.r;
		ambientColor.g = ka.g * dl_i.g;
		ambientColor.b = ka.b * dl_i.b;

		//compute the diffuse color
		GLfloat ln = l*n;
		GLfloat scaler_d = max( ln , 0.0f);

		diffuseColor.r = kd.r * dl_i.r * scaler_d;
		diffuseColor.g = kd.g * dl_i.g * scaler_d;
		diffuseColor.b = kd.b * dl_i.b * scaler_d;

		//compute the specular color
		GLfloat rv = r*v;
		GLfloat scaler_s = max( rv , 0.0f );
		scaler_s = pow(scaler_s, s);

		specularColor.r = ks.r * dl_i.r * scaler_s;
		specularColor.g = ks.g * dl_i.g * scaler_s;
		specularColor.b = ks.b * dl_i.b * scaler_s;

		//compute final color
		finalColor.r = ambientColor.r + diffuseColor.r + specularColor.r;
		finalColor.g = ambientColor.g + diffuseColor.g + specularColor.g;
		finalColor.b = ambientColor.b + diffuseColor.b + specularColor.b;

		return finalColor;
	}
	


	void init_light(){

		//initialize the point light rgb and position
		pl_i.r = 1.0f;
		pl_i.g = 1.0f;
		pl_i.b = 1.0f;

		pl[0] = 300.0f;
		pl[1] = 300.0f;
		pl[2] = 1500.0f;

		//initialize the directional light rgb and position
		dl_i.r = 1.0f;
		dl_i.g = 1.0f;
		dl_i.b = 1.0f;

		dl[0] = 200.0f;
		dl[1] = 50.0f;
		dl[2] = 400.0f;


		l = vec3();
		r = vec3();
		v = vec3();
		n = vec3();

		viewer_position = vec3(500, 330, 800);

	}

};
