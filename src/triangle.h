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
using namespace glm;
using namespace std;
struct Vertex{
    vec3 pos;
    vec3 normal;
    vec3 tex_coord;
};
class Triangle{
public:
    Triangle()=default;

public:
    array<Vertex,3> vertices;

    shared_ptr<Material> m;
};

class TriangleMesh{
public:
    void addTriangles(const vector<Triangle>& triangles){
        this->triangles.insert(this->triangles.cend(),triangles.cbegin(),triangles.cend());
    }
    void addTriangle(Triangle&& triangle){
        triangles.emplace_back(triangle);
    }
public:

    vector<Triangle> triangles;
};







#endif //RAYTRACER_TRIANGLE_H
