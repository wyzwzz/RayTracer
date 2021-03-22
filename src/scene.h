//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_SCENE_IMPL_H
#define RAYTRACER_SCENE_IMPL_H
#include <Scene.h>
#include"model.h"

class Scene_Impl{
public:
    Scene_Impl()
    :russian_roulette(0.9f)
    {};
    bool add_model(const char* model_path);
    void add_model(Model&&);
    void load_environment_map(const char* path);
    vec4 cast_ray(const Ray& ray,int depth);
    void update_bound();

private:
    bool has_env_map() const;
private:
    unique_ptr<Texture<float>> env_map;
    vector<Model> models;
    float russian_roulette;
    AABB aabb;
};

Scene::~Scene() {

}

bool Scene::add_model(const char *model_path) {
    return impl->add_model(model_path);
}

Scene::Scene() {
    impl=make_unique<Scene_Impl>();
}

vec4 Scene::cast_ray(const Ray ray, int depth) const{
    return impl->cast_ray(ray,depth);
}

void Scene::load_environment_map(const char *path) {
    impl->load_environment_map(path);
}


#endif //RAYTRACER_SCENE_IMPL_H
