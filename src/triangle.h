//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H
#include <array>
#include<vector>
#include<glm/glm.hpp>
using namespace glm;
using namespace std;
struct Vertex{
    vec3 pos;
    vec3 normal;
    vec3 tex_coord;
};
class Triangle{
public:
    array<Vertex,3> vertices;
};

class TriangleMesh{
public:
    void addTriangles();
    void addTriangle();
public:

    vector<Triangle> triangles;
};







#endif //RAYTRACER_TRIANGLE_H
