//
// Created by wyz on 2021/3/10.
//


#include"scene.h"


bool Scene_Impl::add_model(const char *model_path) {
    models.emplace_back(model_path);
    return true;
}
