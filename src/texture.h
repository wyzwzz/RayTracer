//
// Created by wyz on 2021/3/12.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H
#include<string>
#include<vector>
#include<iostream>

using namespace std;
struct Color{
    Color():r(0),g(0),b(0),a(0){}
    uint8_t r,g,b,a;
    uint8_t & operator[](size_t index){
        switch (index) {
            case 0:return r;
            case 1:return g;
            case 2:return b;
            case 3:return a;
            default:throw runtime_error("Color: index out of range");
        }
    }
};
enum SampleMethod{
    NearestNeighbor,
    Bilinear,
    Cubic
};

using namespace std;
class ITexture{
public:
    ITexture()=default;
    virtual void load_texture(const string& tex_name)=0;
    virtual Color sample(float u,float v)=0;
    virtual void set_sample_method(SampleMethod method)=0;
    virtual bool is_empty() const=0;
};
class Texture: public ITexture{
public:
    Texture(const string& name);
    void load_texture(const string& tex_name) override;
    Color sample(float u,float v) override;
    void set_sample_method(SampleMethod method) override;
    bool is_empty() const override{
        return data.empty();
    }
private:

public:
    string name;

    vector<uint8_t> data;

    int width,height,nchannels;

    SampleMethod method;
};
class EnvironmentMap: public ITexture{
public:
    EnvironmentMap(const string& name);
    void load_texture(const string& tex_name) override;
    Color sample(float u,float v) override;
    void set_sample_method(SampleMethod method) override;
    bool is_empty() const override;

    /**
     * @brief x,y,z must be normalized vector
     */
    Color sample(float x,float y,float z);


private:

private:
    SampleMethod method;
    int width,height,nchannels;
    string name;
    vector<float> data;
};

#endif //RAYTRACER_TEXTURE_H
