#include "readfile.h"
using namespace std;

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, float* values)
{
  mat4 transform = transfstack.top();
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]);
  vec4 newval = transform * valvec;
  for (int i = 0; i < 4; i++) values[i] = newval[i];
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack)
{
  mat4 &T = transfstack.top();
  T = T * M;
}

void vectransform(vec3 &point, mat4 M){
    glm::vec4 p = vec4(point.x, point.y, point.z, 1.0f);
    p = M * p;
    point = vec3(p.x/p.w, p.y/p.w, p.z/p.w);
}

// Function to read the input data values
bool readvals(stringstream &s, const int numvals, float* values)
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i];
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n";
      return false;
    }
  }
  return true;
}

void readfile(const char* filename)
{
  string str, cmd;
  ifstream in;
  in.open(filename);
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.
    // This is done using standard STL Templates
    stack <mat4> transfstack;
    transfstack.push(mat4(1.0));  // identity

    getline (in, str);
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines

        stringstream s(str);
        s >> cmd;
        int i;
        float values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.
        bool validinput; // Validity of input

        // Lighting Command
        if (cmd == "directional") {
            validinput = readvals(s, 6, values);
            if (validinput){
                Light *light = new Light();
                light->typeName = directionalType;
                light->pos = vec3(values[0], values[1], values[2]);
                light->color = vec3(values[3], values[4], values[5]);
                lights.push_back(light);

            }
        }

        else if (cmd == "point") {
            validinput = readvals(s, 6, values);
            if (validinput){
                Light *light = new Light();
                light->typeName = pointType;
                light->pos = vec3(values[0], values[1], values[2]);
                light->color = vec3(values[3], values[4], values[5]);
                lights.push_back(light);
            }
        }

        // Material Commands
        // Ambient, diffuse, specular, shininess properties for each object.

        else if (cmd == "ambient") {
          validinput = readvals(s, 3, values);
          if (validinput) {
              ambient = vec3(values[0], values[1], values[2]);
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values);
          if (validinput) {
              diffuse = vec3(values[0], values[1], values[2]);
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 3, values);
          if (validinput) {
              specular = vec3(values[0], values[1], values[2]);
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 3, values);
          if (validinput) {
              emission = vec3(values[0], values[1], values[2]);
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values);
          if (validinput) {
            shininess = values[0];
          }
        } else if (cmd == "size") {
          validinput = readvals(s,2,values);
          if (validinput) {
            width = (int) values[0];
            height = (int) values[1];
          }
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {
            eye = vec3(values[0], values[1], values[2]);
            center = vec3(values[3], values[4], values[5]);
            up = vec3(values[6], values[7], values[8]);
            up = Transform::upvector(up, eye-center);
            fovy = values[9];
          }
        }

        else if (cmd == "translate") {
          validinput = readvals(s,3,values);
          if (validinput) {

            float tx = values[0];
            float ty = values[1];
            float tz = values[2];

            mat4 T = glm::translate(mat4(), vec3(tx, ty, tz));
            rightmultiply(T, transfstack);

          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values);
          if (validinput) {
            float sx = values[0];
            float sy = values[1];
            float sz = values[2];

            mat4 S = glm::scale(mat4(), vec3(sx, sy, sz));
            rightmultiply(S, transfstack);

          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values);
          if (validinput) {

            vec3 axis = vec3(values[0], values[1], values[2]);
            axis = normalize(axis);

            float degrees = values[3];
            degrees = degrees * M_PI / 180;

            mat4 R = glm::rotate(mat4(), degrees, axis);
            rightmultiply(R, transfstack);
          }
        }

        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top());
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n";
          } else {
            transfstack.pop();
          }
        }

        else if (cmd == "output"){
            s >> output;
            if (s.fail()) {
              cout << "Failed reading value " << 0 << " will skip\n";
          }
        }

        else if (cmd == "attenuation"){
            validinput = readvals(s,3,values);
            if (validinput) {
                attenuation[0] = values[0];
                attenuation[1] = values[1];
                attenuation[2] = values[2];
            }
        }

        else if (cmd == "maxdepth"){
            validinput = readvals(s,1,values);
            if (validinput) {
                maxdepth = (int)values[0];
            }
        }

        else if (cmd == "maxverts"){
            validinput = readvals(s,1,values);
            if (validinput) {
                maxverts = (int)values[0];
            }
        }

        else if (cmd == "vertex") {
            if (vertices.size() == maxverts) { // No more vertex
              cerr << "Reached Maximum Number of Vertices " << maxverts << " Will ignore further vertices\n";
            } else {
            validinput = readvals(s,3,values);
            if (validinput) {
                vertices.push_back(vec3(values[0], values[1], values[2]));
            }
        }
        }

        else if (cmd == "tri") {
            validinput = readvals(s,3,values);
            if (validinput) {
                Triangle *tri = new Triangle();
                tri->typeName = triangleType;
                tri->v1 = vec3(vertices[(int)values[0]]);
                tri->v2 = vec3(vertices[(int)values[1]]);
                tri->v3 = vec3(vertices[(int)values[2]]);

                tri->ambient = vec3(ambient);
                tri->emission = vec3(emission);
                tri->diffuse = vec3(diffuse);
                tri->specular = vec3(specular);
                tri->shininess = shininess;

                tri->transform = transfstack.top();
                vectransform(tri->v1, tri->transform);
                vectransform(tri->v2, tri->transform);
                vectransform(tri->v3, tri->transform);
                objects.push_back(tri);
            }
        }

        else if (cmd == "sphere") {
            validinput = readvals(s,4,values);
        if (validinput) {
            Sphere *s = new Sphere();
            s->typeName = sphereType;
            s->center = vec3(values[0], values[1], values[2]);
            s->radius = values[3];

            s->ambient = vec3(ambient);
            s->emission = vec3(emission);
            s->diffuse = vec3(diffuse);
            s->specular = vec3(specular);
            s->shininess = shininess;

            s->transform = transfstack.top();
            objects.push_back(s);
            }
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n";
        }
      }
      getline (in, str);
    }

  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n";
    throw 2;
  }
}
