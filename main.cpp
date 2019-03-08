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


void SphereIntersection (Ray ray, Sphere *s, Hit *h){
    
    float t=0;
    /*
    vec4 P0 = vec4(p0[0],p0[1],p0[2],1);
    vec4 P1  = vec4(p1[0], p1[1], p1[2], 1);
     */
    vec3 center = s->center;
    
   // P0 = s->transform * P0;
    //P1 = s->transform * P1;
    
    
    float a = glm::dot(ray.p1,ray.p1);
    float b = 2 * glm::dot(ray.p1 , (ray.p0 - center));
    float c = glm::dot((ray.p0 - center),(ray.p0 - center)) - pow(s->radius,2);
    float discriminant = pow(b,2) - 4 * a * c;
    if (discriminant > 0){
        float t = -b - sqrt(b*b-4*a*c) / (2 * a);
        vec3 point = ray.p0+ray.p1*t;
        vec3 n = (point-center)/glm::normalize(point-center);
        h = new Hit();
        h->t=t;
        h->normal=n;
        Sphere *temp = s;
        h->obj= temp;
        h->p=point;
    }
}

void TriangleIntersection(Ray ray, Triangle *tri, Hit *h) {
    
    /*
    vec3 a = vec3(t->v1[0]/t->v1[3],t->v1[1]/t->v1[3],t->v1[2]/t->v1[3]);
    vec3 b = vec3(t->v2[0]/t->v2[3],t->v2[1]/t->v2[3],t->v2[2]/t->v2[3]);
    vec3 c = vec3(t->v3[0]/t->v3[3],t->v3[1]/t->v3[3],t->v3[2]/t->v3[3]);
    */
    vec3 v1 = tri->v1;
    vec3 v2 = tri->v2;
    vec3 v3 = tri->v3;
    
    vec3 n= glm::cross((v3-v1),(v2-v1))/glm::normalize(glm::cross((v3-v1),(v2-v1)));
    float t = (glm::dot(v1,n) - glm::dot(ray.p0,n)) / (glm::dot(ray.p1,n));
    vec3 point = ray.p0 + ray.p1*t;
    float beta, gamma;
    float a1,b1,c1,a2,b2,c2;
    a1=v2[0]-v1[0];
    b1=v3[0]-v1[0];
    c1=point[0]-v1[0];
    a2=v2[1]-v1[1];
    b2=v3[1]-v1[1];
    c2=point[1]-v1[1];
    gamma = (c2*a1-a2*c1)/(b2*a1-a2*b1);
    beta = c1/a1 - (b1/a1)*gamma;
    if (gamma+beta <1 && gamma<=1 && 0<=gamma && 0<=beta && beta<=1){
        h = new Hit();
        h->t=t;
        h->normal=n;
        Triangle *temp = tri;
        h->obj= temp;
        h->p=point;
    }
    
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
