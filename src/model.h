//
// Created by wyz on 2021/3/11.
//

#ifndef RAYTRACER_MODEL_H
#define RAYTRACER_MODEL_H

#include"triangle.h"

class Model{
public:
    explicit Model(const char* model_path);
    void load(const char* model_path);
    Intersection get_intersection(const Ray& ray);
    AABB get_bound() const;
    float get_emit_area() const;
    vector<Object*> get_emit_object() ;
public:
    string name;
private:
    vector<TriangleMesh> meshes;

};
#endif //RAYTRACER_MODEL_H
