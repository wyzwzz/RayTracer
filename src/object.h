//
// Created by wyz on 2021/3/13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H
#include"aabb.h"
#include"intersection.h"

template<class T>
class Color;
class Object{
public:
    Object()=default;
    ~Object()=default;
    virtual const AABB& get_aabb() const=0;
    virtual Intersection intersect(const Ray& ray)=0;
    virtual Color<uint8_t> get_diffuse_color(const vec3&)=0;
    virtual void sample(Intersection&,float& pdf)=0;
    virtual float get_area() const=0;
};
#endif //RAYTRACER_OBJECT_H
