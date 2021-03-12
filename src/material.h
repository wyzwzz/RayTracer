//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H
#include<string>
#include<array>
#include<memory>
#include"texture.h"
using namespace std;
class Material{
public:
    Material()=default;
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

    unique_ptr<Texture> texture;
};



#endif //RAYTRACER_MATERIAL_H
