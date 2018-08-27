#include "JsonScene.hpp"
#include "FileIO.hpp"
#include "ShaderProgram.hpp"
#include "Allocator.hpp"

namespace Base
{
    JsonScene::JsonScene()
    :ObjectScene(), m_doc()
    {
        ;
    }

    JsonScene::~JsonScene()
    {
        ;
    }

    int32 JsonScene::Init()
    {
        assert(m_doc.IsObject());

        return ObjectScene::Init();
    }

    int32 JsonScene::SetJsonFile(FILE *f)
    {
        size_t len_json = GetFileSize(f);
        void *mem_json = nullptr;
        FileIO json_io;
        mem_json = malloc(len_json+1);
        memset(mem_json, 0, len_json+1);
        if(RET_SUCC != json_io.Open(f, mem_json, len_json))
        {
            free(mem_json);
            return RET_FAILED;
        }

        m_doc.Parse(json_io.GetBuffer());
        if(!m_doc.IsObject())
        {
            free(mem_json);
            return RET_FAILED;
        }

        free(mem_json);
        return RET_SUCC;
    }

    int32 JsonScene::LoadShaders()
    {
        Application &app = Application::Get();
        auto &shaders = app.GetShaderStorage();
        StackAllocator &allocator = app.GetAllocator();

        String128 path;
        const char *vert_filename = nullptr, *frag_filename = nullptr;
        FILE *vert = nullptr, *frag = nullptr;
        void *mem_vert = nullptr, *mem_frag = nullptr;
        size_t size_vert = 0, size_frag = 0;
        FileIO vert_io, frag_io;

        const char *shader_name = nullptr;
        ShaderProgram *program = nullptr;

        assert(m_doc.HasMember("Shaders"));
        assert(m_doc["Shaders"].IsArray());
        auto loadlist = m_doc["Shaders"].GetArray();

        for(auto itr = loadlist.begin();
            loadlist.end() != itr; 
            ++itr)
            {
                assert(itr->IsObject());
                auto obj = itr->GetObject();
                assert(obj.HasMember("name"));
                assert(obj.HasMember("vert"));
                assert(obj.HasMember("frag"));
                assert(obj["name"].IsString());
                assert(obj["vert"].IsString());
                assert(obj["frag"].IsString());

                shader_name = obj["name"].GetString();
                vert_filename = obj["vert"].GetString();
                frag_filename = obj["frag"].GetString();

                path.Clear();
                path = Directories::root;
                path += Directories::Shader;
                path += vert_filename;
                vert = OpenFile(path.C_Str(), "r");

                path.Clear();
                path = Directories::root;
                path += Directories::Shader;
                path += frag_filename;
                frag = OpenFile(path.C_Str(), "r");

                size_vert = GetFileSize(vert);
                size_frag = GetFileSize(frag);

                if(nullptr == mem_vert && nullptr == mem_frag)
                {
                    mem_vert = malloc(size_vert+1);
                    mem_frag = malloc(size_frag+1);
                }
                else
                {
                    mem_vert = realloc(mem_vert, size_vert+1);
                    mem_frag = realloc(mem_frag, size_frag+1);
                }
                memset(mem_vert, 0, size_vert+1);
                memset(mem_frag, 0, size_frag+1);

                if(RET_SUCC != vert_io.Open(vert, mem_vert, size_vert))
                {
                    free(mem_vert);
                    free(mem_frag);
                    return RET_FAILED;
                }

                if(RET_SUCC != frag_io.Open(frag, mem_frag, size_frag))
                {
                    free(mem_vert);
                    free(mem_frag);
                    return RET_FAILED;
                }
                
                program = new (allocator.Alloc<ShaderProgram>()) ShaderProgram();
                if(RET_SUCC != program->Init(vert_io, frag_io))
                {
                    free(mem_vert);
                    free(mem_frag);
                    return RET_FAILED;
                }

                StringID shader_id(shader_name, strlen(shader_name));
                shaders.Register(program, (Uint32)shader_id);

                printf("shader : name(%s) vert(%s) frag(%s)\n", shader_name, vert_filename, frag_filename);
            }
        
        free(mem_vert);
        free(mem_frag);
        
        return RET_SUCC;
    }

    int32 JsonScene::LoadTextures()
    {
        Application &app = Application::Get();
        auto &textures = app.GetTextureStorage();
        StackAllocator &allocator = app.GetAllocator();

        String128 path;
        const char *filename = nullptr;
        Texture *texture = nullptr;

        assert(m_doc.HasMember("Textures"));
        assert(m_doc["Textures"].IsArray());
        auto loadlist = m_doc["Textures"].GetArray();

        for(auto itr = loadlist.begin();
            loadlist.end() != itr; 
            ++itr)
        {
            assert(itr->IsString());
            filename = itr->GetString();
            StringID texture_id(filename, strlen(filename));

            texture = new (allocator.Alloc<Texture>()) Texture({0,0,0});
            
            path.Clear();
            path = Directories::root;
            path += Directories::Texture;
            path += filename;
            texture->id = LoadTexture(path.C_Str(), &texture->w, &texture->h);
            assert(texture->id);

            textures.Register(texture, (Uint32)texture_id);

            printf("tex : %s\n", filename);
        }

        return RET_SUCC;
    }

    int32 JsonScene::CreateObjectStorages()
    {
        Application &app = Application::Get();
        StackAllocator &allocator = app.GetAllocator();

        assert(m_doc.HasMember("ObjectStorages"));
        assert(m_doc["ObjectStorages"].IsArray());
        auto loadlist = m_doc["ObjectStorages"].GetArray();

        const char *storage_name = nullptr;
        int32 order = 0, size = 0;
        ObjectStorage *new_storage = nullptr;
        
        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto obj = itr->GetObject();
                assert(obj.HasMember("name"));
                assert(obj.HasMember("order"));
                assert(obj.HasMember("size"));
                assert(obj["name"].IsString());
                assert(obj["order"].IsInt());
                assert(obj["size"].IsInt());

                storage_name = obj["name"].GetString();
                order = obj["order"].GetInt();
                size = obj["size"].GetInt();
                StringID storage_id(storage_name, strlen(storage_name));

                new_storage = new (allocator.Alloc<ObjectStorage>()) ObjectStorage((Uint32)storage_id, order);
                new_storage->AssignMemory(allocator.Alloc<ObjectStorage::Type>(size), size);
                AddObjectStorage(new_storage);

                printf("objsto : name(%s) size(%d) order(%d)\n", storage_name, size, order);
            }

        return RET_SUCC;
    }

    int32 JsonScene::CreateDrawableStorages()
    {
        Application &app = Application::Get();
        StackAllocator &allocator = app.GetAllocator();
        auto &shaders = app.GetShaderStorage();

        assert(m_doc.HasMember("DrawableStorages"));
        assert(m_doc["DrawableStorages"].IsArray());
        auto loadlist = m_doc["DrawableStorages"].GetArray();

        const char *storage_name = nullptr;
        const char *shader_name = nullptr;
        int32 order = 0, size = 0;
        DrawableStorage *new_storage = nullptr;

        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto obj = itr->GetObject();
                assert(obj.HasMember("name"));
                assert(obj.HasMember("shader"));
                assert(obj.HasMember("order"));
                assert(obj.HasMember("size"));
                assert(obj["name"].IsString());
                assert(obj["shader"].IsString());
                assert(obj["order"].IsInt());
                assert(obj["size"].IsInt());

                storage_name = obj["name"].GetString();
                shader_name = obj["shader"].GetString();
                order = obj["order"].GetInt();
                size = obj["size"].GetInt();
                StringID storage_id(storage_name, strlen(storage_name));
                StringID shader_id(shader_name, strlen(shader_name));

                new_storage = new (allocator.Alloc<DrawableStorage>()) DrawableStorage((Uint32)storage_id, order);
                new_storage->AssignMemory(allocator.Alloc<DrawableStorage::Type>(size), size);
                new_storage->SetShader(shaders[(Uint32)shader_id]);
                AddDrawableStorage(new_storage);

                printf("drawablesto : name(%s) shader(%s) size(%d) order(%d)\n", storage_name, shader_name, size, order);
            }

        return RET_SUCC;
    }
}