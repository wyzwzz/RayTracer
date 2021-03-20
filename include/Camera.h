//
// Created by wyz on 2021/3/15.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H
#include<array>
using namespace std;
class Camera{
public:
    Camera()=default;
    array<float,3> position;
    array<float,3> look_at;
    array<float,3> up;
    float fov;
};
#endif //RAYTRACER_CAMERA_H
