#ifndef BaseJsonScene_hpp
#define BaseJsonScene_hpp

#include "Scene.hpp"
#include "Json.hpp"
#include "FileIO.hpp"

namespace Base
{
    class JsonScene : public ObjectScene
    {
    public:
        JsonScene();

        JsonScene(const JsonScene &other) = delete;

        virtual ~JsonScene();

        JsonScene &operator=(const JsonScene &other) = delete;

        virtual int32 Init();

        void SetJsonFile(const FileIO &f);

        int32 SetJsonFile(const char *filename);

        GameObject *CreateGameObject(const rapidjson::Value::Object &obj);

    protected:
        virtual int32 LoadShaders();
        virtual int32 LoadTextures();
        virtual int32 LoadAnimations();
        virtual int32 CreateObjectStorages();
        virtual int32 CreateDrawableStorages();
        virtual int32 CreateObjects();

    private:
        rapidjson::Document m_doc;
    };

    void OpenJsonFile(const FileIO &f, rapidjson::Document *doc);
    int32 OpenJsonFile(const char *filename, rapidjson::Document *doc);
    
}

#endif