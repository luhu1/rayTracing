#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <cstring>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
typedef glm::vec3 vec3 ;
typedef glm::vec4 vec4 ;
typedef glm::mat4 mat4 ;

#include "variables.h"
#include "ray.hpp"
#include "object.hpp"
#include "Transform.h"

void matransform (std::stack<mat4> &transfstack, float * values) ;
void rightmultiply (const mat4 & M, std::stack<mat4> &transfstack) ;
void vectransform(vec3 &point, mat4 M);
bool readvals (std::stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename) ;

#endif
