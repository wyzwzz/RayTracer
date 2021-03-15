//
// Created by wyz on 2021/3/10.
//
#include<spdlog/spdlog.h>
#include <Renderer.h>

#include"renderer.h"


void Renderer_Impl::render_frame(const Scene &scene, const Camera &camera,int spp)
{
    framebuffer.resize(width*height*3);

    for(uint32_t i=0;i<width;i++){
        for(uint32_t j=0;j<height;j++){

        }
    }


}


void Renderer_Impl::setup_window(int width, int height) {
    this->width=width;
    this->height=height;
}

auto Renderer_Impl::get_frame() -> Image && {
    Image image;
    image.width=width;
    image.height=height;
    image.nchannels=3;
    spdlog::info("framebuffer: {0:d}",(uint64_t )framebuffer.data());
    image.data=move(framebuffer);
    spdlog::info("image data: {0:d}",(uint64_t )image.data.data());
    spdlog::info("image data size: {0}",image.data.size());
    spdlog::info("framebuffer size: {0}",framebuffer.size());
    return move(image);
}


void Renderer::render_frame(const Scene &scene, const Camera &camera,int spp) {
    impl->render_frame(scene,camera,spp);
}
void Renderer::setup_window(int width, int height) {
    impl->setup_window(width,height);
}
auto Renderer::get_frame() ->  Image && {
    return forward<Image>(impl->get_frame());
}
Renderer::~Renderer() {

}

Renderer::Renderer() {
    impl=make_unique<Renderer_Impl>();
}
