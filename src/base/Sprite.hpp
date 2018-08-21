#ifndef BaseSprite_hpp
#define BaseSprite_hpp

#include "TextureStorage.hpp"
#include "GameObject.hpp"
#include "FileIO.hpp"
#include "Math.hpp"
#include <functional>

namespace Base
{
    using Texture = TextureStorage::Val;
    
    class DrawableStorage;
    class ShaderProgram;
    
    class Drawable : public GameObject
    {
    public:
        Drawable() = delete;
        
        Drawable(Uint32 id, int32 isStatic = false);
        
        Drawable(Uint32 id, const GameObject *parent, int32 isStatic = false);
        
        Drawable(const Drawable &other);
        
        virtual ~Drawable();
        
        Drawable &operator=(const Drawable &other);
        
        virtual void Start();
        
        virtual void Update();
        
        virtual void Draw() = 0;
        
        virtual void UpdateVBO() = 0;
        
        // get
        const glm::vec2 &GetScale()const;
        
        float32 GetRotation()const;
        
        // set
        void SetScale(const glm::vec2 &val);
        
        void SetScale(float32 x, float32 y);
        
        void Scale(const glm::vec2 &val);
        
        void Scale(float32 x, float32 y);
        
        void Scale(float32 x);
        
        void SetRotation(float32 val);
        
        void Rotate(float32 delta);
        
        void SetDrawer(DrawableStorage *drawer);
        
    private:
        glm::vec2 m_scale;
        float32 m_rotation;
    };
    
    class Sprite : public Drawable
    {
    private:
        static const glm::vec3 verts[4];
        static const glm::vec2 uvs[4];
    public:
        Sprite() = delete;
        
        Sprite(Uint32 id, int32 isStatic = false);
        
        Sprite(Uint32 id, const GameObject *parent, int32 isStatic = false);
        
        Sprite(const Sprite &other);
        
        virtual ~Sprite();
        
        Sprite &operator=(const Sprite &other);
        
        virtual void Start();
        
        virtual void Update();
        
        virtual void Draw();
        
        virtual void UpdateVBO();
        
        // get
        float32 GetAlpha()const;
        
        const Math::Rect &GetUV()const;
        
        const Texture *GetTexture()const;
        
        // set
        void SetAlpha(float32 val);
        
        void SetUV(const Math::Rect &rect);
        
        void SetUV(const Math::IRect &rect);
        
        void SetTexture(const Texture *val);
        
    private:
        struct VAO
        {
            GLuint id;
            GLuint vbo[2];
        };
        void InitVAO();
        
        void ReleaseVAO();
        
    private:
        VAO m_vao;
        float32 m_alpha;
        Math::Rect m_uv;
        const Texture *m_tex;
    };
    
    class DrawableStorage
    {
    public:
        typedef Drawable* Type;
        
        DrawableStorage() = delete;
        
        DrawableStorage(Uint32 id, Uint32 order);
        
        DrawableStorage(const DrawableStorage &other) = delete;
        
        ~DrawableStorage();
        
        DrawableStorage &operator=(const DrawableStorage &other) = delete;
        
        void AssignMemory(void *memory, Uint32 len);
        
        void SetShader(const ShaderProgram *shader);
        
        void SetRenderSettingFun(const ::std::function<void(void)> &fun);
        
        int32 Register(Type sprite);
        
        const Type operator[](Uint32 id)const;
        
        Type operator[](Uint32 id);
        
        void DrawSprites();
        
        void CheckDeletedSprite();
        
        // get
        Uint32 GetID()const;
        
        Uint32 GetOrder()const;
        
    private:
        Uint32 m_id;
        Uint32 m_order;
        const ShaderProgram *m_shader;
        Uint32 m_sprites_len;
        Type *m_objects;
        std::function<void(void)> m_rendersetting;
    };
}

#endif
