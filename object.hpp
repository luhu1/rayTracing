#ifndef OBJECT_H
#define OBJECT_H

enum objectType {sphereType, triangleType};

struct Object {
    objectType typeName;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    vec3 shininess ;
    mat4 transform ;
};
typedef struct Object Object;


struct Sphere: Object {
    vec3 center;
    float radius ;
};
typedef struct Sphere Sphere;


struct Triangle: Object {
    vec3 v1, v2, v3;
};
typedef struct Triangle Triangle;

#endif
