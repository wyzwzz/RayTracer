//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include<memory>
using namespace std;
class Renderer_Impl;
class Renderer{
public:
    Renderer()=default;
    ~Renderer();
private:
    unique_ptr<Renderer_Impl> impl;
};

#endif //RAYTRACER_RENDERER_H
