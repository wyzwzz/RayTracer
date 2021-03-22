//
// Created by wyz on 2021/3/11.
//
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
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
    spdlog::critical("can't get model name for: {0}",model_path);
    return "";
}
void Model::load(const char *model_path) {
    START_TIMER

    tinyobj::ObjReaderConfig reader_config;
    this->name=getModelName(model_path);
    reader_config.mtl_search_path="./"+name;
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
        spdlog::info("diffuse {0:f} {1:f} {2:f}",m_->diffuse[0],m_->diffuse[1],m_->diffuse[2]);
        m_->specular={m.specular[0],m.specular[1],m.specular[2]};
        m_->transmittance={m.transmittance[0],m.transmittance[1],m.transmittance[2]};
        m_->emission={m.emission[0],m.emission[1],m.emission[2]};
        spdlog::info("emission {0:f} {1:f} {2:f}",m_->emission[0],m_->emission[1],m_->emission[2]);
        m_->shininess=m.shininess;
        spdlog::info("shininess {0}",m_->shininess);
        m_->ambient_texname=m.ambient_texname;
        m_->diffuse_texname=m.diffuse_texname;
        m_->specular_texname=m.specular_texname;
        m_->specular_highlight_texname=m.specular_highlight_texname;
        m_->diffuse_texture=make_unique<Texture<uint8_t>>(m_->name);
        m_->diffuse_texture->load_texture(this->name+"/"+m_->diffuse_texname);
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
        TriangleMesh mesh;
        mesh.triangles.reserve(shapes[s].mesh.num_face_vertices.size());
        size_t index_offset=0;
        for(size_t f=0;f<shapes[s].mesh.num_face_vertices.size();f++)
        {

            auto fv=shapes[s].mesh.num_face_vertices[f];
            if(fv!=3)
            {
                spdlog::critical("only support triangle");
            }
            Triangle triangle;
            for(size_t v=0;v<fv;v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                triangle.vertices[v].pos.x = attrib.vertices[3 * idx.vertex_index + 0];
                triangle.vertices[v].pos.y = attrib.vertices[3 * idx.vertex_index + 1];
                triangle.vertices[v].pos.z = attrib.vertices[3 * idx.vertex_index + 2];
                triangle.vertices[v].normal.x = attrib.normals[3 * idx.normal_index + 0];
                triangle.vertices[v].normal.y = attrib.normals[3 * idx.normal_index + 1];
                triangle.vertices[v].normal.z = attrib.normals[3 * idx.normal_index + 2];
//                if(triangle.vertices[v].tex_coord.x>1.f || triangle.vertices[v].tex_coord.y>1.f){
//                    spdlog::error("tex coord {2} > 1.f {0} {1}",triangle.vertices[v].tex_coord.x,triangle.vertices[v].tex_coord.y,
//                                  ms[shapes[s].mesh.material_ids[f]]->name);
//                }
                float _ux=attrib.texcoords[2 * idx.texcoord_index + 0];
                int _u=_ux;
                if(_u>0)
                    triangle.vertices[v].tex_coord.x = _ux-_u;
                else if(_ux<0.f)
                    triangle.vertices[v].tex_coord.x = _ux-_u+1;
                float _vy=attrib.texcoords[2 * idx.texcoord_index + 1];
                int _v=_vy;
                if(_v>0)
                    triangle.vertices[v].tex_coord.y = _vy-_v;
                else if(_vy<0.f)
                    triangle.vertices[v].tex_coord.y = _vy-_v+1;

                if(triangle.vertices[v].tex_coord.x>1.f || triangle.vertices[v].tex_coord.y>1.f){
                    spdlog::critical("tex coord {2} > 1.f {0} {1}",triangle.vertices[v].tex_coord.x,triangle.vertices[v].tex_coord.y,
                                  ms[shapes[s].mesh.material_ids[f]]->name);
                }
            }
            triangle.init();

            size_t m_id=shapes[s].mesh.material_ids[f];

//            spdlog::debug("{0:d}",m_id);
//            spdlog::info("{0}",materials[m_id].diffuse_texname);

            triangle.m=ms[m_id];

            mesh.addTriangle(move(triangle));

            index_offset += fv;
        }
        mesh.build_bvh_tree();
        meshes.emplace_back(move(mesh));
    }


    END_TIMER("load model: "+string(model_path));
}

Intersection Model::get_intersection(const Ray &ray) {
    for(auto& mesh:meshes){
        auto intersect=mesh.get_intersection(ray);
        if(intersect.happen){
            return intersect;//assert one ray just intersect one mesh one triangle
        }

    }
    return Intersection();
}

AABB Model::get_bound() const {
    AABB aabb;
    for(auto& mesh:meshes){
        aabb.union_(mesh.get_bound());
    }
    return aabb;
}
