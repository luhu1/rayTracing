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


vec3 ComputeLight (const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, const vec3 mydiffuse, const vec3 myspecular, const float myshininess) {

    float nDotL = dot(normal, direction)  ;
    vec3 lambert = mydiffuse * lightcolor * max (nDotL, 0.0f) ;

    float nDotH = dot(normal, halfvec) ;
    vec3 phong = myspecular * lightcolor * pow (max(nDotH, 0.0f), myshininess) ;

    vec3 retval = lambert + phong ;
    return retval ;
    }


vec3 calColor(Hit *hit){
    vec3 normal = hit->normal;
    vec3 mypos = hit->p;
    vec3 ambient = hit->obj->ambient;
    vec3 diffuse = hit->obj->diffuse;
    vec3 specular = hit->obj->specular;
    vec3 emission = hit->obj->emission;
    float shininess = hit->obj->shininess;

    vec3 fragColor = vec3(0.0f, 0.0f, 0.0f);
    vec3 eyedirn = normalize(eye - mypos) ;

    fragColor = ambient;
    vec3 col;

    // Directional lights
    for (int i=0; i<dirlightposn.size(); i++){
        vec3 direction = normalize (dirlightposn[i]) ;
        vec3 half1 = normalize (direction + eyedirn) ;
        col = ComputeLight(direction, dirlightcolor[i], normal, half1, diffuse, specular, shininess) ;
        fragColor += col;
    }

    // Point lights
    for (int i=0; i<ptlightposn.size(); i++){
        vec3 position = ptlightposn[i];
        vec3 direction = normalize (position - mypos) ;
        vec3 half1 = normalize (direction + eyedirn) ;
        col = ComputeLight(direction, ptlightcolor[i], normal, half1, diffuse, specular, shininess) ;
        fragColor += col;
    }

    // fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return fragColor;
}


vec3 FindColor (Hit *hit){
    if (hit == NULL)
        return vec3(0, 0, 0);
    else if (hit->obj->typeName == sphereType)
        return calColor(hit);
    else if (hit->obj->typeName == triangleType)
        return calColor(hit);
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
