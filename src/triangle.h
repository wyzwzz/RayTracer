//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H
#include <array>
#include<vector>
#include<glm/glm.hpp>
#include<memory>
#include"material.h"
#include "object.h"
#include "bvhtree.h"
#include "config.h"
#include "texture.h"
using namespace glm;
using namespace std;
struct Vertex{
    vec3 pos;
    vec3 normal;
    vec2 tex_coord;
};
class Triangle: public Object{
public:
    Triangle()=default;
    void init(){
        e1=vertices[1].pos-vertices[0].pos;
        e2=vertices[2].pos-vertices[0].pos;
        vec3 n=cross(e1,e2);
        normal=normalize(n);
        area=length(n)*0.5f;
        aabb.union_(vertices[0].pos);
        aabb.union_(vertices[1].pos);
        aabb.union_(vertices[2].pos);
    }
    const AABB& get_aabb() const override{
        return this->aabb;
    }
    Intersection intersect(const Ray& ray) override{
        Intersection intersection;

        //if hit the front face or
        if(dot(ray.direction,normal)>0){
            return intersection;
        }

        vec3 s=ray.origin-vertices[0].pos;
        vec3 s1=cross(ray.direction,e2);
        double s1e1=dot(s1,e1);
        if(fabs(s1e1)<EPSILON){
            return intersection;
        }
        double s1e1_inv=1.0/s1e1;
        double b1=dot(s1,s)*s1e1;
        if(b1<0 || b1>1){
            return intersection;
        }

        vec3 s2=cross(s,e1);
        double b2=dot(s2,ray.direction)*s1e1_inv;
        if(b2<0 || b1+b2>1){
            return intersection;
        }
        double t=dot(s2,e2)*s1e1_inv;

        intersection.happen=true;
        intersection.normal=normal;
        intersection.barycentric={1.0-b1-b2,b1,b2};
        intersection.m=m;
        intersection.obj=this->shared_from_this();
        intersection.distance=t;

        return intersection;
    }
    Color getDiffuseColor(const vec3& b) override{
        vec2 tex_sample_coord=vertices[0].tex_coord*b.x+vertices[1].tex_coord*b.y+vertices[2].tex_coord*b.z;
        return m->diffuse_texture->sample(tex_sample_coord.x,tex_sample_coord.y);
    }

public:
    vec3 normal;
    vec3 e1,e2;
    float area;
    AABB aabb;

    array<Vertex,3> vertices;

    shared_ptr<Material> m;
};

class TriangleMesh{
public:
    TriangleMesh(){

    }
    void addTriangles(const vector<Triangle>& triangles){
        this->triangles.insert(this->triangles.cend(),triangles.cbegin(),triangles.cend());
    }
    void addTriangle(Triangle&& triangle){
        triangles.emplace_back(triangle);
    }
//    const AABB& get_aabb() const {
//        return aabb;
//    }
//    Intersection intersect(const Ray& ray) {
//        return {};
//    }
    void build_bvh_tree(){
        vector<Object*> objects;
        objects.reserve(triangles.size());
        for(auto& it:triangles){
            objects.emplace_back(&it);
            aabb.union_(it.get_aabb());
        }
        bvh_tree=make_unique<BVHTree>(objects);
    }
public:
    AABB aabb;
    vector<Triangle> triangles;

private:
    unique_ptr<BVHTree> bvh_tree;
};







#endif //RAYTRACER_TRIANGLE_H
