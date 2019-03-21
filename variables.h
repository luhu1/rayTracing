#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
#include <string>
#include <iostream>
#include "object.hpp"
#include "light.hpp"
#include "ray.hpp"


extern vec3 eye ;
extern vec3 up ;
extern vec3 center ;
extern int maxdepth;
extern int width, height ;
extern float fovy ;
extern std::string output;

extern vec3 ambient;
extern vec3 diffuse;
extern vec3 specular;
extern vec3 emission;
extern vec3 attenuation;
extern float shininess;

extern mat4 projection, modelview; // The mvp matrices

extern std::vector <Light*> lights;

extern int maxverts;
extern std::vector <vec3> vertices;
extern std::vector <Object*> objects;

#endif
