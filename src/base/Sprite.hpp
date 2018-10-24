#ifndef BaseSprite_hpp
#define BaseSprite_hpp

#include "TextureStorage.hpp"
#include "FileIO.hpp"
#include "Math.hpp"
#include "Component.hpp"
#include "Storage.hpp"
#include <functional>

namespace Base
{
    class DrawableStorage;
    class ShaderProgram;
    
    class Drawable : public Component
    {
    public:
        static Component *Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator);

    public:
        COMPONENT(Drawable);

        Drawable();
        
        // not copying id
        Drawable(const Drawable &other);
        
        virtual ~Drawable();
        
        // not copying id
        Drawable &operator=(const Drawable &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);
        
        virtual void Start();
        
        virtual void Update();

        virtual void Release();
        
        virtual void Draw() = 0;
        
        virtual void UpdateVBO() = 0;
        
        void SetDrawer(DrawableStorage *drawer);

        Uint32 GetID()const;
        
    private:
        Uint32 m_id;
    };
    
    class Sprite : public Drawable
    {
    private:
        static const glm::vec3 verts[4];
        static const glm::vec2 uvs[4];

    public:
        COMPONENT(Sprite);

        Sprite();
        
        Sprite(const Sprite &other);
        
        virtual ~Sprite();
        
        Sprite &operator=(const Sprite &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);
        
        virtual void Start();
        
        virtual void Update();

        virtual void Release();
        
        virtual void Draw();
        
        virtual void UpdateVBO();
        
        // get
        const Math::Rect &GetUV()const;
        
        const Texture *GetTexture()const;
        
        // set
        void SetUV(const Math::Rect &rect);
        
        void SetUV(const Math::IRect &rect);
        
        void SetTexture(const Texture *val);
        
    private:
        struct VAO
        {
            GLuint id;
            GLuint vbo[4];
        };
        void InitVAO();
        
        void ReleaseVAO();

        int32 NeedUpdateUV()const;

        void SetNeedUpdateUV(int32 val);
        
    private:
        VAO m_vao;
        Math::Rect m_uv;
        const Texture *m_tex;
        /* flags
         * Drawable's flags
         * 9 : update uv
         */
    };
    
    class DrawableStorage : public Storage<Drawable>
    {
    public:
        typedef Drawable* Type;
        
        DrawableStorage() = delete;
        
        DrawableStorage(Uint32 id, Uint32 order);
        
        DrawableStorage(const DrawableStorage &other) = delete;
        
        ~DrawableStorage();
        
        DrawableStorage &operator=(const DrawableStorage &other) = delete;
        
        void SetShader(const ShaderProgram *shader);
        
        void SetRenderSettingFun(const ::std::function<void(void)> &fun);
        
        void DrawDrawables();
        
        void CheckDeletedDrawables();
        
        // get
        Uint32 GetID()const;
        
        Uint32 GetOrder()const;

    private:
        static void DrawDrawable(Drawable **drawable);
        static void CheckDeleteDrawable(Drawable **drawable);
        
    private:
        Uint32 m_id;
        Uint32 m_order;
        const ShaderProgram *m_shader;
        std::function<void(void)> m_rendersetting;
    };
}

#endif
