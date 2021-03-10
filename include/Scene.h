//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include<memory>
using namespace std;
class Scene_Impl;
class Scene{
public:
    Scene();
    ~Scene();
private:
    unique_ptr<Scene_Impl> impl;
};


#endif //RAYTRACER_SCENE_H
