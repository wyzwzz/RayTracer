//
// Created by wyz on 2021/3/10.
//
#include"scene.h"
#include<spdlog/spdlog.h>

bool Scene_Impl::add_model(const char *model_path) {
    models.emplace_back(model_path);
    update_bound();
    spdlog::info("model {0} load successfully, total model size: {1}",model_path,models.size());
    return true;
}

vec4 Scene_Impl::cast_ray(const Ray &ray, int depth) {
    vec4 direct_light{0.f},indirect_light{0.f};
//    bool intersected=false;
    for(auto& model:models){
//        spdlog::info("model:{0}",model.name);
        Intersection intersect=model.get_intersection(ray);
        if(!intersect.happen){
            continue;
        }

//        spdlog::info("happen");
        //process happened intersection
        auto hit_obj=intersect.obj;
        auto normal=intersect.normal;
        auto m=intersect.m;

        auto tex_coord=intersect.tex_coord;

        if(!m->diffuse_texture->is_empty()){
            auto color=m->diffuse_texture->sample(tex_coord.x,tex_coord.y);
//            spdlog::info("color: {0} {1} {2}",color.r,color.g,color.b);
            direct_light={m->diffuse[0]*color.r/255.f,m->diffuse[1]*color.g/255.f,m->diffuse[2]*color.b/255.f,0.f};
            return direct_light;
        }
//        std::cout<<"name: "<<m->name<<std::endl;
//        std::cout<<"diffuse: "<<m->diffuse[0]<<" "<<m->diffuse[1]<<" "<<m->diffuse[2]<<std::endl;
        direct_light={m->diffuse[0],m->diffuse[1],m->diffuse[2],0.f};
        return direct_light;

    }
    /**
     * if ray not intersect with the scene's all objects, then think of environment map
     */
    if(has_env_map()){
        auto env_color=env_map->sample(ray.direction.x,ray.direction.y,ray.direction.z);
//        spdlog::info("env_color: {0} {1} {2}",env_color.r,env_color.g,env_color.b);
        direct_light={env_color.r,env_color.g,env_color.b,0.f};
    }



//    spdlog::info("no happened");
    //all meshes not intersect with the ray
    return direct_light;
}

void Scene_Impl::update_bound(){
    for(auto& model:models){
        this->aabb.union_(model.get_bound());
    }
    std::cout<<"[scene boundary]: "<<aabb<<std::endl;
}

void Scene_Impl::load_environment_map(const char *path) {

    env_map=make_unique<Texture<float>>("env_map");
    env_map->load_texture(path);
}

bool Scene_Impl::has_env_map() const {
    return env_map && !env_map->is_empty();
}
