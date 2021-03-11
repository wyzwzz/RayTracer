//
// Created by wyz on 2021/3/11.
//
#define TINYOBJLOADER_IMPLEMENTATION

#include<tiny_obj_loader.h>
#include"model.h"
#include"util.h"
Model::Model(const char *model_path) {
    load(model_path);
}
std::string getModelName(const char* model_path)
{
    std::string path(model_path);
    for(int i=path.size()-1;i>=0;i--){
        if(path[i]=='/'){
            std::string name=path.substr(i+1,path.size()-i-5);
            spdlog::info("model name :{0}",name);
            return name;
        }
    }
}
void Model::load(const char *model_path) {
    START_TIMER

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path="./"+getModelName(model_path);
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(model_path, reader_config))
    {
        if (!reader.Error().empty())
        {
            spdlog::error("TinyObjReader: {0}",reader.Error());
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        spdlog::error("TinyObjReader: {0}",reader.Warning());
    }
    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();
    for(auto& m:materials)
        spdlog::info("{0}",m.name);
    spdlog::info("mesh size: {0}",shapes.size());
    for(auto& it:shapes){
        spdlog::info("mesh {0} size: {1}",it.name,it.mesh.num_face_vertices.size());
    }
    spdlog::info("total triangle size: {0}",attrib.vertices.size()/3);




    END_TIMER("load model: "+string(model_path));
}
