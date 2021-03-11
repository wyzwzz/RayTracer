//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_SCENE_IMPL_H
#define RAYTRACER_SCENE_IMPL_H
#include <Scene.h>
#include"model.h"
class Scene_Impl{
public:
    Scene_Impl()=default;
    bool add_model(const char* model_path);
    void add_model(Model&&);

private:
    vector<Model> models;
};

Scene::~Scene() {

}

bool Scene::add_model(const char *model_path) {
    return impl->add_model(model_path);
}

Scene::Scene() {
    impl=make_unique<Scene_Impl>();
}


#endif //RAYTRACER_SCENE_IMPL_H
