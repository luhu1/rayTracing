#ifndef DISPLAY_H
#define DISPLAY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
typedef glm::vec3 vec3 ;
typedef glm::vec4 vec4 ;
typedef glm::mat4 mat4 ;

#include "variables.h"
#include "object.hpp"
#include "ray.hpp"

vec3 ComputeLight (const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, const vec3 diffuse, const vec3 specular, const float shininess);
float calAttenuation(float dist);
vec3 calColor(Hit *hit);
vec3 FindColor (Hit *hit);

#endif
