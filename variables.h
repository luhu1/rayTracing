#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
#include <string>
#include <iostream>
#include "object.hpp"
#include "ray.hpp"


extern vec3 eye ;
extern vec3 up ;
extern vec3 center ;
extern int maxdepth;
extern int width, height ;
extern float fovy ;
extern std::string output;
extern vec3 attenuation;

// Materials (read from file)
extern vec3 ambient ;
extern vec3 diffuse ;
extern vec3 specular ;
extern vec3 emission ;
extern float shininess ;

extern mat4 projection, modelview; // The mvp matrices

extern std::vector <vec3> dirlightposn; // Directional Light Positions
extern std::vector <vec3> dirlightcolor; // Directional Light Colors
extern std::vector <vec3> dirlightransf; // Directional Lights transformed by modelview

extern std::vector <vec3> ptlightposn; // Point Light Positions
extern std::vector <vec3> ptlightcolor; // Point Light Colors
extern std::vector <vec3> ptlightransf; // Point Lights transformed by modelview


extern int maxverts;
extern std::vector <vec3> vertices;
extern std::vector <Object*> objects;

#endif
