# version 330 core
// Do not use any version older than 330!

/* This is the fragment shader for reading in a scene description, including
   lighting.  Uniform lights are specified from the main program, and used in
   the shader.  As well as the material parameters of the object.  */

// Inputs to the fragment shader are the outputs of the same name of the vertex shader.
// Note that the default output, gl_Position, is inaccessible!
in vec3 mynormal;
in vec4 myvertex;

// You will certainly need this matrix for your lighting calculations
uniform mat4 modelview;

// This first defined output of type vec4 will be the fragment color
out vec4 fragColor;

uniform vec3 color;

const int numLights = 10;
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.
// I use ambient, diffuse, specular, shininess.
// But, the ambient is just additive and doesn't multiply the lights.

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;


vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction)  ;
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;

        float nDotH = dot(normal, halfvec) ;
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ;

        vec4 retval = lambert + phong ;
        return retval ;
        }


void main (void)
{
    if (enablelighting) {
        vec4 finalcolor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

        const vec3 eyepos = vec3(0,0,0) ;
        vec4 myvertex2 = modelview * myvertex;
        vec3 mypos = myvertex2.xyz / myvertex2.w ; // Dehomogenize current location

        vec3 eyedirn = normalize(eyepos - mypos) ;

        // Compute normal, needed for shading.
        vec3 mynormal2 =  mat3(transpose(inverse(modelview))) * mynormal ;
        vec3 normal = normalize(mynormal2) ;

        finalcolor = ambient;
        vec4 col;

        for(int i=0; i<numused; i++){
            if (lightposn[i].w == 0){
                // directional light
                vec3 direction = normalize (lightposn[i].xyz) ;
                vec3 half1 = normalize (direction + eyedirn) ;
                col = ComputeLight(direction, lightcolor[i], normal, half1, diffuse, specular, shininess) ;

            }else{
                // point light
                vec3 position = lightposn[i].xyz / lightposn[i].w ;
                vec3 direction = normalize (position - mypos) ; // no attenuation
                vec3 half1 = normalize (direction + eyedirn) ;
                col = ComputeLight(direction, lightcolor[i], normal, half1, diffuse, specular, shininess) ;
            }

            finalcolor += col;
        }

        // finalcolor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        fragColor = finalcolor;
    } else {
        fragColor = vec4(color, 1.0f);
    }
}
