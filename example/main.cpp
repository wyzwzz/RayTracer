//
// Created by wyz on 2021/3/10.
//
#include<Renderer.h>
#include<Scene.h>
#include<iostream>
int main(int argc,char** argv)
{
    Camera camera;

    Scene scene;
    scene.add_model("./car/car.obj");
//    scene.add_model("./diningroom/diningroom.obj");
    Renderer renderer;


    renderer.setup_window(1200,900);
    renderer.render_frame(scene,camera);
    auto frame=renderer.get_frame();
    std::cout<<"frame "<<(uint64_t )frame.data.data();
//    frame.save_image("car_result.jpg");

    return 0;
}
