#include "variables.h"

Ray rayThruPixel(int i, int j){
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
    hitList.clear();

    for (int i=0; i<objects.size(); i++){
        if (objects[i]->typeName == sphereType){
            SphereIntersection (ray, (Sphere*)objects[i]);
        }
        else if (objects[i]->typeName == triangleType){
            TriangleIntersection(ray, (Triangle*)objects[i]);
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


void SphereIntersection (Ray ray, Sphere *s){

    vec3 center = s->center;
    float a = glm::dot(ray.p1,ray.p1);
    float b = 2 * glm::dot(ray.p1 , (ray.p0 - center));
    float c = glm::dot((ray.p0 - center),(ray.p0 - center)) - pow(s->radius,2);
    float discriminant = pow(b,2) - 4 * a * c;


    if (discriminant >= 0){
        float t = (-b + sqrt(discriminant)) / (2 * a);
        if (t > 0){
            vec3 point = ray.rayPath(t);
            vec3 n = glm::normalize(point-center);

            Hit *h = new Hit();
            h->t = t;
            h->normal = n;
            h->obj = s;
            h->p = point;
            hitList.push_back(h);
        }

        if (discriminant > 0){
            t = (-b - sqrt(discriminant)) / (2 * a);
            if (t > 0){
                vec3 point = ray.rayPath(t);
                vec3 n = glm::normalize(point-center);

                Hit *h = new Hit();
                h->t = t;
                h->normal = n;
                h->obj = s;
                h->p = point;
                hitList.push_back(h);
            }
        }
    }
}


void TriangleIntersection(Ray ray, Triangle *tri) {

    vec3 v1 = tri->v1;
    vec3 v2 = tri->v2;
    vec3 v3 = tri->v3;

    vec3 n = glm::normalize(glm::cross((v3-v1),(v2-v1)));
    if (glm::dot(ray.p1,n) == 0)
        return; // parallel to the plane

    float t = (glm::dot(v1,n) - glm::dot(ray.p0,n)) / (glm::dot(ray.p1,n));
    if (t <= 0)
        return; // Intersect behind the camera

    vec3 point = ray.p0 + ray.p1*t;
    vec3 AB = v2 - v1, AC = v3 - v1;
    vec3 PA = v1 - point, PB = v2 - point, PC = v3 - point;

    // Barycentric coordinates
    float Area = glm::length(glm::cross(AB, AC));
    float alpha = glm::length(glm::cross(PB, PC)) / Area;
    float beta = glm::length(glm::cross(PC, PA)) / Area;
    float gamma = glm::length(glm::cross(PA, PB)) / Area;

    if (alpha >= 0 && beta >= 0 && gamma >= 0 && fabs(alpha + beta + gamma - 1) <= 0.001){
        Hit *h = new Hit();
        h->t = t;
        h->normal = n;
        h->obj = tri;
        h->p = point;
        hitList.push_back(h);
    }
}
