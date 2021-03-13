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
using namespace glm;
using namespace std;
struct Vertex{
    vec3 pos;
    vec3 normal;
    vec3 tex_coord;
};
class Triangle: public Object{
public:
    Triangle()=default;
    void init(){
        vec3 n=cross(vertices[1].pos-vertices[0].pos,vertices[2].pos-vertices[0].pos);
        normal=normalize(n);
        area=length(n)*0.5f;
        aabb.union_(vertices[0].pos);
        aabb.union_(vertices[1].pos);
        aabb.union_(vertices[2].pos);
    }
    const AABB& get_aabb() const override{
        return this->aabb;
    }
public:
    vec3 normal;
    float area;
    AABB aabb;

    array<Vertex,3> vertices;

    shared_ptr<Material> m;
};

class TriangleMesh: public Object{
public:
    TriangleMesh(){

    }
    void addTriangles(const vector<Triangle>& triangles){
        this->triangles.insert(this->triangles.cend(),triangles.cbegin(),triangles.cend());
    }
    void addTriangle(Triangle&& triangle){
        triangles.emplace_back(triangle);
    }
    const AABB& get_aabb() const override{
        return aabb;
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
public:
    AABB aabb;
    vector<Triangle> triangles;

private:
    unique_ptr<BVHTree> bvh_tree;
};







#endif //RAYTRACER_TRIANGLE_H
