/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include "Transform.h"

using namespace std ;
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.
// Some of you may want to use the more modern routines in readfile.cpp
// that can also be used.
void transformvec (const float input[4], float output[4])
{
  glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
  glm::vec4 outputvec = modelview * inputvec;
  output[0] = outputvec[0];
  output[1] = outputvec[1];
  output[2] = outputvec[2];
  output[3] = outputvec[3];
}

void display()
{
    modelview = Transform::lookAt(eye,center,up);

    for(int i=0; i<numused; i++)
        transformvec(lightposn+i*4, lightransf+i*4);

  // Transformations for objects, involving translation and scaling
  mat4 sc(1.0) , tr(1.0), transf(1.0);
  sc = Transform::scale(sx,sy,1.0);
  tr = Transform::translate(tx,ty,0.0);

  transf = modelview * tr * sc;

  modelview = transf;

  for (int i = 0 ; i < numobjects ; i++) {
    object* obj = &(objects[i]); // Grabs an object struct.

    modelview = transf * obj->transform;


    // Actually draw the object
    // We provide the actual drawing functions for you.
    // Remember that obj->type is notation for accessing struct fields
    // if (obj->type == cube) {
    //   solidCube(obj->size);
    // }
    // else if (obj->type == sphere) {
    //   const int tessel = 20;
    //   solidSphere(obj->size, tessel, tessel);
    // }
    // else if (obj->type == teapot) {
    //   solidTeapot(obj->size);
    // }
  }
}
