#include "display.hpp"
using namespace std;


vec3 ComputeLight (const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, const vec3 diffuse, const vec3 specular, const float shininess) {

    float nDotL = dot(normal, direction)  ;
    vec3 lambert = diffuse * lightcolor * max (nDotL, 0.0f) ;

    float nDotH = dot(normal, halfvec) ;
    vec3 phong = specular * lightcolor * pow (max(nDotH, 0.0f), shininess) ;

    vec3 retval = lambert + phong ;
    return retval ;
    }


float calAttenuation(float dist){
    float v = attenuation[0] + attenuation[1] * dist + attenuation[2] * dist * dist;
    return 1.0f / v;
}


vec3 calColor(Hit *hit){
    vec3 normal = hit->normal;
    vec3 mypos = hit->p;

    vec3 ambient = hit->obj->ambient;
    vec3 emission = hit->obj->emission;
    vec3 diffuse = hit->obj->diffuse;
    vec3 specular = hit->obj-> specular;
    float shininess = hit->obj->shininess;

    vec3 fragColor = ambient + emission;
    vec3 eyedirn = normalize(eye - mypos) ;

    vec3 col;

    for (int i=0; i<lights.size(); i++){

        // Directional lights
        if (lights[i]->typeName == directionalType){
            vec3 direction = normalize (lights[i]->pos) ;
            vec3 half1 = normalize (direction + eyedirn) ;

            if (lightVisibility(mypos, lights[i])){
                col = ComputeLight(direction, lights[i]->color, normal, half1, diffuse, specular, shininess) ;
                fragColor += col;
            }
        }

        // Point lights
        else if (lights[i]->typeName == pointType){
            vec3 direction = normalize (lights[i]->pos - mypos) ;
            vec3 half1 = normalize (direction + eyedirn) ;

            if (lightVisibility(mypos, lights[i])){
                col = ComputeLight(direction, lights[i]->color, normal, half1, diffuse, specular, shininess) ;
                float attn = calAttenuation(distance(mypos, lights[i]->pos));
                fragColor += attn * col;
            }
        }
    }

    return fragColor;
}
