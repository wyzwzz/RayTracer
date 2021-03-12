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

    assert(width>0 && height>0 && nchannels>0 && nchannels<=4);

    data.resize(width*height*nchannels);

    memcpy(data.data(),img,data.size());

    spdlog::info("texture file: {0}, width {1:d} height {2:d} nrComponents {3:d}",tex_name,width,height,nchannels);

    stbi_image_free(img);
}

Color Texture::sample(float u, float v) {
    switch (this->method) {
        case NearestNeighbor:return nn_sample(u,v);break;
        case Bilinear:return bilinear_sample(u,v);break;
        case Cubic:return cubic_sample(u,v);break;
    }
}

void Texture::set_sample_method(SampleMethod method) {
    this->method=method;
}

Color &&Texture::nn_sample(float u, float v) {
    Color color;
    int w=u*width;
    int h=(1.f-v)*height;
    auto offset=(w*width+h)*nchannels;

    for(int i=0;i<nchannels;i++){
        color[i]=data[offset+i];
    }

    return move(color);
}

Color &&Texture::bilinear_sample(float u, float v) {
    Color color;
    int index_0,index_1,index_2,index_3;
    float x_pos=float(width)*u;
    float y_pos=(1.f-v)*float(height);
    float w=width*u;
    float h=(1.f-v)*height;
    float offset_x=x_pos-w;
    float offset_y=y_pos-h;


    return move(color);
}

Color &&Texture::cubic_sample(float u, float v) {
    Color color;
    int w=u*width;
    int h=(1.f-v)*height;

    return move(color);
}
