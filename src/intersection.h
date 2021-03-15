//
// Created by wyz on 2021/3/15.
//

#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include"material.h"
class Object;
class Intersection{
public:
    Intersection(){

    }
    bool happen;
    vec3 normal;//normal for triangle surface, can improve by calculate with three vertices' normal
    vec3 barycentric;
    double distance;
    shared_ptr<Object> obj;
    shared_ptr<Material> m;
};



#endif //RAYTRACER_INTERSECTION_H
