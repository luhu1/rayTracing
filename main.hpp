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
#include "ray.hpp"
#include "display.hpp"

 vec3 eye ;
 vec3 up ;
 vec3 center ;
 int maxdepth;
 int width, height ;
 float fovy ;
 std::string output;
 vec3 attenuation;

// Materials (read from file)
 vec3 ambient ;
 vec3 diffuse ;
 vec3 specular ;
 vec3 emission ;
 float shininess ;

 mat4 projection, modelview; // The mvp matrices

 std::vector <vec3> dirlightposn; // Directional Light Positions
 std::vector <vec3> dirlightcolor; // Directional Light Colors
 std::vector <vec3> dirlightransf; // Directional Lights transformed by modelview

 std::vector <vec3> ptlightposn; // Point Light Positions
 std::vector <vec3> ptlightcolor; // Point Light Colors
 std::vector <vec3> ptlightransf; // Point Lights transformed by modelview


 int maxverts;
 std::vector <vec3> vertices;
 std::vector <Object*> objects;


void init();
void destroy();
vec3 recRayTracing(Ray ray, int depth);
void saveScreenshot(std::string fname, vec3 *pix);
