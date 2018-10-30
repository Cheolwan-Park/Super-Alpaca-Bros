#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include <algorithm>
#include <functional>

namespace Base
{
    // Scene class
    Scene::Scene()
    :m_objstorages(4), m_drawablestorages(4), m_doc()
    {
        ;
    }
    
    Scene::~Scene()
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->~ObjectStorage();
            m_objstorages[i] = nullptr;
        }
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            m_drawablestorages[i]->~DrawableStorage();
            m_drawablestorages[i] = nullptr;
        }
    }
    
    int32 Scene::Init()
    {
        #ifndef NDEBUG
        printf("==== loading scene ====\n");
        #endif
        
        if(RET_SUCC != LoadShaders())
            return RET_FAILED;

        if(RET_SUCC != LoadTextures())
            return RET_FAILED;

        if(RET_SUCC != LoadAnimations())
            return RET_FAILED;

        if(RET_SUCC != CreateObjectStorages())
            return RET_FAILED;
        
        if(RET_SUCC != CreateDrawableStorages())
            return RET_FAILED;

        if(RET_SUCC != CreateObjects())
            return RET_FAILED;

        // m_doc.Clear();

        return RET_SUCC;
    }

    GameObject *Scene::GetObject(Uint32 hash)
    {
        GameObject *obj = nullptr;
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            obj = m_objstorages[i]->Get(hash);
            if(obj)
                return obj;
        }
        return nullptr;
    }

    GameObject *Scene::GetObject(Uint32 storage, Uint32 hash)
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            if(m_objstorages[i]->GetID() == storage)
            {
                return m_objstorages[i]->Get(hash);
            }
        }
        return nullptr;
    }

    void Scene::Update()
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->UpdateObjects();
        }
    }

    void Scene::Render()
    {
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            m_drawablestorages[i]->DrawDrawables();
        }
    }

    void Scene::CheckDeletedObjects()
    {
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            m_drawablestorages[i]->CheckDeletedDrawables();
        }
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->CheckAndDeleteObjects();
        }
    }

    void Scene::AddObjectStorage(ObjectStorage *storage)
    {
        assert(storage);
        m_objstorages.Append(storage);
        auto *front = m_objstorages.GetBuffer();
        auto *end = front + m_objstorages.GetCount();
        std::sort(front, end,
                  [](ObjectStorage *a, ObjectStorage *b) {
                      return (a->GetOrder() > b->GetOrder());
                  });
    }

    void Scene::AddDrawableStorage(DrawableStorage *storage)
    {
        assert(storage);
        m_drawablestorages.Append(storage);
        auto *front = m_drawablestorages.GetBuffer();
        auto *end = front + m_drawablestorages.GetCount();
        std::sort(front, end,
                  [](DrawableStorage* a, DrawableStorage *b) {
                      return (a->GetOrder()) > (b->GetOrder());
                  });
    }

    ObjectStorage *Scene::GetObjectStorage(Uint32 hash)
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            if(hash == m_objstorages[i]->GetID())
                return m_objstorages[i];
        }
        return nullptr;
    }

    DrawableStorage *Scene::GetDrawableStorage(Uint32 hash)
    {
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            if(hash == m_drawablestorages[i]->GetID())
                return m_drawablestorages[i];
        }
        return nullptr;
    }

    GameObject *Scene::AddGameObject(Uint32 storage, GameObject *gameobject)
    {
        assert(gameobject);
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            if(storage == m_objstorages[i]->GetID())
            {
                m_objstorages[i]->Register(gameobject);
                return gameobject;
            }
        }
        return nullptr;
    }

    int32 Scene::RegisterDrawable(Uint32 storage, Drawable *drawable)
    {
        assert(drawable);
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            if(storage == m_drawablestorages[i]->GetID())
            {
                m_drawablestorages[i]->Register(drawable);
                return RET_SUCC;
            }
        }
        return RET_FAILED;
    }
}














