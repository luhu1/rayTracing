#ifndef RAY_H
#define RAY_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
typedef glm::vec3 vec3 ;
typedef glm::vec4 vec4 ;
typedef glm::mat4 mat4 ;

#include "variables.h"

class Ray{
public:
    Ray();
    Ray(vec3 p0, vec3 p1){
        this->p0 = vec3(p0);
        this->p1 = vec3(p1);
    }

    vec3 rayPath(float t){
        return p0 + t * p1;
    }

    vec3 p0;    // Origin
    vec3 p1;    // Direction
};


struct Hit {
    float t;
    vec3 p; // Intersection point
    vec3 normal;
    Object *obj;
};
typedef struct Hit Hit;


int lightVisility(vec3 pos, vec3 lightpos, bool isPtLight);
vec3 calReflection(vec3 d, vec3 n);
Ray rayThruPixel(float i, float j);
Hit * Intersect(Ray ray);
void SphereIntersection (Ray ray, Sphere *s, std::vector<Hit*>& hitList);
void TriangleIntersection(Ray ray, Triangle *tri, std::vector<Hit*>& hitList);

#endif
