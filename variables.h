#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
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
int maxdepth = 3;   // maximum recursive depth
int width = 500, height = 500 ; // width and height
float fovy = 90.0 ; // For field of view
#else
EXTERN vec3 eyeinit ;
EXTERN vec3 upinit ;
EXTERN vec3 center ;
EXTERN int maxdepth;
EXTERN int width, height ;
EXTERN float fovy ;
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

// Materials (read from file)
// With multiple objects, these are colors for each.
EXTERN vec3 ambient ;
EXTERN vec3 diffuse ;
EXTERN vec3 specular ;
EXTERN vec3 emission ;
EXTERN float shininess ;

EXTERN std::vector <Hit*> hitList;
EXTERN std::vector <Object*> objects;

#endif
