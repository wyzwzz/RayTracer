//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
#include<memory>
#include<glm/glm.hpp>
using namespace std;
using namespace glm;
class Scene_Impl;
class Ray;
class Scene{
public:
    Scene();
    ~Scene();
    bool add_model(const char* model_path);
    vec4 cast_ray(const Ray ray,int depth) const;
private:
    unique_ptr<Scene_Impl> impl;
};


#endif //RAYTRACER_SCENE_H
