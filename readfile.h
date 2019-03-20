// Readfile definitions

void matransform (stack<mat4> &transfstack, float * values) ;
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
void vectransform(vec3 &point, mat4 M);
bool readvals (stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename) ;
