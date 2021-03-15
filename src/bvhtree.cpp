//
// Created by wyz on 2021/3/10.
//
#include<algorithm>
#include "bvhtree.h"
#include "util.h"
BVHTree::BVHTree(vector<Object *> objects, BVHTree::SplitMethod split_method)
:primitives(move(objects)),split_method(split_method),root(nullptr)
{
    START_TIMER

    root=recursive_build(primitives);

    END_TIMER("bvh tree build")
}

BVHNode *BVHTree::recursive_build(vector<Object *>& objects) {
    auto node=new BVHNode();

    if(objects.size()==1){
        node->aabb=objects[0]->get_aabb();
        node->left= nullptr;
        node->right= nullptr;
        node->object=objects[0];

    }
    else if(objects.size()==2){
        vector<Object*> left={objects[0]};
        vector<Object*> right={objects[1]};
        node->left=recursive_build(left);
        node->right=recursive_build(right);
        node->aabb=Union(node->left->aabb,node->right->aabb);

    }
    else{
        AABB center_pos_aabb;
        for(size_t i=0;i<objects.size();i++){
            center_pos_aabb.union_(objects[i]->get_aabb().center_pos());
        }
        int dim=center_pos_aabb.max_extent();
        switch (dim) {
            case 0:
                sort(objects.begin(),objects.end(),[](auto o1,auto o2){
                    return o1->get_aabb().center_pos().x < o2->get_aabb().center_pos().x;
                });break;
            case 1:
                sort(objects.begin(),objects.end(),[](auto o1,auto o2){
                    return o1->get_aabb().center_pos().y < o2->get_aabb().center_pos().y;
                });break;
            case 2:
                sort(objects.begin(),objects.end(),[](auto o1,auto o2){
                    return o1->get_aabb().center_pos().z < o2->get_aabb().center_pos().z;
                });break;
        }
        auto begin = objects.begin();
        auto mid = objects.begin() + (objects.size() / 2);
        auto end = objects.end();

        auto left_objects= std::vector<Object*>(begin, mid);
        auto right_objects = std::vector<Object*>(mid, end);

        assert(objects.size() == (left_objects.size() + right_objects.size()));

        node->left=recursive_build(left_objects);
        node->right=recursive_build(right_objects);

        node->aabb=Union(node->left->aabb,node->right->aabb);
    }

    return node;
}

Intersection BVHTree::intersect(const Ray &ray) const {
    if(root){
        return intersect(root,ray);
    }
    else{
        return {};
    }
}

Intersection BVHTree::intersect(const BVHNode *node, const Ray &ray) const {

    if(!node){
        return {};
    }
    else if(node->left || node->right){
        Intersection intersection0=intersect(node->left,ray);
        Intersection intersection1=intersect(node->right,ray);
        return intersection0.distance<intersection1.distance?intersection0:intersection1;
    }
    else{
        if(node->aabb.intersect(ray)){
            return node->object->intersect(ray);
        }
        else{
            return {};
        }
    }
}
