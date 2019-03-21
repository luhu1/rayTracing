#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <vector>
#include <FreeImage.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
typedef glm::vec3 vec3 ;
typedef glm::vec4 vec4 ;
typedef glm::mat4 mat4 ;

#include "Transform.h"
#include "readfile.h"
#include "object.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "display.hpp"

vec3 eye ;
vec3 up ;
vec3 center ;
int maxdepth;
int width, height ;
float fovy ;
std::string output;
float epsilon;

mat4 projection, modelview; // The mvp matrices

vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 emission;
vec3 attenuation;
float shininess;
std::vector <Light*> lights;

 int maxverts;
 std::vector <vec3> vertices;
 std::vector <Object*> objects;


void init();
void destroy();
vec3 recRayTracing(Ray ray, int depth);
void saveScreenshot(std::string fname, vec3 *pix);
