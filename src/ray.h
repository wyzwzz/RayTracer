//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H
#include<glm/glm.hpp>
#include<limits>
using namespace glm;
class Ray{
public:
    vec3 origin;
    vec3 direction;//must be normalized
    vec3 inv_direction;
    double t;
    double t_min,t_max;
    Ray(const vec3& origin,const vec3& direction,double t=0.0):
    origin(origin),direction(direction),inv_direction({1.f/direction.x,1.f/direction.y,1.f/direction.z}),t(t),t_min(0.0),t_max(std::numeric_limits<double>::max())
    {}

};

#endif //RAYTRACER_RAY_H
