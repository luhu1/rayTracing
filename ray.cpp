#include "ray.hpp"

float calDistance(const vec3 p1, const vec3 p2){
    vec3 p = p1 - p2;
    return p.x*p.x + p.y*p.y + p.z*p.z;
}
