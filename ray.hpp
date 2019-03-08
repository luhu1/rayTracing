#ifndef RAY_H
#define RAY_H
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    vec3 p0, p1;
};
