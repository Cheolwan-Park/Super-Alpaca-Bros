#ifndef BaseJsonScene_hpp
#define BaseJsonScene_hpp

#include "Scene.hpp"
#include "Json.hpp"

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

        int32 SetJsonFile(FILE *f);

    protected:
        virtual int32 LoadShaders();
        virtual int32 LoadTextures();
        virtual int32 CreateObjectStorages();
        virtual int32 CreateDrawableStorages();

    private:
        rapidjson::Document m_doc;
    };
    
}

#endif