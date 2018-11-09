#ifndef BaseScene_hpp
#define BaseScene_hpp

#include "Application.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Allocator.hpp"
#include "types.hpp"
#include "Array.hpp"
#include "Json.hpp"
#include "FileIO.hpp"
#include "Physics.hpp"
#include "Camera.hpp"


namespace Base
{
    class Scene
    {
    public:
        Scene();
        
        Scene(const Scene &other) = delete;
        
        virtual ~Scene();
        
        Scene &operator=(const Scene &other) = delete;
        
        virtual int32 Init();

        virtual GameObject *GetObject(Uint32 hash);

        virtual GameObject *GetObject(Uint32 storage, Uint32 hash);
        
        virtual void Update();
        
        virtual void Render();
        
        virtual void CheckDeletedObjects();

        void AddObjectStorage(ObjectStorage *storage);
        
        void AddDrawableStorage(DrawableStorage *storage);
        
        ObjectStorage *GetObjectStorage(Uint32 hash);
        
        DrawableStorage *GetDrawableStorage(Uint32 hash);

        Camera *GetCamera(Uint32 hash);

        Physics &GetPhysics();

        GameObject *AddGameObject(Uint32 storagehash, GameObject *gameobject);

        int32 RegisterDrawable(Uint32 storagehash, Drawable *drawable);

        void SetJsonFile(const FileIO &f);

        int32 SetJsonFile(const char *filename);

        GameObject *CreateGameObject(const rapidjson::Value::Object &obj);
        
    protected:
        virtual int32 LoadPhysics();
        virtual int32 LoadCameras();
        virtual int32 LoadShaders();
        virtual int32 LoadTextures();
        virtual int32 LoadAnimations();
        virtual int32 CreateObjectStorages();
        virtual int32 CreateDrawableStorages();
        virtual int32 CreateObjects();

    private:
        Array<ObjectStorage*> m_objstorages;
        Array<DrawableStorage*> m_drawablestorages;
        Array<Camera*> m_cameras;
        Physics m_physics;
        rapidjson::Document m_doc;
    };
}

#endif
