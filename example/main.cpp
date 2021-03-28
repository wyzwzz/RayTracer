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

//    camera.position={5.72f,0.12f,9.55f};
//    camera.look_at={5.085f,-0.131f,8.819f};
//    camera.up={-0.165f,0.968f,-0.189f};
//    camera.fov=45.f;
    //diningroom
//    camera.position={0.f,12.72f,31.85f};
//    camera.look_at={0.f,12.546f,30.865f};
//    camera.up={0.f,0.985f,-0.174f};
//    camera.fov=45.f;
    //nanosuit
//    camera.position={0.f,8.f,20.f};
//    camera.look_at={0.f,8.f,0.f};
//    camera.up={0.f,1.f,0.f};
//    camera.fov=45.f;
    /**
    * 2.scene config
    */
    Scene scene;

    scene.add_model("./car/car.obj");
//    scene.add_model("./cornellbox/cornellbox.obj");
//    scene.add_model("./diningroom/diningroom.obj");
//    scene.add_model("./nanosuit/nanosuit.obj");

    scene.load_environment_map("./car/environment_day.hdr");
//    scene.load_environment_map("./car/environment_dusk.hdr");
//    scene.load_environment_map("./nanosuit/environment_day.hdr");

    /**
     * 3.renderer config
     */
    Renderer renderer;

    renderer.setup_window(1920/4,1080/4);
//    renderer.setup_window(1080,1920);


    renderer.render_frame(scene,camera,4);

    auto frame=renderer.get_frame();

    frame.save_image("C:/Users/wyz/projects/RayTracer/result/car_result_t.jpg");

    return 0;
}
