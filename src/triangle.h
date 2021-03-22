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
//        std::cout<<"test triangle: "<<vertices[0].pos.x<<" "<<vertices[0].pos.y<<" "<<vertices[0].pos.z<<" "
//                <<vertices[1].pos.x<<" "<<vertices[1].pos.y<<" "<<vertices[1].pos.z<<" "
//                <<vertices[2].pos.x<<" "<<vertices[2].pos.y<<" "<<vertices[2].pos.z<<std::endl;
//        std::cout<<"ray: "<<ray.origin.x<<" "<<ray.origin.y<<" "<<ray.origin.z<<" "
//                          <<ray.direction.x<<" "<<ray.direction.y<<" "<<ray.direction.z<<std::endl;
        Intersection intersection;
//        std::cout<<"test intersect between triangle and ray"<<std::endl;

        //if hit the front face
        if(dot(ray.direction,normal)>0){
            return intersection;
        }

        vec3 s=ray.origin-vertices[0].pos;
        vec3 s1=cross(ray.direction,e2);
        double s1e1=dot(s1,e1);
        if(fabs(s1e1)<EPSILON){
//            std::cout<<"triangle not happend with ray 000"<<std::endl;
            return intersection;
        }
        double s1e1_inv=1.0/s1e1;
        double b1=dot(s1,s)*s1e1_inv;
        if(b1<0 || b1>1.0){
//            std::cout<<"b1 "<<b1<<std::endl;
//            std::cout<<"triangle not happend with ray 111"<<std::endl;
            return intersection;
        }

        vec3 s2=cross(s,e1);
        double b2=dot(s2,ray.direction)*s1e1_inv;
        if(b2<0 || b1+b2>1.0){
//            std::cout<<"b1 "<<b1<<" b2 "<<b2<<std::endl;
//            std::cout<<"triangle not happend with ray 222"<<std::endl;
            return intersection;
        }
        double t=dot(s2,e2)*s1e1_inv;
//        {1.0-b1-b2,b1,b2};
        intersection.happen=true;
        intersection.normal=vertices[0].normal*float(1.0-b1-b2)+vertices[1].normal*(float)b1+vertices[2].normal*(float)b2;
        intersection.pos=vertices[0].pos*(float)(1.0-b1-b2)+vertices[1].pos*(float)b1+vertices[2].pos*(float)b2;
        intersection.tex_coord=vertices[0].tex_coord*(float)(1.0-b1-b2)+vertices[1].tex_coord*(float)b1+vertices[2].tex_coord*(float)b2;
        intersection.m=m;
        intersection.obj=this;
        intersection.distance=t;

        return intersection;
    }
    Color<uint8_t> get_diffuse_color(const vec3& b) override{
        vec2 tex_sample_coord=vertices[0].tex_coord*b.x+vertices[1].tex_coord*b.y+vertices[2].tex_coord*b.z;
        return m->diffuse_texture->sample(tex_sample_coord.x,tex_sample_coord.y);
    }
    void sample(Intersection& intersection,float& pdf) override{

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

    void build_bvh_tree(){
        vector<Object*> objects;
        objects.reserve(triangles.size());
        for(auto& it:triangles){
            objects.emplace_back(&it);
            aabb.union_(it.get_aabb());
        }
        bvh_tree=make_unique<BVHTree>(objects);
    }
    Intersection get_intersection(const Ray& ray){
        return bvh_tree->intersect(ray);
    }
    AABB get_bound() const{
        return aabb;
    }
public:
    AABB aabb;
    vector<Triangle> triangles;

private:
    unique_ptr<BVHTree> bvh_tree;
};







#endif //RAYTRACER_TRIANGLE_H
