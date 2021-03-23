//
// Created by wyz on 2021/3/10.
//
#include"scene.h"
#include"util.h"
#include<spdlog/spdlog.h>

bool Scene_Impl::add_model(const char *model_path) {
    models.emplace_back(model_path);
    update_bound();
    update_emit_area();
    update_emit_object();
    spdlog::info("model {0} load successfully, total model size: {1}",model_path,models.size());
    return true;
}

Intersection Scene_Impl::get_intersection(const Ray& ray) {
    for(auto& model:models){
        Intersection intersect=model.get_intersection(ray);
        if(intersect.happen)
            return intersect;
    }
    return Intersection();
}

vec4 Scene_Impl::cast_ray(const Ray &ray, int depth) {
    if(depth>3){
//        spdlog::critical("depth>10 {0}",depth);
        return vec4{0.f};
    }
    vec4 direct_light{0.f},indirect_light{0.f},specular_light{0.f};

    if(get_random_float()>=russian_roulette){
        return vec4{0.f};
    }

    Intersection intersect=get_intersection(ray);
    if(intersect.happen){
//        spdlog::info("happen");
        //process happened intersection
        auto hit_obj=intersect.obj;
        auto hit_normal=intersect.normal;
        auto m=intersect.m;
        auto hit_pos=intersect.pos;
        auto tex_coord=intersect.tex_coord;

        /**
         * process with diffuse and specular reflection
         */


        if(m->has_diffuse()){
            /**
             * diffuse should think of direct-light and indirect partial
             */

            /**
             * if scene has area-light, should sample the area-light to direct-light
             */
            if(has_emit_object()){
                Intersection light_inter;
                float light_pdf;
                sample_light(light_inter,light_pdf);
                vec3 light_direction=normalize(light_inter.pos-hit_pos);
                Intersection mid_insect=get_intersection({hit_pos+0.001f*light_direction,light_direction});
                if(mid_insect.distance>=intersect.distance-0.01f){
                    spdlog::info("sample at light");
                    float light_dist2=light_inter.distance*light_inter.distance;
                    vec3 light_emit={light_inter.m->emission[0],light_inter.m->emission[1],light_inter.m->emission[2]};
                    direct_light=vec4{light_emit*m->get_reflect_coefficient(-ray.direction,light_direction,hit_normal)
                                      *dot(light_direction,hit_normal)*dot(-light_direction,light_inter.normal)
                                      /light_dist2/light_pdf/russian_roulette,0.f};
                }

            }

            /**
             * using monte carlo method to get diffuse light color
             * sample method: randomly choose one direction wi~pdf(w)
             */
             vec3 diff_base_color{1.f};
            if(!m->diffuse_texture->is_empty()){
                auto color=m->diffuse_texture->sample(tex_coord.x,tex_coord.y);
//            spdlog::info("color: {0} {1} {2}",color.r,color.g,color.b);
                diff_base_color={color.r/255.f,color.g/255.f,color.b/255.f};
            }

            vec3 diff_reflect_dir=normalize(m->sample(-ray.direction,hit_normal));
            indirect_light=vec4{vec3(cast_ray({hit_pos+0.001f*diff_reflect_dir,diff_reflect_dir},depth+1))*diff_base_color*m->get_reflect_coefficient(-ray.direction,diff_reflect_dir,hit_normal)
                    *dot(diff_reflect_dir,hit_normal)/m->pdf(-ray.direction,diff_reflect_dir,hit_normal)/russian_roulette,0.f};

//        std::cout<<"name: "<<m->name<<std::endl;
//        std::cout<<"diffuse: "<<m->diffuse[0]<<" "<<m->diffuse[1]<<" "<<m->diffuse[2]<<std::endl;
        }

        if(m->has_specular()){
            /**
             * just get the color return with reflect direction by recursive
             */
             auto reflect_direction=normalize(ray.direction+2.f*hit_normal);
//             std::cout<<"pos "<<hit_pos.x<<" "<<hit_pos.y<<" "<<hit_pos.z<<" normal: "<<normal.x<<" "<<normal.y<<" "<<normal.z<<" reflect: "<<reflect_direction.x<<" "<<reflect_direction.y<<" "<<reflect_direction.z<<std::endl;
             specular_light=cast_ray({hit_pos+reflect_direction*0.001f,reflect_direction},depth+1);
//             specular_light.x=pow(specular_light.x,m->shininess);
//             specular_light.y=pow(specular_light.y,m->shininess);
//             specular_light.z=pow(specular_light.z,m->shininess);
        }
        /**
         * assert one ray just intersect with one object, so once find just return result
         */

        return {(direct_light.x+indirect_light.x)+specular_light.x*m->specular[0],
                    (direct_light.y+indirect_light.y)+specular_light.y*m->specular[1],
                    (direct_light.z+indirect_light.z)+specular_light.z*m->specular[2],0.f};
    }


    /**
     * if ray not intersect with the scene's all objects, then think of environment map
     */
    if(has_env_map()){
        spdlog::info("has env map");
        auto env_color=env_map->sample(ray.direction.x,ray.direction.y,ray.direction.z);
//        if(depth>0)
//        spdlog::info("env_color: {0} {1} {2}",env_color.r,env_color.g,env_color.b);
//        gamma correction but no tone mapping
//        auto r=pow(env_color.r,0.454545f);
//        auto g=pow(env_color.g,0.454545f);
//        auto b=pow(env_color.b,0.454545f);
//        if(depth>0)
//        spdlog::info("after gamma env_color: {0} {1} {2}",r,g,b);
//        return {r,g,b,0.f};
        return {pow(env_color.r,0.454545f),pow(env_color.g,0.454545f),pow(env_color.b,0.454545f),0.f};
    }
    else{
        return {0.f,0.f,0.f,0.f};
    }
}

void Scene_Impl::update_bound(){
    for(auto& model:models){
        this->aabb.union_(model.get_bound());
    }
    std::cout<<"[scene boundary]: "<<aabb<<std::endl;
}

void Scene_Impl::update_emit_area() {
    this->total_emit_object_area=0.f;
    for(auto& model:models){
        this->total_emit_object_area+=model.get_emit_area();
    }
    std::cout<<"[scene total emit objects' area]: "<<total_emit_object_area<<std::endl;
}

void Scene_Impl::update_emit_object() {
    this->emit_objects.clear();
    for(auto& model:models){
        auto obj=model.get_emit_object();
        emit_objects.insert(emit_objects.cend(),obj.cbegin(),obj.cend());
    }
    std::cout<<"[scene total emit objects' size]: "<<emit_objects.size()<<std::endl;
}

void Scene_Impl::load_environment_map(const char *path) {

    env_map=make_unique<Texture<float>>("env_map");
    env_map->load_texture(path);
}

bool Scene_Impl::has_env_map() const {
    return env_map && !env_map->is_empty();
}

bool Scene_Impl::has_emit_object() const {
    return total_emit_object_area>0.f;
}

void Scene_Impl::sample_light(Intersection &intersection, float &pdf) {
    float p_area=get_random_float()*total_emit_object_area;
    float emit_area=0.f;
    for(auto& it:emit_objects){
        emit_area+=it->get_area();
        if(emit_area>=p_area){
            it->sample(intersection,pdf);
            return;
        }
    }
}





