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
#include "readfile.h"
void display(void);

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
        }
    }
    saveScreenshot(argv[1], pixels);

    destroy();
    FreeImage_DeInitialise();
    return 0;
}
