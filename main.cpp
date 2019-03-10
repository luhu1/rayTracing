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
void reshape(){
    float aspect = (float) width / (float) height, zNear = 0.1, zFar = 99.0 ;
    projection = Transform::perspective(fovy, aspect, zNear, zFar) ;
}


void init(){

}

void destroy(){
    for(int i=0; i<objects.size(); i++)
        free(objects[i]);
}


Ray rayThruPixel(int i, int j){
    float fovyR = glm::radians(fovy);
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::normalize(glm::cross(w, u));

    float ratio = (float) width / height;
    float fovxR = ratio * fovyR;

    float alpha = glm::tan((float)fovxR/2) * ((j-((float)width/2))/((float)width / 2));
    float beta  = glm::tan((float)fovyR/2) * (((float)height/2-i)/((float)height/2));

    vec3 direction = glm::normalize(alpha*u+beta*v-w);
    Ray ray = Ray(eye, direction);

    return ray;
}


vec3 FindColor (Hit *hit){
    if (hit == NULL)
        return vec3(0, 0, 0);
    else if (hit->obj->typeName == sphereType)
        return vec3(0, 255, 0);
    else if (hit->obj->typeName == triangleType)
        return vec3(0, 0, 255);
    else
        return vec3(0, 0, 0);
}


void SphereIntersection (Ray ray, Sphere *s, Hit *&h){

    vec3 center = s->center;
    float a = glm::dot(ray.p1,ray.p1);
    float b = 2 * glm::dot(ray.p1 , (ray.p0 - center));
    float c = glm::dot((ray.p0 - center),(ray.p0 - center)) - pow(s->radius,2);
    float discriminant = pow(b,2) - 4 * a * c;
    if (discriminant > 0){
        float t = -b - sqrt(b*b-4*a*c) / (2 * a);
        if (t <= 0)
            return;
        vec3 point = ray.p0+ray.p1*t;
        vec3 n = glm::normalize(point-center);
        h = new Hit();
        h->t = t;
        h->normal = n;
        h->obj = s;
        h->p = point;
    }
}

void TriangleIntersection(Ray ray, Triangle *tri, Hit *&h) {

    vec3 v1 = tri->v1;
    vec3 v2 = tri->v2;
    vec3 v3 = tri->v3;

    vec3 n= glm::normalize(glm::cross((v3-v1),(v2-v1)));
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

    if ((gamma+beta) <=1 && 0<=gamma && gamma<=1 && 0<=beta && beta<=1 && t > 0){
        h = new Hit();
        h->t = t;
        h->normal = n;
        h->obj = tri;
        h->p = point;
    }
}


Hit * Intersect(Ray ray){
    hitList.clear();

    for (int i=0; i<objects.size(); i++){
        Hit *h = NULL;
        if (objects[i]->typeName == sphereType){
            SphereIntersection (ray, (Sphere*)objects[i], h);
        }
        else if (objects[i]->typeName == triangleType){
            TriangleIntersection(ray, (Triangle*)objects[i], h);
        }
        if (h != NULL)
            hitList.push_back(h);
    }

    if (hitList.empty())
        return NULL;

    // find the closest Intersected object
    Hit *h = hitList[0];
    float minDist = calDistance(eye, hitList[0]->p);

    for (int i=1; i< hitList.size(); i++){
        float dist = calDistance(eye, hitList[i]->p);
        if (dist < minDist){
            minDist = dist;
            h = hitList[i];
        }
    }
    return h;
}

void saveScreenshot(string fname, vec3 *pix) {
  BYTE *pixels = new BYTE[3*width*height];
  for(int i=0; i<width*height; i++){
      pixels[i*3] = (int)pix[i].x;
      pixels[i*3+1] = (int)pix[i].y;
      pixels[i*3+2] = (int)pix[i].z;
  }

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
  fname.append(".png");
  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete[] pixels;
}


int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    if (argc < 2) {
    cerr << "Usage: transforms scene.test\n";
    exit(-1);
    }

    init();
    readfile(argv[1]);
    reshape();

    vec3 *pixels = new vec3[width*height];

    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            Ray ray = rayThruPixel(i, j);
            Hit *hit = Intersect(ray);
            vec3 color = FindColor (hit);
            pixels[i*width+j] = color;
            // pixels[j*h+i] = color;
        }
    }
    saveScreenshot(argv[1], pixels);

    destroy();
    FreeImage_DeInitialise();
    return 0;
}
