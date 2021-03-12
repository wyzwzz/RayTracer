//
// Created by wyz on 2021/3/12.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H
#include<string>
#include<vector>
struct Color{
    Color():r(0),g(0),b(0),a(0){}
    uint8_t r,g,b,a;
    uint8_t & operator[](size_t index){
        switch (index) {
            case 0:return r;
            case 1:return g;
            case 2:return b;
            case 3:return a;
        }
    }
};
enum SampleMethod{
    NearestNeighbor,
    Bilinear,
    Cubic
};
using namespace std;
class Texture{
public:
    Texture(const string& name);
    void load_texture(const string& tex_name);
    Color sample(float u,float v);
    void set_sample_method(SampleMethod method);

private:
    Color&& nn_sample(float u,float v);
    Color&& bilinear_sample(float u,float v);
    Color&& cubic_sample(float u,float v);
public:
    string name;

    vector<uint8_t> data;

    int width,height,nchannels;

    SampleMethod method;
};


#endif //RAYTRACER_TEXTURE_H
