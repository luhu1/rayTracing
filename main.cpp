#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include "Transform.h"
#include <FreeImage.h>

using namespace std;

// Main variables in the program.
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp
#include "ray.hpp"
void display(void);  // prototype for display function.

// Reshapes the window
void reshape(int width, int height){
    w = width;
    h = height;

    float aspect = (float) w / (float) h, zNear = 0.1, zFar = 99.0 ;
    // I am changing the projection matrix to fit with the new window aspect ratio
    projection = Transform::perspective(fovy, aspect, zNear, zFar) ;
}


void init(){

}

void destroy(){
    for(int i=0; i<objects.size(); i++)
        free(objects[i]);
}

Ray rayThruPixel(int i, int j){
    float width = w;
    float height = h;

    float fovy = fovy * pi / 180.0;
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = up;

    float ratio = (float)width / height;
    float fovx = ratio * fovy;

    float alpha = glm::tan((float)fovx/2) * ((float)(j- (width/2))/((float)width / 2));
    float beta  = glm::tan((float)fovy/2) * ((float)(height/2-i)/((float)height/2));

    vec3 direction = alpha*u + beta*v -w / glm::normalize(alpha*u+beta*v-w);
    Ray ray = Ray(eye, direction);

    return ray;
}


vec3 FindColor (){
    return vec3(0, 0, 0);
}


float SphereIntersection (vec3 p0, vec3 p1){
    
    Sphere* s = new Sphere;
    float t=0;
    vec4 P0 = vec4(p0[0],p0[1],p0[2],1);
    vec4 P1  = vec4(p1[0], p1[1], p1[2], 1);
    vec4 center = vec4(s->center[0], s->center[1], s->center[2], 1);
    
    P0 = s->transform * P0;
    P1 = s->transform * P1;
    
    
    float a = glm::dot(P1,P1);
    float b = 2 * glm::dot(P1 , (P0 - center));
    float c = glm::dot((P0 - center),(P0 - center)) - pow(s->radius,2);
    float discriminant = pow(b,2) - 4 * a * c;
    if (discriminant > 0){
        float t = -b - sqrt(b*b-4*a*c) / (2 * a);
    }
    return t;
}
 

int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    if (argc < 2) {
    cerr << "Usage: transforms scene.test\n";
    exit(-1);
    }

    init();
    readfile(argv[1]);
    reshape(w, h);

    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            Ray ray = rayThruPixel(i, j);
            hitObjects.clear();
        }
    }


    destroy();
    FreeImage_DeInitialise();
    return 0;
}
