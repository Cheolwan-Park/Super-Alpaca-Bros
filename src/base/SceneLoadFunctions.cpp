#include "Scene.hpp"

namespace Base
{
    void Scene::SetJsonFile(const FileIO &f)
    {
        OpenJsonFile(f, &m_doc);
    }

    int32 Scene::SetJsonFile(const char *filename)
    {
        String128 path = Directories::Scene;
        path += filename;
        return OpenJsonFile(path.C_Str(), &m_doc);
    }

    GameObject *Scene::CreateGameObject(const rapidjson::Value::Object &obj)
    {
        if(obj.HasMember("file"))
        {
            assert(obj["file"].IsString());
            String128 path("");
            path = Directories::GameObject;
            path += obj["file"].GetString();
            rapidjson::Document doc;
            OpenJsonFile(path.C_Str(), &doc);

            assert(doc.HasMember("Object"));
            assert(doc["Object"].IsObject());

            return CreateGameObject(doc["Object"].GetObject());
        }

        assert(obj.HasMember("storage"));
        assert(obj.HasMember("id"));
        assert(obj["storage"].IsString());
        assert(obj["id"].IsString());

        const char *storageid = obj["storage"].GetString();
        const char *id = obj["id"].GetString();

        auto *storage = GetObjectStorage(CompileTimeHash::runtime_hash(storageid, strlen(storageid)));
        assert(storage);
        GameObject *newobject = GameObject::Factory(obj, Application::Get().GetAllocator(), storage, CompileTimeHash::runtime_hash(id, strlen(id)));
        assert(newobject);
        #ifndef NDEBUG 
        printf("%s(%u) added\n", id, newobject->GetID());
        #endif
        return newobject;
    }

    int32 Scene::LoadPhysics()
    {
        assert(m_doc.HasMember("Physics"));
        assert(m_doc["Physics"].IsObject());

        m_physics.InitWithJson(m_doc["Physics"].GetObject(), Application::Get().GetAllocator());

        return RET_SUCC;
    }

    int32 Scene::LoadCameras()
    {
        Application &app = Application::Get();
        auto &allocator = app.GetAllocator();

        assert(m_doc.HasMember("Cameras"));
        assert(m_doc["Cameras"].IsArray());
        auto loadlist = m_doc["Cameras"].GetArray();

        Camera *newcamera = nullptr;
        for(auto itr = loadlist.begin();
            loadlist.end() != itr; 
            ++itr)
            {
                assert(itr->IsObject());
                auto obj = itr->GetObject();

                newcamera = new (allocator.Alloc<Camera>()) Camera();
                assert(newcamera);
                newcamera->InitWithJson(obj);
                m_cameras.Append(newcamera);
                #ifndef NDEBUG
                printf("camera : name(%s)\n", obj["name"].GetString());
                #endif
            }

        return RET_SUCC;
    }

    int32 Scene::LoadShaders()
    {
        Application &app = Application::Get();
        auto &shaders = app.GetShaderStorage();
        auto &allocator = app.GetAllocator();

        void *mem_vert = nullptr, *mem_frag = nullptr;

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

                program = new (allocator.Alloc<ShaderProgram>()) ShaderProgram();
                if(RET_SUCC != program->InitWithJson(obj, mem_vert, mem_frag))
                {
                    free(mem_vert);
                    free(mem_frag);
                    return RET_FAILED;
                }
                shaders.Register(program, program->GetID());

                #ifndef NDEBUG
                printf("shader : name(%s) vert(%s) frag(%s)\n", obj["name"].GetString(), obj["vert"].GetString(), obj["frag"].GetString());
                #endif
            }
        
        free(mem_vert);
        free(mem_frag);
        
        return RET_SUCC;
    }

    int32 Scene::LoadTextures()
    {
        Application &app = Application::Get();
        auto &textures = app.GetTextureStorage();
        auto &allocator = app.GetAllocator();

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
            assert(itr->IsObject());
            auto obj = itr->GetObject();

            texture = new (allocator.Alloc<Texture>()) Texture({0,0,0});
            LoadTexture(texture, obj);

            textures.Register(texture, texture->hashid);
            
            #ifndef NDEBUG
            printf("tex : %s\n", obj["file"].GetString());
            #endif
        }

        return RET_SUCC;
    }

    int32 Scene::LoadAnimations()
    {
        Application &app = Application::Get();
        auto &allocator = app.GetAllocator();
        auto &animations = app.GetAnimationStorage();

        assert(m_doc.HasMember("Animations"));
        assert(m_doc["Animations"].IsArray());
        const auto loadlist = m_doc["Animations"].GetArray();

        Animation *newanim = nullptr;
        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto anim = itr->GetObject();

                newanim = new (allocator.Alloc<Animation>()) Animation();
                assert(newanim);
                newanim->InitWithJson(anim);

                animations.Register(newanim, newanim->GetID());

                #ifndef NDEBUG
                printf("anim : %s\n", anim["name"].GetString());
                #endif
            }

        return RET_SUCC;
    }

    int32 Scene::CreateObjectStorages()
    {
        Application &app = Application::Get();
        auto &allocator = app.GetAllocator();

        assert(m_doc.HasMember("ObjectStorages"));
        assert(m_doc["ObjectStorages"].IsArray());
        auto loadlist = m_doc["ObjectStorages"].GetArray();

        ObjectStorage *newstorage = nullptr;
        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto obj = itr->GetObject();

                newstorage = new (allocator.Alloc<ObjectStorage>()) ObjectStorage();
                newstorage->InitWithJson(obj, allocator);
                AddObjectStorage(newstorage);

                #ifndef NDEBUG
                printf("objsto : name(%s) size(%d) order(%d)\n", obj["name"].GetString(), obj["size"].GetInt(), obj["order"].GetInt());
                #endif
            }

        return RET_SUCC;
    }

    int32 Scene::CreateDrawableStorages()
    {
        Application &app = Application::Get();
        StackAllocator &allocator = app.GetAllocator();

        assert(m_doc.HasMember("DrawableStorages"));
        assert(m_doc["DrawableStorages"].IsArray());
        auto loadlist = m_doc["DrawableStorages"].GetArray();

        DrawableStorage *new_storage = nullptr;

        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto obj = itr->GetObject();
                new_storage = new (allocator.Alloc<DrawableStorage>()) DrawableStorage();
                new_storage->InitWithJson(obj, allocator);
                AddDrawableStorage(new_storage);

                #ifndef NDEBUG
                printf("drawablesto : name(%s) shader(%s) size(%d) order(%d)\n", obj["name"].GetString(), obj["shader"].GetString(), obj["size"].GetInt(), obj["order"].GetInt());
                #endif
            }

        return RET_SUCC;
    }

    int32 Scene::CreateObjects()
    {
        Application &app = Application::Get();
        auto &allocator = app.GetAllocator();
        
        assert(m_doc.HasMember("GameObjects"));
        assert(m_doc["GameObjects"].IsArray());
        auto loadlist = m_doc["GameObjects"].GetArray();

        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                CreateGameObject(itr->GetObject());
            }

        return RET_SUCC;
    }
}