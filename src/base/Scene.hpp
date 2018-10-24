#ifndef BaseScene_hpp
#define BaseScene_hpp

#include "Application.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Allocator.hpp"
#include "types.hpp"
#include "Array.hpp"

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

        virtual GameObject *GetObject(Uint32 hash) = 0;

        virtual GameObject *GetObject(Uint32 storage, Uint32 hash) = 0;
        
        virtual void Update() = 0;
        
        virtual void Render() = 0;
        
        virtual void CheckDeletedObjects() = 0;
        
    protected:
        virtual int32 LoadShaders() = 0;
        virtual int32 LoadTextures() = 0;
        virtual int32 LoadAnimations() = 0;
    };
    
    class ObjectScene : public Scene
    {
    public:
        ObjectScene();
        
        ObjectScene(const ObjectScene &other) = delete;
        
        virtual ~ObjectScene();
        
        ObjectScene &operator=(const ObjectScene &other) = delete;
        
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

        GameObject *AddGameObject(Uint32 storagehash, GameObject *gameobject);

        int32 RegisterDrawable(Uint32 storagehash, Drawable *drawable);
        
    protected:
        virtual int32 CreateObjectStorages() = 0;
        virtual int32 CreateDrawableStorages() = 0;
        virtual int32 CreateObjects() = 0;
        
    private:
        Array<ObjectStorage*> m_objstorages;
        Array<DrawableStorage*> m_drawablestorages;
    };

    
}

#endif
