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


int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    if (argc < 2) {
    cerr << "Usage: transforms scene.test\n";
    exit(-1);
    }

    init();
    readfile(argv[1]);

    FreeImage_DeInitialise();
    return 0;
}
