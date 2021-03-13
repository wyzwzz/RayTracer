//
// Created by wyz on 2021/3/13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H
#include"aabb.h"
class Object{
public:
    Object()=default;
    ~Object()=default;
    virtual const AABB& get_aabb() const=0;
};
#endif //RAYTRACER_OBJECT_H
