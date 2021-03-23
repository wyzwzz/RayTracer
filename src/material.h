//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H
#include<string>
#include<array>
#include<memory>
#include"texture.h"
#include"config.h"
#include<glm/glm.hpp>
#include "util.h"
using namespace glm;
using namespace std;
class Material{
public:
    Material()=default;
    bool has_emission() const{
        auto norm=emission[0]*emission[0]+emission[1]*emission[1]+emission[2]*emission[2];
        if(norm>EPSILON){
            return true;
        }
        else{
            return false;
        }
    }
    bool has_diffuse() const{
        auto norm=diffuse[0]*diffuse[0]+diffuse[1]*diffuse[1]+diffuse[2]*diffuse[2];
        if(norm>EPSILON){
            return true;
        }
        else{
            return false;
        }
    }
    bool has_specular() const{
        auto norm=specular[0]*specular[0]+specular[1]*specular[1]+specular[2]*specular[2];
        if(norm>EPSILON){
            return true;
        }
        else{
            return false;
        }
    }
    vec3 get_reflect_coefficient(const vec3& in_dir,const vec3& out_dir,const vec3& normal) const{
        float cosalpha=dot(normal,out_dir);
        if(cosalpha>0.f){
            return vec3(diffuse[0],diffuse[1],diffuse[2])/M_PI;
        }
        else{
            return vec3{0.f};
        }
    }
    float pdf(const vec3& in_dir,const vec3& out_dir,const vec3& normal){

        return 0.f;
    }
    vec3 sample(const vec3& in_dir,const vec3& normal){
        float x1 = get_random_float(), x2 = get_random_float();
        float z = std::fabs(1.0f - 2.0f * x1);
        float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x2;
        vec3 out_dir(r*std::cos(phi), r*std::sin(phi), z);
        return adjust_by_normal(out_dir, normal);
    }
    vec3 adjust_by_normal(const vec3& dir,const vec3& normal){
        vec3 B, C;
        if (std::fabs(normal.x) > std::fabs(normal.y)){
            float inv_len = 1.0f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
            C = vec3(normal.z * inv_len, 0.0f, -normal.x *inv_len);
        }
        else {
            float inv_len = 1.0f / std::sqrt(normal.y * normal.y + normal.z * normal.z);
            C = vec3(0.0f, normal.z * inv_len, -normal.y *inv_len);
        }
        B = cross(C, normal);
        return dir.x * B + dir.y * C + dir.z * normal;
    }
public:
    string name;

    array<float,3> ambient;
    array<float,3> diffuse;
    array<float,3> specular;
    array<float,3> transmittance;
    array<float,3> emission;
    float shininess;

    string ambient_texname;
    string diffuse_texname;
    string specular_texname;
    string specular_highlight_texname;

    unique_ptr<Texture<uint8_t>> diffuse_texture;
};



#endif //RAYTRACER_MATERIAL_H
