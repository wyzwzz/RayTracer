//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_BVH_H
#define RAYTRACER_BVH_H
#include <vector>
#include"object.h"
#include"aabb.h"
#include"ray.h"
#include"intersection.h"
using namespace std;
/**
 * one BVHNode only contain one object
 */
class BVHNode{
public:
    AABB aabb;
    BVHNode* left;
    BVHNode* right;
    Object* object;
public:
    BVHNode()
    :aabb(AABB()),left(nullptr),right(nullptr),object(nullptr)
    {

    }
};


class BVHTree{
public:
    enum class SplitMethod{NAIVE};
    BVHTree(vector<Object*>,SplitMethod split_method=SplitMethod::NAIVE);
    BVHNode* recursive_build(vector<Object*>&);

    Intersection intersect(const Ray& ray) const;

    Intersection intersect(const BVHNode* node,const Ray& ray) const;





public:

    BVHNode* root;
    SplitMethod split_method;
    const int max_primitives_in_node=1;
    vector<Object*> primitives;
};


#endif //RAYTRACER_BVH_H
