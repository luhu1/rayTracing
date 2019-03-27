#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
typedef glm::vec3 vec3 ;
typedef glm::mat4 mat4 ;

enum lightType {directionalType, pointType};

struct Light {
    lightType typeName;
    vec3 pos;
    vec3 transf;
    vec3 color;
};
typedef struct Light Light;


#endif
