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
    auto mode_name=getModelName(model_path);
    reader_config.mtl_search_path="./"+mode_name;
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
    vector<shared_ptr<Material>> ms;
    for(auto& m:materials)
    {
        spdlog::info("{0}",m.name);
        unique_ptr<Material> m_(new Material());
        m_->name=m.name;
        m_->ambient={m.ambient[0],m.ambient[1],m.ambient[2]};
        m_->diffuse={m.diffuse[0],m.diffuse[1],m.diffuse[2]};
        m_->specular={m.specular[0],m.specular[1],m.specular[2]};
        m_->transmittance={m.transmittance[0],m.transmittance[1],m.transmittance[2]};
        m_->emission={m.emission[0],m.emission[1],m.emission[2]};
        m_->shininess=m.shininess;
        m_->ambient_texname=m.ambient_texname;
        m_->diffuse_texname=m.diffuse_texname;
        m_->specular_texname=m.specular_texname;
        m_->specular_highlight_texname=m.specular_highlight_texname;
        m_->texture=make_unique<Texture>(m_->name);
        m_->texture->load_texture(mode_name+"/"+m_->diffuse_texname);
        ms.push_back(move(m_));
    }
    for(auto i=0;i<materials.size();i++){
        assert(ms[i]->name==materials[i].name);
    }
    spdlog::info("mesh size: {0}",shapes.size());
    for(auto& it:shapes)
    {
        spdlog::info("mesh {0} triangles size: {1}",it.name,it.mesh.num_face_vertices.size());
    }
    spdlog::info("total vertex size: {0}",attrib.vertices.size()/3);
    for(size_t s=0;s<shapes.size();s++)
    {
        size_t index_offset=0;
        for(size_t f=0;f<shapes[s].mesh.num_face_vertices.size();f++)
        {
            auto fv=shapes[s].mesh.num_face_vertices[f];
            if(fv!=3)
            {
                spdlog::critical("only support triangle");
            }


            int m_id=shapes[s].mesh.material_ids[f];
//            spdlog::debug("{0:d}",m_id);
//            spdlog::info("{0}",materials[m_id].diffuse_texname);

        }
    }


    END_TIMER("load model: "+string(model_path));
}
