#include "Sprite.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Application.hpp"
#include "Scene.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Base
{
    // Drawable class
    Drawable::Drawable(Uint32 id, int32 isStatic)
    :GameObject(id, isStatic), m_scale(0.0f, 0.0f), m_rotation(0.0f)
    {
        ;
    }
    
    Drawable::Drawable(Uint32 id, const GameObject *parent, int32 isStatic)
    :GameObject(id, parent, isStatic), m_scale(0.0f, 0.0f), m_rotation(0.0f)
    {
        ;
    }

    Drawable::Drawable(const rapidjson::GenericObject<true, rapidjson::Value> &obj)
    :GameObject(obj), m_scale(0.0f, 0.0f), m_rotation(0.0f)
    {
        assert(obj.HasMember("Drawable"));
        assert(obj["Drawable"].IsObject());
        auto dobj = obj["Drawable"].GetObject();

        assert(dobj.HasMember("scale"));
        assert(dobj.HasMember("rotation"));
        assert(dobj["scale"].IsObject());
        assert(dobj["rotation"].IsFloat());

        JsonParseMethods::ReadVector2(dobj["scale"].GetObject(), &m_scale);
        m_rotation = dobj["rotation"].GetFloat();
    }
    

    Drawable::Drawable(const Drawable &other)
    :GameObject(other), m_scale(other.m_scale), m_rotation(other.m_rotation)
    {
        ;
    }
    
    Drawable::~Drawable()
    {
        ;
    }
    
    Drawable &Drawable::operator=(const Drawable &other)
    {
        GameObject::operator=(other);
        this->m_scale = other.m_scale;
        this->m_rotation = other.m_rotation;
        return (*this);
    }
    
    void Drawable::Start()
    {
        GameObject::Start();
    }
    
    void Drawable::Update()
    {
        GameObject::Update();
    }
    
    const glm::vec2 &Drawable::GetScale()const
    {
        return m_scale;
    }
    
    float32 Drawable::GetRotation()const
    {
        return m_rotation;
    }
    
    void Drawable::SetScale(const glm::vec2 &val)
    {
        m_scale = val;
    }
    
    void Drawable::SetScale(float32 x, float32 y)
    {
        m_scale.x = x;
        m_scale.y = y;
    }
    
    void Drawable::Scale(const glm::vec2 &val)
    {
        m_scale.x *= val.x;
        m_scale.y *= val.y;
    }
    
    void Drawable::Scale(float32 x, float32 y)
    {
        m_scale.x *= x;
        m_scale.y *= y;
    }
    
    void Drawable::Scale(float32 x)
    {
        m_scale *= x;
    }
    
    void Drawable::SetRotation(float32 val)
    {
        m_rotation = val;
    }
    
    void Drawable::Rotate(float32 delta)
    {
        m_rotation += delta;
    }
    
    void Drawable::SetDrawer(DrawableStorage *drawer)
    {
        drawer->Register(this, GetID());
    }
    
    
    
    // Sprite class
    const glm::vec3 Sprite::verts[4] =
    {
        {-0.5f,  0.5f, 1.0f},   // top left
        { 0.5f,  0.5f, 1.0f},   // top right
        {-0.5f, -0.5f, 1.0f},   // bottom left
        { 0.5f, -0.5f, 1.0f}    // bottom right
    };
    
    const glm::vec2 Sprite::uvs[4] =
    {
        {0.0f, 0.0f},   // top left
        {1.0f, 0.0f},   // top right
        {0.0f, 1.0f},   // bottom left
        {1.0f, 1.0f}    // bottom right
    };
    
    Sprite::Sprite(Uint32 id, int32 isStatic)
    :Drawable(id, isStatic), m_vao({0,0,0}),
    m_uv({0.0f, 0.0f, 1.0f, 1.0f}), m_tex(nullptr)
    {
        InitVAO();
    }
    
    Sprite::Sprite(Uint32 id, const GameObject *parent, int32 isStatic)
    :Drawable(id, parent, isStatic), m_vao({0,0,0}),
    m_uv({0.0f, 0.0f, 1.0f, 1.0f}), m_tex(nullptr)
    {
        InitVAO();
    }
    
    Sprite::Sprite(const rapidjson::GenericObject<true, rapidjson::Value> &obj)
    :Drawable(obj), m_vao({0,0,0}), m_uv({0.0f, 0.0f, 1.0f, 1.0f}), m_tex(nullptr)
    {
        InitVAO();
        
        assert(obj.HasMember("Sprite"));
        assert(obj["Sprite"].IsObject());
        auto sobj = obj["Sprite"].GetObject();

        assert(sobj.HasMember("uv"));
        assert(sobj.HasMember("texture"));
        assert(sobj["uv"].IsObject());
        assert(sobj["texture"].IsString());

        Application &app = Application::Get();
        auto &textures = app.GetTextureStorage();
        const char *texture_id = sobj["texture"].GetString();
        m_tex = textures[CompileTimeHash::runtime_hash(texture_id, strlen(texture_id))];

        Math::IRect irect({0, 0, 0, 0});
        JsonParseMethods::ReadIRect(sobj["uv"].GetObject(), &irect);
        SetUV(irect);
    }

    Sprite::Sprite(const Sprite &other)
    :Drawable(other), m_vao({0,0,0}),
    m_uv(other.m_uv), m_tex(other.m_tex)
    {
        InitVAO();
    }
    
    Sprite::~Sprite()
    {
        ReleaseVAO();
    }
    
    Sprite &Sprite::operator=(const Sprite &other)
    {
        Drawable::operator=(other);
        this->m_tex = other.m_tex;
        return (*this);
    }
    
    void Sprite::Start()
    {
        glBindVertexArray(m_vao.id);
        UpdateVBO();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        Drawable::Start();
    }
    
    void Sprite::Update()
    {
        Drawable::Update();
    }
    
    void Sprite::Draw()
    {
        static const glm::vec3 zbias(0.0f, 0.0f, 1.0f);
        
        if(nullptr != m_tex && isAvailable())
        {
            glBindVertexArray(m_vao.id);
            
            if(false == isStatic())
                UpdateVBO();
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_tex->id);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
    }
    
    void Sprite::UpdateVBO()
    {
        if(NeedUpdateUV())
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[1]);
            glm::vec2 *uv_buffer = (glm::vec2*)glMapBufferRange(GL_ARRAY_BUFFER,
                                                                0, sizeof(uvs),
                                                                GL_MAP_WRITE_BIT |
                                                                GL_MAP_FLUSH_EXPLICIT_BIT |
                                                                GL_MAP_UNSYNCHRONIZED_BIT);
            float32 right = m_uv.x + m_uv.w;
            float32 top = m_uv.y + m_uv.h;
            uv_buffer[0] = {m_uv.x,top};
            uv_buffer[1] = {right, top};
            uv_buffer[2] = {m_uv.x, m_uv.y};
            uv_buffer[3] = {right, m_uv.y};
            glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, sizeof(uvs));
            glUnmapBuffer(GL_ARRAY_BUFFER);
            SetNeedUpdateUV(false);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[0]);
        glm::vec3 *vert_buffer = (glm::vec3*)glMapBufferRange(GL_ARRAY_BUFFER,
                                                         0, sizeof(verts),
                                                         GL_MAP_WRITE_BIT |
                                                         GL_MAP_FLUSH_EXPLICIT_BIT |
                                                         GL_MAP_UNSYNCHRONIZED_BIT);
        glm::mat3x3 model = glm::mat3x3(1.0f);
        glm::vec2 worldpos = GetWorldPosition();
        model = glm::translate(model, worldpos);
        model = glm::rotate(model, GetRotation());
        model = glm::scale(model, GetScale());
        for(Uint32 i=0; i<4; ++i)
        {
            vert_buffer[i] = model*verts[i];
        }
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, sizeof(verts));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    
    void Sprite::InitVAO()
    {
        if(0 != m_vao.id)
            return;
        
        glGenVertexArrays(1, &m_vao.id);
        glBindVertexArray(m_vao.id);
        glGenBuffers(2, m_vao.vbo);
        
        // check static
        GLenum usage = GL_DYNAMIC_DRAW;
        if(isStatic())
            usage = GL_STATIC_DRAW;
            
        
        // vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, usage);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        // texcoord buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, usage);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void Sprite::ReleaseVAO()
    {
        if(0 == m_vao.id)
            return;
        glDeleteBuffers(2, m_vao.vbo);
        glDeleteVertexArrays(1, &m_vao.id);
    }
    
    const Math::Rect &Sprite::GetUV()const
    {
        return m_uv;
    }
    
    const Texture *Sprite::GetTexture()const
    {
        return m_tex;
    }
    
    void Sprite::SetUV(const Math::Rect &rect)
    {
        m_uv = rect;
        SetNeedUpdateUV(true);
    }
    
    void Sprite::SetUV(const Math::IRect &rect)
    {
        m_uv.x = (float32)rect.x/(m_tex->w);
        m_uv.w = (float32)rect.w/(m_tex->w);
        m_uv.y = (float32)rect.y/(m_tex->h);
        m_uv.h = (float32)rect.h/(m_tex->h);
        SetNeedUpdateUV(true);
    }
    
    void Sprite::SetTexture(const Texture *val)
    {
        m_tex = val;
    }

    int32 Sprite::NeedUpdateUV()const
    {
        return m_flags.GetFlag(9);
    }

    void Sprite::SetNeedUpdateUV(int32 val)
    {
        m_flags.SetFlag(9, val);
    }

    
    // SpriteDrawer class
    
    void DefaultRenderSettingFun()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    DrawableStorage::DrawableStorage(Uint32 id, Uint32 order)
    :Storage<Drawable>(),  m_id(id), m_order(order), 
    m_shader(nullptr), m_rendersetting(DefaultRenderSettingFun)
    {
        ;
    }
    
    DrawableStorage::~DrawableStorage()
    {
        Clear();
    }
    
    void DrawableStorage::SetShader(const ShaderProgram *shader)
    {
        m_shader = shader;
    }
    
    void DrawableStorage::SetRenderSettingFun(const std::function<void(void)> &fun)
    {
        m_rendersetting = fun;
    }
    
    void DrawableStorage::DrawDrawables()
    {
        if(nullptr != m_shader)
        {
            glUseProgram(m_shader->GetProgram());
            m_rendersetting();
            
            // set vp uniform
            Camera *maincam = Camera::GetMain();
            const glm::mat4 &proj = maincam->GetProjectionMatrix();
            const glm::mat4 &view = maincam->GetViewMatrix();
            glm::mat4 vp = proj * view;
            glUniformMatrix4fv(m_shader->GetViewProjectionLocation(),
                               1, GL_FALSE,
                               glm::value_ptr(vp));
            glUniform1i(m_shader->GetTextureLocation(), 0);
            
            ForDo(DrawDrawable);
        }
    }
    
    void DrawableStorage::CheckDeletedDrawables()
    {
        ForDo(CheckDeleteDrawable);
    }
    
    Uint32 DrawableStorage::GetID()const
    {
        return m_id;
    }
    
    Uint32 DrawableStorage::GetOrder()const
    {
        return m_order;
    }

    void DrawableStorage::DrawDrawable(Drawable **drawable)
    {
        if((*drawable)->isAvailable())
            (*drawable)->Draw();
    }

    void DrawableStorage::CheckDeleteDrawable(Drawable **drawable)
    {
        if((*drawable)->isDeleted())
            (*drawable) = nullptr;
    }
}










