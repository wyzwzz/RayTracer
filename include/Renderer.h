//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include<memory>
#include<Camera.h>
#include<Scene.h>
#include<Image.h>
using namespace std;
class Renderer_Impl;
class Renderer{
public:
    Renderer();
    void setup_window(int width,int height);
    void render_frame(const Scene& scene,const Camera& camera,int spp=16);
    auto get_frame()-> Image;
    ~Renderer();
private:
    unique_ptr<Renderer_Impl> impl;
};

#endif //RAYTRACER_RENDERER_H
