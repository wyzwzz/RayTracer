//
// Created by wyz on 2021/3/10.
//
#include<spdlog/spdlog.h>
#include <Renderer.h>
#include<glm/glm.hpp>
#include"renderer.h"
#include"ray.h"
#include<omp.h>
using namespace glm;
void Renderer_Impl::render_frame(const Scene &scene, const Camera &camera,int spp)
{
    if(spp<=0){
        spp=16;
        spdlog::info("input spp <= 0 and set to 16");
    }
    framebuffer.clear();
    framebuffer.resize(width*height*3);

    float scale=std::tan(glm::radians(camera.fov*0.5f));
    float ratio=(float)width/height;

    vec3 look_at=vec3{camera.look_at[0],camera.look_at[1],camera.look_at[2]};
    spdlog::info("look_at: {0} {1} {2}",look_at.x,look_at.y,look_at.z);
    vec3 up=glm::normalize(vec3{camera.up[0],camera.up[1],camera.up[2]});
    vec3 camera_pos={camera.position[0],camera.position[1],camera.position[2]};
    spdlog::info("camera_pos: {0} {1} {2}",camera_pos.x,camera_pos.y,camera_pos.z);
    vec3 view_direction=glm::normalize(look_at-camera_pos);
    spdlog::info("view_direction: {0} {1} {2}",view_direction.x,view_direction.y,view_direction.z);
    vec3 right=glm::normalize(glm::cross(view_direction,up));

    vec3 view_center=camera_pos+view_direction*1.f;//near plane distance is 1.f

    spdlog::info("camera right: {0} {1} {2}",right.x,right.y,right.z);

    for(int j=0;j<height;j++){
#pragma omp parallel for
        for(int i=0;i<width;i++){
            float x=(2*(i+0.5f)/width-1.f)*scale*ratio;
            float y=(1.f-2*(j+0.5f)/height)*scale;
            auto idx=(j*width+i)*3;

            vec3 ray_start_pos_ij=view_center+right*x+up*y;
            vec3 ray_direction=glm::normalize(ray_start_pos_ij-camera_pos);

            vec4 color{0.f};
//            std::cout<<"i: "<<i<<" j: "<<j<<std::endl;
            for(int i=0;i<spp;i++){
                color+=scene.cast_ray({camera_pos,ray_direction},0);
            }
            color/=spp;

            framebuffer[idx+0]=(glm::uint8_t)255*glm::clamp(color.r,0.f,1.f);
            framebuffer[idx+1]=(glm::uint8_t)255*glm::clamp(color.g,0.f,1.f);
            framebuffer[idx+2]=(glm::uint8_t)255*glm::clamp(color.b,0.f,1.f);
        }
//        std::cout<<"height: "<<j<<std::endl;
    }


}


void Renderer_Impl::setup_window(int width, int height) {
    this->width=width;
    this->height=height;
}

auto Renderer_Impl::get_frame() -> Image  {
    Image image;
    image.width=width;
    image.height=height;
    image.nchannels=3;
    image.data=move(framebuffer);
    return image;
}


void Renderer::render_frame(const Scene &scene, const Camera &camera,int spp) {
    impl->render_frame(scene,camera,spp);
}
void Renderer::setup_window(int width, int height) {
    impl->setup_window(width,height);
}
auto Renderer::get_frame() -> Image  {
    return impl->get_frame();
}
Renderer::~Renderer() {

}

Renderer::Renderer() {
    impl=make_unique<Renderer_Impl>();
}
