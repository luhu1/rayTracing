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
    vec3 diffuse = hit->obj->diffuse;
    vec3 specular = hit->obj->specular;
    vec3 emission = hit->obj->emission;
    float shininess = hit->obj->shininess;

    vec3 fragColor = vec3(0.0f, 0.0f, 0.0f);
    vec3 eyedirn = normalize(eye - mypos) ;

    fragColor = ambient + emission;
    vec3 col;

    // Directional lights
    for (int i=0; i<dirlightposn.size(); i++){
        vec3 direction = normalize (dirlightposn[i]) ;
        vec3 half1 = normalize (direction + eyedirn) ;

        if (lightVisility(mypos, dirlightposn[i], false)){
            col = ComputeLight(direction, dirlightcolor[i], normal, half1, diffuse, specular, shininess) ;
            fragColor += col;
        }
    }

    // Point lights
    for (int i=0; i<ptlightposn.size(); i++){
        vec3 position = ptlightposn[i];
        vec3 direction = normalize (position - mypos) ;
        vec3 half1 = normalize (direction + eyedirn) ;

        if (lightVisility(mypos, position, true)){
            col = ComputeLight(direction, ptlightcolor[i], normal, half1, diffuse, specular, shininess) ;
            float attn = calAttenuation(distance(mypos, ptlightcolor[i]));
            fragColor += attn * col;
        }
    }

    return fragColor;
}


vec3 FindColor (Hit *hit){
    if (!hit)
        return vec3(0.0f, 0.0f, 0.0f);
    else if (hit->obj->typeName == sphereType)
        return calColor(hit);
    else if (hit->obj->typeName == triangleType)
        return calColor(hit);
    else
        return vec3(0.0f, 0.0f, 0.0f);
}
