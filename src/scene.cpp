//
// Created by wyz on 2021/3/10.
//


#include"scene.h"
#include<spdlog/spdlog.h>

bool Scene_Impl::add_model(const char *model_path) {
    models.emplace_back(model_path);
    spdlog::info("model {0} load successfully, total model size: {1}",model_path,models.size());
    return true;
}

vec4 Scene_Impl::cast_ray(const Ray &ray, int depth) {
    vec4 direct_light{0.f},indirect_light{0.f};
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
        }
//        std::cout<<"name: "<<m->name<<std::endl;
//        std::cout<<"diffuse: "<<m->diffuse[0]<<" "<<m->diffuse[1]<<" "<<m->diffuse[2]<<std::endl;
        direct_light={m->diffuse[0],m->diffuse[1],m->diffuse[2],0.f};
        return direct_light;
    }


//    spdlog::info("no happened");
    //all meshes not intersect with the ray
    return glm::vec4(0.f,0.f,0.f,0.f);
}
