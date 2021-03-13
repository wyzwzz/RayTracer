//
// Created by wyz on 2021/3/10.
//


#include"scene.h"
#include<spdlog/spdlog.h>

bool Scene_Impl::add_model(const char *model_path) {
    models.emplace_back(model_path);
    spdlog::info("model {0} load successfully, total model size: {1}",model_path,models.size());
    return true;
}
