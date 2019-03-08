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

vec3 rayThruPixel(int i, int j){
    float width = w;
    float height = h;
    
    float fovy = fovy * pi / 180.0;
    vec3 w = glm::normalize(eyeinit - center);
    vec3 u = glm::normalize(glm::cross(upinit,w));
    vec3 v = glm::cross(w,u); // upinit


    float ratio = (float)width / height;
    float fovx = ratio * fovy;

    float alpha = glm::tan((float)fovx/2) * ((float)(j- (width/2))/((float)width / 2));
    float beta  = glm::tan((float)fovy/2) * ((float)(height/2-i)/((float)height/2));

    vec3 ray=eyeinit+ (alpha*u + beta*v -w / glm::normalize(alpha*u+beta*v-w));

    return ray;
}

float SphereIntersection (){
    
    Sphere* s = new Sphere;
    Ray* r = new Ray;
    vec4 p0 = vec4(p0[0],p0[1],p0[2],1);
    vec4 p1  = vec4(p1[0], p1[1], p1[2], 0);
    vec4 c = vec4(s.center[0], s.center[1], s.center[2], 1);
    
    p0 = transform * p0;
    p1 = transform * p1;
    
    
    float a = glm::dot(p1,p1);
    float b = 2 * glm::dot(p1 , (p0 - c));
    float c = glm::dot((P0h - ch),(p0 - c)) - pow(s.r,2);
    float discriminant = pow(b,2) - 4 * a * c;
    if (discriminant > 0){
        float t = -b - sqrt(b*b-4*a*c) / (2 * a);
    }
    return t;

    float TriangleIntersection () {
        
        
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



    FreeImage_DeInitialise();
    return 0;
}
