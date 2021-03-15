//
// Created by wyz on 2021/3/13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H
#include"aabb.h"
#include"intersection.h"
class Color;
class Object: public enable_shared_from_this<Object>{
public:
    Object()=default;
    ~Object()=default;
    virtual const AABB& get_aabb() const=0;
    virtual Intersection intersect(const Ray& ray)=0;
    virtual Color getDiffuseColor(const vec3&)=0;
};
#endif //RAYTRACER_OBJECT_H
