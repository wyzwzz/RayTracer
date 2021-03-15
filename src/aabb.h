//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H
#include<glm/glm.hpp>
#include<limits>
#include"ray.h"
using namespace glm;
using namespace std;
class AABB{
public:
    AABB(){
        double min_num = std::numeric_limits<double>::lowest();
        double max_num = std::numeric_limits<double>::max();
        max_p = vec3(min_num, min_num, min_num);
        min_p = vec3(max_num, max_num, max_num);
    }
    AABB(const AABB& aabb){
        this->min_p=aabb.min_p;
        this->max_p=aabb.max_p;
    }
    AABB(const vec3& p1,const vec3& p2){
        min_p = vec3(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
        max_p = vec3(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
    }
    void union_(const AABB& aabb){
        this->min_p={
                fmin(min_p.x,aabb.min_p.x),
                fmin(min_p.y,aabb.min_p.y),
                fmin(min_p.z,aabb.min_p.z)
        };
        this->max_p={
                fmax(max_p.x,aabb.max_p.x),
                fmax(max_p.y,aabb.max_p.y),
                fmax(max_p.z,aabb.max_p.z)
        };
    }
    void union_(const vec3& p){
        this->min_p={
                fmin(min_p.x,p.x),
                fmin(min_p.y,p.y),
                fmin(min_p.z,p.z)
        };
        this->max_p={
                fmax(max_p.x,p.x),
                fmax(max_p.y,p.y),
                fmax(max_p.z,p.z)
        };
    }
public:
    vec3 diagonal() const{
        return max_p-min_p;
    }
    int max_extent() const{
        vec3 d=this->diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }
    double surface_area() const
    {
        vec3 d = diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    vec3 center_pos() const {
        return 0.5f*(min_p+max_p);
    }

    bool intersect(const Ray& ray) const{
        float t_min_x=(min_p.x-ray.origin.x)*ray.inv_direction.x;
        float t_max_x=(max_p.x-ray.origin.x)*ray.inv_direction.x;
        if(ray.direction.x<0){
            swap(t_min_x,t_max_x);
        }
        float t_min_y=(min_p.y-ray.origin.y)*ray.inv_direction.y;
        float t_max_y=(max_p.y-ray.origin.y)*ray.inv_direction.y;
        if(ray.direction.y<0){
            swap(t_min_y,t_max_y);
        }
        float t_min_z=(min_p.z-ray.origin.z)*ray.inv_direction.z;
        float t_max_z=(max_p.z-ray.origin.z)*ray.inv_direction.z;
        if(ray.direction.z<0){
            swap(t_min_z,t_max_z);
        }
        float enter_t=std::max(t_min_x,std::max(t_min_y,t_min_z));
        float exit_t=std::min(t_max_x,std::min(t_max_y,t_max_z));
        if(exit_t > 0 && enter_t<=exit_t) return true;
        else return false;
    }

public:

    vec3 min_p,max_p;
};

inline AABB Union(const AABB& aabb1,const AABB& aabb2)
{
    AABB aabb;
    aabb.min_p={
            fmin(aabb1.min_p.x,aabb2.min_p.x),
            fmin(aabb1.min_p.y,aabb2.min_p.y),
            fmin(aabb1.min_p.z,aabb2.min_p.z)
    };
    aabb.max_p={
            fmax(aabb1.max_p.x,aabb2.max_p.x),
            fmax(aabb1.max_p.y,aabb2.max_p.y),
            fmax(aabb1.max_p.z,aabb2.max_p.z)
    };
    return aabb;
}

#endif //RAYTRACER_AABB_H
