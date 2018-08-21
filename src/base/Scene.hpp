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
        
        virtual void Update() = 0;
        
        virtual void Render() = 0;
        
        virtual void CheckDeletedObjects() = 0;
        
    protected:
        virtual int32 LoadShaders() = 0;
        virtual int32 LoadTextures() = 0;
    };
    
    class ObjectScene : public Scene
    {
    public:
        ObjectScene();
        
        ObjectScene(const ObjectScene &other) = delete;
        
        virtual ~ObjectScene();
        
        ObjectScene &operator=(const ObjectScene &other) = delete;
        
        virtual int32 Init();
        
        virtual void Update();
        
        virtual void Render();
        
        virtual void CheckDeletedObjects();
        
        void AddObjectStorage(ObjectStorage *storage);
        
        void AddDrawableStorage(DrawableStorage *storage);
        
        ObjectStorage *GetObjectStorage(Uint32 hash);
        
        DrawableStorage *GetDrawableStorage(Uint32 hash);
        
    protected:
        virtual int32 CreateObjectStorages() = 0;
        virtual int32 CreateDrawableStorages() = 0;
        
    private:
        Array<ObjectStorage*> m_objstorages;
        Array<DrawableStorage*> m_drawablestorages;
    };
}

#endif
