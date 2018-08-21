#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include <algorithm>
#include <functional>

namespace Base
{
    // Scene class
    Scene::Scene()
    {
        ;
    }
    
    Scene::~Scene()
    {
        ;
    }
    
    int32 Scene::Init()
    {
        printf("==== loading scene ====\n");
        if(RET_SUCC != LoadShaders())
            return RET_FAILED;
        if(RET_SUCC != LoadTextures())
            return RET_FAILED;
        return RET_SUCC;
    }
    
    
    // ObjectScene class
    ObjectScene::ObjectScene()
    :Scene(), m_objstorages(4,2), m_drawablestorages(4,2)
    {
        ;
    }
    
    ObjectScene::~ObjectScene()
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
    
    int32 ObjectScene::Init()
    {
        if(RET_SUCC != Scene::Init())
            return RET_FAILED;

        if(RET_SUCC != CreateObjectStorages())
            return RET_FAILED;
        
        if(RET_SUCC != CreateDrawableStorages())
            return RET_FAILED;
        
        return RET_SUCC;
    }
    
    void ObjectScene::Update()
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->UpdateObjects();
        }
    }
    
    void ObjectScene::Render()
    {
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            m_drawablestorages[i]->DrawSprites();
        }
    }
    
    void ObjectScene::CheckDeletedObjects()
    {
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            m_drawablestorages[i]->CheckDeletedSprite();
        }
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->CheckAndDeleteObjects();
        }
    }
    
    void ObjectScene::AddObjectStorage(ObjectStorage *storage)
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
    
    void ObjectScene::AddDrawableStorage(DrawableStorage *storage)
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
    
    ObjectStorage *ObjectScene::GetObjectStorage(Uint32 hash)
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            if(hash == m_objstorages[i]->GetID())
                return m_objstorages[i];
        }
        return nullptr;
    }
    
    DrawableStorage *ObjectScene::GetDrawableStorage(Uint32 hash)
    {
        for(size_t i=0; i<m_drawablestorages.GetCount(); ++i)
        {
            if(hash == m_drawablestorages[i]->GetID())
                return m_drawablestorages[i];
        }
        return nullptr;
    }
}














