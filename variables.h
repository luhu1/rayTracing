#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
#include <string>
#include <iostream>
#include "object.hpp"
#include "ray.hpp"

#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up

#ifdef MAINPROGRAM
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point
int maxdepth = 1;   // maximum recursive depth
int width = 500, height = 500 ; // width and height
float fovy = 90.0 ; // For field of view
std::string output = "output.png";
vec3 attenuation = vec3(1.0f, 0.0f, 0.0f);

// Materials (read from file)
// With multiple objects, these are colors for each.
vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);;
vec3 specular = vec3(0.0f, 0.0f, 0.0f);;
vec3 emission = vec3(0.0f, 0.0f, 0.0f);;
float shininess = 1.0f;
#else
EXTERN vec3 eyeinit ;
EXTERN vec3 upinit ;
EXTERN vec3 center ;
EXTERN int maxdepth;
EXTERN int width, height ;
EXTERN float fovy ;
EXTERN std::string output;
EXTERN vec3 attenuation;

// Materials (read from file)
// With multiple objects, these are colors for each.
EXTERN vec3 ambient ;
EXTERN vec3 diffuse ;
EXTERN vec3 specular ;
EXTERN vec3 emission ;
EXTERN float shininess ;
#endif

EXTERN mat4 projection, modelview; // The mvp matrices
static enum {view, translate, scale} transop ; // which operation to transform
EXTERN float sx, sy ; // the scale in x and y
EXTERN float tx, ty ; // the translation in x and y


EXTERN std::vector <vec3> dirlightposn; // Directional Light Positions
EXTERN std::vector <vec3> dirlightcolor; // Directional Light Colors
EXTERN std::vector <vec3> dirlightransf; // Directional Lights transformed by modelview

EXTERN std::vector <vec3> ptlightposn; // Point Light Positions
EXTERN std::vector <vec3> ptlightcolor; // Point Light Colors
EXTERN std::vector <vec3> ptlightransf; // Point Lights transformed by modelview


EXTERN int maxverts;
EXTERN std::vector <vec3> vertices;



EXTERN std::vector <Object*> objects;

#endif
