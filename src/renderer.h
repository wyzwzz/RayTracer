//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_RENDERER_IMPL_H
#define RAYTRACER_RENDERER_IMPL_H
#include<Renderer.h>
#include<Scene.h>
class Renderer_Impl{
    void render_frame(const Scene& scene);
};


Renderer::~Renderer() {

}

#endif //RAYTRACER_RENDERER_IMPL_H
