/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.
// Since all files need access to it, we define EXTERN as either blank or
// extern, depending on if included in the main program or not.

#include <vector>

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
int amountinit = 5;
int w = 500, h = 500 ; // width and height
float fovy = 90.0 ; // For field of view
#else
EXTERN vec3 eyeinit ;
EXTERN vec3 upinit ;
EXTERN vec3 center ;
EXTERN int amountinit;
EXTERN int w, h ;
EXTERN float fovy ;
#endif

EXTERN mat4 projection, modelview; // The mvp matrices
static enum {view, translate, scale} transop ; // which operation to transform
enum shape {cube, sphere, teapot} ;
EXTERN float sx, sy ; // the scale in x and y
EXTERN float tx, ty ; // the translation in x and y


EXTERN vector <vec3> dirlightposn; // Directional Light Positions
EXTERN vector <vec3> dirlightcolor; // Directional Light Colors
EXTERN vector <vec3> dirlightransf; // Directional Lights transformed by modelview

EXTERN vector <vec3> ptlightposn; // Point Light Positions
EXTERN vector <vec3> ptlightcolor; // Point Light Colors
EXTERN vector <vec3> ptlightransf; // Point Lights transformed by modelview


EXTERN int maxverts;
EXTERN vector <vec3> vertices;
EXTERN vector <vec3> triangles;

// Materials (read from file)
// With multiple objects, these are colors for each.
EXTERN float ambient[3] ;
EXTERN float diffuse[3] ;
EXTERN float specular[3] ;
EXTERN float emission[3] ;
EXTERN float shininess ;

// For multiple objects, read from a file.
const int maxobjects = 10 ;
EXTERN int numobjects ;
EXTERN struct object {
  shape type ;
  float size ;
  float ambient[4] ;
  float diffuse[4] ;
  float specular[4] ;
  float emission[4] ;
  float shininess ;
  mat4 transform ;
} objects[maxobjects] ;
