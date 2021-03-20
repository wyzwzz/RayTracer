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

Color Texture::sample(float u, float v) {
    switch (this->method) {
        case NearestNeighbor:return nn_sample(u,v);
        case Bilinear:return bilinear_sample(u,v);
        case Cubic:return cubic_sample(u,v);
    }
}

void Texture::set_sample_method(SampleMethod method) {
    this->method=method;
}

Color Texture::nn_sample(float u, float v) {
    Color color;
    int w=u*width;
    int h=(1.f-v)*height;
    auto offset=(w*width+h)*nchannels;

    for(int i=0;i<nchannels;i++){
        color[i]=data[offset+i];
    }

    return color;
}

Color Texture::bilinear_sample(float u, float v) {
    Color color;
    int index_0,index_1,index_2,index_3;
    float x_pos=float(width)*u;
    float y_pos=(1.f-v)*float(height);
    int w=width*u;
    int h=(1.f-v)*height;
    index_0=(w*width+h)*nchannels;
    if(h>=height || w>=width) {
        if (h < height)
            index_1 = (w * width + h + 1) * nchannels;
        else
            index_1 = index_0;
        if (w < width)
            index_2 = ((w + 1) * width + h) * nchannels;
        else
            index_2 = index_0;
        index_3=index_0;
    }
    else
        index_3=((w+1)*width+h+1)*nchannels;

    float offset_x=x_pos-w;
    float offset_y=y_pos-h;
    for(int i=0;i<nchannels;i++){
        float tmp_x0=data[index_0+i]*(1.f-offset_x)+data[index_1+i]*offset_x;
        float tmp_x1=data[index_2+i]*(1.f-offset_x)+data[index_3+i]*offset_x;
        color[i]=tmp_x0*(1.f-offset_y)+tmp_x1*offset_y;
    }

    return color;
}

Color Texture::cubic_sample(float u, float v) {
    Color color;
    int w=u*width;
    int h=(1.f-v)*height;

    return color;
}
