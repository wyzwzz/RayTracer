//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_RENDERER_IMPL_H
#define RAYTRACER_RENDERER_IMPL_H
#include<Renderer.h>
#include<Scene.h>
#include<Camera.h>
#include<vector>
class Renderer_Impl{
    int width,height;
    vector<uint8_t> framebuffer;
public:
    Renderer_Impl():width(1200),height(900){}
    void setup_window(int width,int height);
    void render_frame(const Scene& scene,const Camera& camera,int spp);
    auto get_frame()->Image&&;
};






#endif //RAYTRACER_RENDERER_IMPL_H
