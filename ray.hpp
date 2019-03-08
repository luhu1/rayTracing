#ifndef RAY_H
#define RAY_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
typedef glm::vec3 vec3 ;
typedef glm::mat4 mat4 ;

#include "object.hpp"

typedef glm::vec3 vec3 ;

class Ray{
public:
    Ray();
    Ray(vec3& p0, vec3& p1){
        this->p0 = p0;
        this->p1 = p1;
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


float calDistance(const vec3 p1, const vec3 p2);

#endif
