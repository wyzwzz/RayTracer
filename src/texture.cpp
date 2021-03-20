//
// Created by wyz on 2021/3/12.
//

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"texture.h"
#include"util.h"
Texture::Texture(const string& name)
:name(name),method(Bilinear),width(0),height(0),nchannels(0)
{

}

void Texture::load_texture(const string& tex_name)
{
    if(tex_name.empty()){
        spdlog::error("tex_name is empty");
        return;
    }

    unsigned char *img = stbi_load(tex_name.c_str(), &width, &height, &nchannels, 0);

    if(!(width>0 && height>0 && nchannels>0 && nchannels<=4)){
        spdlog::error("texture load failed: {0}",tex_name);
        stbi_image_free(img);
        return ;
    }

    data.resize(width*height*nchannels);

    memcpy(data.data(),img,data.size());

    spdlog::info("texture file: {0}, width {1:d} height {2:d} nrComponents {3:d}",tex_name,width,height,nchannels);

    stbi_image_free(img);
}

template <class DataType>
Color nn_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v);
template <class DataType>
Color bilinear_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v);
template <class DataType>
Color cubic_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v);


Color Texture::sample(float u, float v) {
    switch (this->method) {
        case NearestNeighbor:return nn_sample(data,width,height,nchannels,u,v);
        case Bilinear:return bilinear_sample(data,width,height,nchannels,u,v);
        case Cubic:return cubic_sample(data,width,height,nchannels,u,v);
    }
}

void Texture::set_sample_method(SampleMethod method) {
    this->method=method;
}



EnvironmentMap::EnvironmentMap(const string &name)
:name(name),width(0),height(0),nchannels(0),method(Bilinear)
{

}

void EnvironmentMap::load_texture(const string &tex_name) {
    if(tex_name.empty()){
        spdlog::error("tex_name is empty");
        return;
    }

    float *img = stbi_loadf(tex_name.c_str(), &width, &height, &nchannels, 0);

    if(!(width>0 && height>0 && nchannels>0 && nchannels<=4)){
        spdlog::error("texture load failed: {0}",tex_name);
        stbi_image_free(img);
        return ;
    }

    data.resize(width*height*nchannels);

    memcpy(data.data(),img,data.size());

    spdlog::info("environment map: {0}, width {1:d} height {2:d} nrComponents {3:d}",tex_name,width,height,nchannels);

    stbi_image_free(img);
}

Color EnvironmentMap::sample(float u, float v) {
    switch (this->method) {
        case NearestNeighbor:return nn_sample(data,width,height,nchannels,u,v);
        case Bilinear:return bilinear_sample(data,width,height,nchannels,u,v);
        case Cubic:return cubic_sample(data,width,height,nchannels,u,v);
    }
}

void EnvironmentMap::set_sample_method(SampleMethod method) {
    this->method=method;
}

bool EnvironmentMap::is_empty() const {
    return data.empty();
}

Color EnvironmentMap::sample(float x, float y, float z) {
    static float inv_atan_u=0.1591f;
    static float inv_atan_v=0.3183f;
    float u=atan2(z,x)*inv_atan_u+0.5f;
    float v=asin(y)*inv_atan_v+0.5f;
    return sample(u,v);
}

template <class DataType>
Color nn_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v)
{
    Color color;
    int w=u*width;
    int h=(1.f-v)*height;
    auto offset=(w*width+h)*nComp;

    for(int i=0;i<nComp;i++){
        color[i]=data[offset+i];
    }

    return color;
}

template <class DataType>
Color bilinear_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v)
{
    Color color;
    int index_0,index_1,index_2,index_3;
    float x_pos=float(width)*u;
    float y_pos=(1.f-v)*float(height);
    int w=width*u;
    int h=(1.f-v)*height;
    index_0=(w*width+h)*nComp;
    if(h<height && w<width) {
        index_1 = ((w + 1) * width + h) * nComp;
        index_2=(w * width + h + 1) * nComp;
        index_3=((w+1)*width+h+1)*nComp;
    }
    else if(w<width){
        index_1 = ((w + 1) * width + h) * nComp;
        index_2 = index_0;
        index_3=index_1;
    }
    else if(h<height){
        index_1=index_0;
        index_2=(w * width + h + 1) * nComp;
        index_3=index_2;
    }
    else{
        index_3=index_2=index_1=index_0;
    }

    float offset_x=x_pos-w;
    float offset_y=y_pos-h;
    for(int i=0;i<nComp;i++){
        float tmp_x0=data[index_0+i]*(1.f-offset_x) + data[index_1+i]*offset_x;
        float tmp_x1=data[index_2+i]*(1.f-offset_x) + data[index_3+i]*offset_x;
        color[i]=tmp_x0*(1.f-offset_y)+tmp_x1*offset_y;
    }

    return color;
}

template <class DataType>
Color cubic_sample(const vector<DataType>& data,int width,int height,int nComp,float u,float v)
{
    Color color;
    int w=u*width;
    int h=(1.f-v)*height;

    return color;
}

