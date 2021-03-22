//
// Created by wyz on 2021/3/10.
//
#include<Renderer.h>
#include<Scene.h>

int main(int argc,char** argv)
{
    /**
     * 1.camera config
     */
    Camera camera;

    //cornellbox
//    camera.position={0.f,0.f,2.5f};
//    camera.look_at={0.f,0.f,0.f};
//    camera.up={0.f,1.f,0.f};
//    camera.fov=60.f;
    //car
    camera.position={8.22f,-0.61f,-9.80f};
    camera.look_at={7.514f,-0.702f,-9.097f};
    camera.up={-0.065f,0.996f,0.065f};
    camera.fov=45.f;

    /**
    * 2.scene config
    */
    Scene scene;

    scene.add_model("./car/car.obj");
//    scene.add_model("./cornellbox/cornellbox.obj");
//    scene.add_model("./diningroom/diningroom.obj");

    scene.load_environment_map("./car/environment_day.hdr");

    /**
     * 3.renderer config
     */
    Renderer renderer;

    renderer.setup_window(1920/8,1080/8);

    renderer.render_frame(scene,camera,1);

    auto frame=renderer.get_frame();

    frame.save_image("C:/Users/wyz/projects/RayTracer/result/car_result4.jpg");

    return 0;
}
