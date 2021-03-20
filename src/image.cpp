//
// Created by wyz on 2021/3/19.
//
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#include<Image.h>

void Image::save_image(const string &file_name) {

    stbi_write_png(file_name.c_str(),width,height,nchannels,data.data(),0);

}
