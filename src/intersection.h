//
// Created by wyz on 2021/3/15.
//

#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include"material.h"
class Object;
class Intersection{
public:
    Intersection()
    :happen(false),distance(std::numeric_limits<double>::max()),obj(nullptr),m(nullptr),normal({}),tex_coord({}),pos({})
    {

    }
    bool happen;
    vec3 normal;//normal for triangle surface, can improve by calculate with three vertices' normal
    vec2 tex_coord;
    vec3 pos;
    double distance;
    Object* obj;
    shared_ptr<Material> m;
};



#endif //RAYTRACER_INTERSECTION_H
