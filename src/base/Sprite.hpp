#ifndef BaseSprite_hpp
#define BaseSprite_hpp

#include "TextureStorage.hpp"
#include "GameObject.hpp"
#include "FileIO.hpp"
#include "Math.hpp"
#include "Storage.hpp"
#include <functional>

namespace Base
{
    class DrawableStorage;
    class ShaderProgram;
    
    class Drawable : public GameObject
    {
    public:
        MAKE_TYPE_ID(Drawable);

        Drawable() = delete;
        
        Drawable(Uint32 id, int32 isStatic = false);
        
        Drawable(Uint32 id, const GameObject *parent, int32 isStatic = false);

        Drawable(const rapidjson::GenericObject<true, rapidjson::Value> &obj);
        
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
        MAKE_TYPE_ID(Sprite);

        Sprite() = delete;
        
        Sprite(Uint32 id, int32 isStatic = false);
        
        Sprite(Uint32 id, const GameObject *parent, int32 isStatic = false);

        Sprite(const rapidjson::GenericObject<true, rapidjson::Value> &obj);
        
        Sprite(const Sprite &other);
        
        virtual ~Sprite();
        
        Sprite &operator=(const Sprite &other);
        
        virtual void Start();
        
        virtual void Update();
        
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
            GLuint vbo[2];
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
         * GameObject's flags
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
