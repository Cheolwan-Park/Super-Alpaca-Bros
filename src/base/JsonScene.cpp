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

    void JsonScene::SetJsonFile(const FileIO &f)
    {
        OpenJsonFile(f, &m_doc);
    }

    int32 JsonScene::SetJsonFile(const char *filename)
    {
        return OpenJsonFile(filename, &m_doc);
    }

    GameObject *JsonScene::CreateGameObject(const rapidjson::Value::Object &obj)
    {
        if(obj.HasMember("file"))
        {
            assert(obj["file"].IsString());
            String128 path("");
            path = Directories::GameObject;
            path += obj["file"].GetString();
            printf("path : %s\n", path.C_Str());
            rapidjson::Document doc;
            OpenJsonFile(path.C_Str(), &doc);

            assert(doc.HasMember("Object"));
            assert(doc["Object"].IsObject());

            return CreateGameObject(doc["Object"].GetObject());
        }

        assert(obj.HasMember("type"));
        assert(obj.HasMember("storage"));
        assert(obj.HasMember("id"));
        assert(obj["type"].IsString());
        assert(obj["storage"].IsString());
        assert(obj["id"].IsString());

        const char *type = obj["type"].GetString();
        const char *storageid = obj["storage"].GetString();
        const char *id = obj["id"].GetString();

        auto *factoryfunc = GameObjectFactory::GetGlobal().GetFunction(CompileTimeHash::runtime_hash(type, strlen(type)));
        assert(factoryfunc);
        auto *storage = GetObjectStorage(CompileTimeHash::runtime_hash(storageid, strlen(storageid)));
        assert(storage);
        GameObject *newobject = (*factoryfunc)(obj, Application::Get().GetAllocator(), storage, CompileTimeHash::runtime_hash(id, strlen(id)));
        assert(newobject);
        #ifndef NDEBUG 
        printf("%s(%u) added\n", id, newobject->GetID());
        #endif
        return newobject;
    }

    int32 JsonScene::LoadShaders()
    {
        Application &app = Application::Get();
        auto &shaders = app.GetShaderStorage();
        auto &allocator = app.GetAllocator();

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
                path = Directories::Shader;
                path += vert_filename;
                vert = OpenFile(path.C_Str(), "r");

                path.Clear();
                path = Directories::Shader;
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
            assert(itr->IsString());
            filename = itr->GetString();
            StringID texture_id(filename, strlen(filename));

            texture = new (allocator.Alloc<Texture>()) Texture({0,0,0});
            
            path.Clear();
            path = Directories::Texture;
            path += filename;
            texture->id = LoadTexture(path.C_Str(), &texture->w, &texture->h);
            assert(texture->id);

            textures.Register(texture, (Uint32)texture_id);

            printf("tex : %s\n", filename);
        }

        return RET_SUCC;
    }

    int32 JsonScene::LoadAnimations()
    {
        Application &app = Application::Get();
        auto &allocator = app.GetAllocator();
        auto &animations = app.GetAnimationStorage();

        assert(m_doc.HasMember("Animations"));
        assert(m_doc["Animations"].IsArray());
        const auto loadlist = m_doc["Animations"].GetArray();

        Animation *newanim = nullptr;
        Math::IRect range = {0, 0, 0, 0};
        Math::Size count = {0, 0};
        const char *name = nullptr;
        const char *texture_id = nullptr;

        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto anim = itr->GetObject();
                assert(anim.HasMember("name"));
                assert(anim.HasMember("texture"));
                assert(anim.HasMember("range"));
                assert(anim.HasMember("count"));
                assert(anim.HasMember("eachtime"));
                assert(anim.HasMember("loop"));
                assert(anim["name"].IsString());
                assert(anim["texture"].IsString());
                assert(anim["range"].IsObject());
                assert(anim["count"].IsObject());
                assert(anim["eachtime"].IsFloat());
                assert(anim["loop"].IsBool());

                name = anim["name"].GetString();
                texture_id = anim["texture"].GetString();
                JsonParseMethods::ReadIRect(anim["range"].GetObject(), &range);
                JsonParseMethods::ReadSize(anim["count"].GetObject(), &count);

                newanim = new (allocator.Alloc<Animation>()) Animation();
                assert(newanim);
                newanim->SetSpritesRange(range);
                newanim->SetSpritesCount(count);
                newanim->SetTextureID(CompileTimeHash::runtime_hash(texture_id, strlen(texture_id)));
                newanim->SetEachTime(anim["eachtime"].GetFloat());
                newanim->SetLooping(anim["loop"].GetBool());

                animations.Register(newanim, CompileTimeHash::runtime_hash(name, strlen(name)));

                printf("anim : %s\n", name);
            }

        return RET_SUCC;
    }

    int32 JsonScene::CreateObjectStorages()
    {
        Application &app = Application::Get();
        auto &allocator = app.GetAllocator();

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

    int32 JsonScene::CreateObjects()
    {
        auto &factory = GameObjectFactory::GetGlobal();
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

    void OpenJsonFile(const FileIO &f, rapidjson::Document *doc)
    {
        assert(doc);
        doc->Parse(f.GetBuffer());
        assert(doc->IsObject());
    }

    int32 OpenJsonFile(const char *filename, rapidjson::Document *doc)
    {
        assert(doc);

        FILE *f = OpenFile(filename, "r");
        assert(f);

        size_t len_json = GetFileSize(f);
        void *mem_json = nullptr;
        
        FileIO json_io;
        mem_json = malloc(len_json+1);
        memset(mem_json, 0, len_json+1);
        if(RET_SUCC != json_io.Open(f, mem_json, len_json))
        {
            fprintf(stderr, "failed to read %s\n", filename);
            fclose(f);
            free(mem_json);
            return RET_FAILED;
        }

        doc->Parse(json_io.GetBuffer());
        assert(doc->IsObject());
        fclose(f);
        free(mem_json);
    }
}