//
// Created by wyz on 2021/3/15.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H
#include<vector>
#include<string>
#include<iostream>
using namespace std;
class Image{
public:
    uint32_t width,height,nchannels;
    vector<uint8_t> data;

    Image():width(0),height(0),nchannels(0){
        std::cout<<"Image Construction"<<std::endl;
    }
    ~Image(){
        data.clear();
    }
    Image(const Image& image){
        width=image.width;
        height=image.height;
        nchannels=image.nchannels;
        data=image.data;
        std::cout<<"Image Copy Construction"<<std::endl;
    }
    Image& operator=(Image&& image) noexcept {
        width=image.width;
        height=image.height;
        nchannels=image.nchannels;
        data=move(image.data);

    }
    Image(Image&& image) noexcept {
        width=image.width;
        height=image.height;
        nchannels=image.nchannels;
        data=move(image.data);

    }

    void save_image(const string& file_name);
};



#endif //RAYTRACER_IMAGE_H
