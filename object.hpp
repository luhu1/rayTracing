#ifndef OBJECT_H
#define OBJECT_H

enum objectType {sphereType, triangleType};

struct Object {
    objectType typeName;
    float ambient[4] ;
    float diffuse[4] ;
    float specular[4] ;
    float emission[4] ;
    float shininess ;
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
