//
// Created by wyz on 2021/3/12.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H
#include<string>
#include<vector>
#include<iostream>
#include<spdlog/spdlog.h>
#include"stb_image.h"

using namespace std;

template<class DataType>
class Color{
public:
    Color():r(0),g(0),b(0),a(0){}
    DataType r,g,b,a;
    DataType & operator[](size_t index){
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

template<class T>
class Texture{
public:
    Texture(const string& name):name(name),method(Bilinear),width(0),height(0),nchannels(0){ }


    void load_texture(const string& tex_name);



    Color<T> sample(float x,float y,float z);
    Color<T> sample(float u,float v);
    void set_sample_method(SampleMethod method) ;
    bool is_empty() const {
        return data.empty();
    }
private:

public:
    string name;

    vector<T> data;

    int width,height,nchannels;

    SampleMethod method;
};

template<>
inline void Texture<uint8_t>::load_texture(const string &tex_name) {
    this->name=tex_name;
    if(tex_name.empty()){
        spdlog::error("tex_name is empty");
        return;
    }

    auto img = stbi_load(tex_name.c_str(), &width, &height, &nchannels, 0);

    if(!(width>0 && height>0 && nchannels>0 && nchannels<=4)){
        spdlog::error("texture load failed: {0}",tex_name);
        stbi_image_free(img);
        return ;
    }

    data.resize((int64_t)width*height*nchannels);

    memcpy(data.data(),img,data.size());

    spdlog::info("texture file: {0}, width {1:d} height {2:d} nrComponents {3:d}",tex_name,width,height,nchannels);

//    if(tex_name=="car/textures/street_albedo.jpg"){
//        for(size_t i=0;i<height;i++){
//            for(size_t j=0;j<width;j++){
//                std::cout<<" ("<<(int)img[(i*width+j)*nchannels]<<" "<<(int)img[(i*width+j)*nchannels+1]<<" "<<(int)img[(i*width+j)*nchannels+2]<<") ";
//            }
//            std::cout<<std::endl;
//        }
//    }

    stbi_image_free(img);
}

template<>
inline void Texture<float>::load_texture(const string& tex_name)
{
    this->name=tex_name;
    if(tex_name.empty()){
        spdlog::error("tex_name is empty");
        return;
    }
//    stbi_set_flip_vertically_on_load(true);
    float* img = stbi_loadf(tex_name.c_str(), &width, &height, &nchannels, 0);

    if(!(width>0 && height>0 && nchannels>0 && nchannels<=4)){
        spdlog::error("texture load failed: {0}",tex_name);
        stbi_image_free(img);
        return ;
    }

    data.resize((int64_t)width*height*nchannels);

    memcpy(data.data(),img,data.size()*sizeof(float));

    spdlog::info("texture file: {0}, width {1:d} height {2:d} nrComponents {3:d}",tex_name,width,height,nchannels);

    size_t cnt=0;
//    for(size_t i=800;i<height;i++){
//        for(size_t j=0;j<width;j++){
//            std::cout<<data[(i*width+j)*nchannels]<<" ";
//        }
//    }
    spdlog::info("min value : {0}",cnt);

    stbi_image_free(img);
}
template <class DataType>
Color<DataType> nn_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v);
template <class DataType>
Color<DataType> bilinear_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v);
template <class DataType>
Color<DataType> cubic_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v);

template <class T>
inline Color<T> Texture<T>::sample(float u, float v) {
//    std::cout<<"name: "<<name<<std::endl;
    switch (this->method) {
        case NearestNeighbor:return nn_sample(data,width,height,nchannels,u,v);
        case Bilinear:return bilinear_sample(data,width,height,nchannels,u,v);
        case Cubic:return cubic_sample(data,width,height,nchannels,u,v);
    }
}
template <class T>
inline void Texture<T>::set_sample_method(SampleMethod method) {
    this->method=method;
}

template<class T>
inline Color<T> Texture<T>::sample(float x, float y, float z) {
    static float inv_atan_u=0.1591f;
    static float inv_atan_v=0.3183f;
    float u=atan2(-x,z)*inv_atan_u+0.5f;
    float v=asin(y)*inv_atan_v+0.5f;
    return sample(u,v);
}
template <class DataType>
inline Color<DataType> nn_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v)
{
    Color<DataType> color;
    int w=u*width;
    int h=(1.f-v)*height;
    auto offset=(w*width+h)*nComp;

    for(int i=0;i<nComp;i++){
        color[i]=data[offset+i];
    }

    return color;
}


template <class DataType>
inline Color<DataType> bilinear_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v)
{
    if(data.size()!=width*height*nComp){
        spdlog::critical("texture data size error!");
    }

    Color<DataType> color;
    int64_t index_0,index_1,index_2,index_3;
    float x_pos=float(width-1)*u;
    float y_pos=(1.f-v)*float(height-1);
    int64_t w=(width-1)*u;
    int64_t h=(1.f-v)*(height-1);
    if(u>1.f || u<0.f || v>1.f || v<0.f){
        spdlog::critical("u v out of range {0} {1}",u,v);
    }
    index_0=(h*width+w)*nComp;

    if(h<height-2 && w<width-2) {
        index_1 = ((h + 1) * width + w) * nComp;
        index_2=(h * width + w + 1) * nComp;
        index_3=((h+1)*width+w+1)*nComp;
    }
    else if(w<width-2){
        index_1 = ((h + 1) * width + w) * nComp;
        index_2 = index_0;
        index_3=index_1;
    }
    else if(h<height-2){
        index_1=index_0;
        index_2=(h * width + w + 1) * nComp;
        index_3=index_2;
    }
    else{
        index_3=index_2=index_1=index_0;
    }
//    spdlog::set_level(spdlog::level::info);
//    spdlog::info("width {0} height {1} ncomp {2}",width,height,nComp);
//    spdlog::info("index: {0} {1} {2} {3} for size {4}",index_0,index_1,index_2,index_3,data.size());
//    spdlog::info("u v: {0} {1}",u,v);
    float offset_x=x_pos-w;
    float offset_y=y_pos-h;
    for(int i=0;i<nComp;i++){
//        std::cout<<data[index_0+i]<<" "<<data[index_1+i]<<std::endl;
        float tmp_x0=data[index_0+i]*(1.f-offset_x) + data[index_1+i]*offset_x;
        float tmp_x1=data[index_2+i]*(1.f-offset_x) + data[index_3+i]*offset_x;
        color[i]=tmp_x0*(1.f-offset_y)+tmp_x1*offset_y;
    }

    return color;
}

template <class DataType>
inline Color<DataType> cubic_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v)
{
    Color<DataType> color;
    int w=u*width;
    int h=(1.f-v)*height;

    return color;
}

#endif //RAYTRACER_TEXTURE_H
