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

    unique_ptr<Texture> diffuse_texture;
};



#endif //RAYTRACER_MATERIAL_H
