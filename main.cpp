#include "main.hpp"
using namespace std;

// Reshapes the window
void reshape(){
    float aspect = (float) width / (float) height, zNear = 0.1, zFar = 99.0 ;
    projection = Transform::perspective(fovy, aspect, zNear, zFar) ;
}


void init(){
    eye = vec3(0.0,0.0,5.0);
    up = vec3(0.0,1.0,0.0);
    center = vec3(0.0,0.0,0.0);
    maxdepth = 1;
    width = 500, height = 500 ; // window width and height
    fovy = 90.0 ; // For field of view
    output = "output.png";
    attenuation = vec3(1.0f, 0.0f, 0.0f);

    // Materials
    ambient = vec3(0.0f, 0.0f, 0.0f);
    diffuse = vec3(0.0f, 0.0f, 0.0f);;
    specular = vec3(0.0f, 0.0f, 0.0f);;
    emission = vec3(0.0f, 0.0f, 0.0f);;
    shininess = 1.0f;
}

void destroy(){
    for(int i=0; i<objects.size(); i++)
        free(objects[i]);
}


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

vec3 recRayTracing(Ray ray, int depth){
    if (depth == maxdepth)
        return vec3(0.0f, 0.0f, 0.0f);

    Hit *hit = Intersect(ray);

    if (!hit)
        return vec3(0.0f, 0.0f, 0.0f);

    vec3 color = FindColor(hit);

    vec3 dir = calReflection(ray.p1, hit->normal);
    Ray ray2 = Ray(hit->p, dir);
    return color + recRayTracing(ray2, depth+1);
}


void saveScreenshot(string fname, vec3 *pix) {
  BYTE *pixels = new BYTE[3*width*height];
  for(int i=0; i<width*height; i++){
      pixels[i*3] = (int)(pix[i].z * 255);
      pixels[i*3+1] = (int)(pix[i].y * 255);
      pixels[i*3+2] = (int)(pix[i].x * 255);
  }

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete[] pixels;
}


int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    if (argc < 2) {
    cerr << "Usage: transforms scene.test\n";
    exit(-1);
    }

    init();
    readfile(argv[1]);
    reshape();

    vec3 *pixels = new vec3[width*height];

    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            Ray ray = rayThruPixel(i, j);
            vec3 color = recRayTracing(ray, 0);
            pixels[(height-i-1)*width+j] = color;
        }
    }
    saveScreenshot(output, pixels);

    destroy();
    FreeImage_DeInitialise();
    return 0;
}
