#include "ray.hpp"
using namespace std;

vec3 vectransform2(vec3 point, mat4 M){
    glm::vec4 p = glm::vec4(point.x, point.y, point.z, 1.0f);
    p = M * p;
    return vec3(p.x/p.w, p.y/p.w, p.z/p.w);
}

vec3 vectransform3(vec3 point, mat4 M){
    glm::vec4 p = glm::vec4(point.x, point.y, point.z, 0.0f);
    p = M * p;
    return vec3(p.x, p.y, p.z);
}


int lightVisibility(vec3 pos, Light *light){
    vector <Hit*> hitList;
    vec3 dir;

    if (light->typeName == pointType){
        dir = glm::normalize(light->pos - pos);
    }
    else if (light->typeName == directionalType){
        dir = glm::normalize(-(light->pos));
    }
    else{
        return 0;
    }

    vec3 p0 = pos + dir * epsilon;  // avoid numerical inaccuracy
    Ray ray = Ray(p0, dir);


    for (int i=0; i<objects.size(); i++){
        if (objects[i]->typeName == sphereType){
            SphereIntersection (ray, (Sphere*)objects[i], hitList);
        }
        else if (objects[i]->typeName == triangleType){
            TriangleIntersection(ray, (Triangle*)objects[i], hitList);
        }
    }

    for (int i=0; i<hitList.size(); i++){
        if(light->typeName == directionalType){
            // cout << hitList[i]->t << endl;
            return 0;
        }
        else if (light->typeName == pointType){
            float dist1 = glm::distance(pos, hitList[i]->p);
            float dist2 = glm::distance(pos, light->pos);
            if (dist1 < dist2)
                return 0;
        }
    }

    return 1;
}


vec3 calReflection(vec3 d, vec3 n){
    vec3 r = d - 2 * glm::dot(d, n) * n;
    return glm::normalize(r);
}


Ray rayThruPixel(float i, float j){
    float fovyR = glm::radians(fovy);
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::normalize(glm::cross(w, u));

    float ratio = (float) width / height;
    float fovxR = ratio * fovyR;

    float alpha = glm::tan((float)fovxR/2) * ((j-((float)width/2))/((float)width / 2));
    float beta  = glm::tan((float)fovyR/2) * (((float)height/2-i)/((float)height/2));

    vec3 direction = glm::normalize(alpha*u+beta*v-w);
    Ray ray = Ray(eye, direction);

    return ray;
}


Hit * Intersect(Ray ray){
    vector <Hit*> hitList;

    for (int i=0; i<objects.size(); i++){
        if (objects[i]->typeName == sphereType){
            SphereIntersection (ray, (Sphere*)objects[i], hitList);
        }
        else if (objects[i]->typeName == triangleType){
            TriangleIntersection(ray, (Triangle*)objects[i], hitList);
        }
    }
    if (hitList.empty())
        return NULL;

    // find the closest Intersected object
    Hit *h = hitList[0];
    float minDist = glm::distance(eye, hitList[0]->p);

    for (int i=1; i< hitList.size(); i++){
        float dist = glm::distance(eye, hitList[i]->p);
        if (dist < minDist){
            minDist = dist;
            h = hitList[i];
        }
    }
    return h;
}


void SphereIntersection (Ray ray, Sphere *s, vector<Hit*>& hitList){

    vec3 p0 = vec3(ray.p0);
    vec3 p1 = vec3(ray.p1);

    mat4 inv = glm::inverse(s->transform);
    p0 = vectransform2(p0, inv);
    p1 = vectransform3(p1, inv);
    Ray ray2 = Ray(p0, p1);

    vec3 center = s->center;
    float a = glm::dot(p1, p1);
    float b = 2 * glm::dot(p1 , (p0 - center));
    float c = glm::dot((p0 - center),(p0 - center)) - pow(s->radius,2);
    float discriminant = b * b - 4 * a * c;

    float t;
    if (discriminant > 0){
        // cout << discriminant << endl;
        t = (-b + sqrt(discriminant)) / (2 * a);
        if (t > epsilon){
            vec3 point = ray2.rayPath(t);
            vec3 n = point-center;
            n = vectransform3(n, transpose(inv));
            n = glm::normalize(n);

            Hit *h = new Hit();
            h->t = t;
            h->normal = n;
            h->obj = s;
            h->p = vectransform2(point, s->transform);
            hitList.push_back(h);
        }
    }

    if (discriminant > epsilon){
        t = (-b - sqrt(discriminant)) / (2 * a);
        if (t > epsilon){
            vec3 point = ray2.rayPath(t);
            vec3 n = point-center;
            n = vectransform3(n, transpose(inv));
            n = glm::normalize(n);

            Hit *h = new Hit();
            h->t = t;
            h->normal = n;
            h->obj = s;
            h->p = vectransform2(point, s->transform);
            hitList.push_back(h);
        }
    }
}


void TriangleIntersection(Ray ray, Triangle *tri, std::vector<Hit*>& hitList){

    vec3 v1 = tri->v1;
    vec3 v2 = tri->v2;
    vec3 v3 = tri->v3;

    vec3 n = glm::normalize(glm::cross((v3-v1),(v2-v1)));
    if (glm::dot(n, ray.p1) > 0){
        n *= -1;
    }
    if (fabs(glm::dot(ray.p1,n)) < epsilon)
        return; // parallel to the plane

    float t = (glm::dot(v1,n) - glm::dot(ray.p0,n)) / (glm::dot(ray.p1,n));
    if (t < epsilon)
        return; // Intersect behind the camera

    vec3 point = ray.p0 + ray.p1*t;
    vec3 AB = v2 - v1, AC = v3 - v1;
    vec3 PA = v1 - point, PB = v2 - point, PC = v3 - point;

    // Barycentric coordinates
    float Area = glm::length(glm::cross(AB, AC));
    float alpha = glm::length(glm::cross(PB, PC)) / Area;
    float beta = glm::length(glm::cross(PC, PA)) / Area;
    float gamma = glm::length(glm::cross(PA, PB)) / Area;

    if (fabs(alpha + beta + gamma - 1.0f) <= epsilon){
        Hit *h = new Hit();
        h->t = t;
        h->normal = n;
        h->obj = tri;
        h->p = point;
        hitList.push_back(h);
    }
}
