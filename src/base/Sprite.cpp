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
    Component *Drawable::Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator, GameObject *gameobject)
    {
        fprintf(stderr, "Drawable is abstract class\n");
        return nullptr;
    }

    Drawable::Drawable()
    :Component()
    {
        ;
    }

    Drawable::Drawable(const Drawable &other)
    :Component(other)
    {
        ;
    }
    
    Drawable::~Drawable()
    {
        ;
    }
    
    Drawable &Drawable::operator=(const Drawable &other)
    {
        assert(this != &other);
        Component::operator=(other);
        return (*this);
    }

    void Drawable::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Component::InitWithJson(obj, allocator);

        assert(obj.HasMember("storage"));
        assert(obj["storage"].IsString());

        const char *storagename = obj["storage"].GetString();
        Uint32 storageid = CompileTimeHash::runtime_hash(storagename, strlen(storagename));

        if("none"_hash != storageid)
        {
            Scene *scene = Application::Get().GetScene();
            assert(scene);
            DrawableStorage *storage = scene->GetDrawableStorage(storageid);
            assert(storage);
            storage->Register(this);
        }
    }
    
    void Drawable::Start()
    {
        Component::Start();
    }
    
    void Drawable::Update()
    {
        ;
    }

    void Drawable::Release()
    {
        ;
    }

    void Drawable::SetDrawer(DrawableStorage *drawer)
    {
        drawer->Register(this);
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
        {0.0f, 1.0f},   // top left
        {1.0f, 1.0f},   // top right
        {0.0f, 0.0f},   // bottom left
        {1.0f, 0.0f}    // bottom right
    };

    Sprite::Sprite()
    :Drawable(), m_vao(), m_uv(), m_tex(nullptr)
    { 
        ;
    }
    
    Sprite::Sprite(const Sprite &other)
    :Drawable(other), m_vao(), m_uv(other.m_uv), m_tex(other.m_tex)
    {
        ;
    }
    
    Sprite::~Sprite()
    {
        ;
    }
    
    Sprite &Sprite::operator=(const Sprite &other)
    {
        assert(this != &other);
        Drawable::operator=(other);
        this->m_uv = other.m_uv;
        this->m_tex = other.m_tex;
        return (*this);
    }

    void Sprite::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Drawable::InitWithJson(obj, allocator);
        
        assert(obj.HasMember("uv"));
        assert(obj.HasMember("texture"));
        assert(obj["uv"].IsObject());
        assert(obj["texture"].IsString());

        Application &app = Application::Get();
        auto &textures = app.GetTextureStorage();
        const char *texture_id = obj["texture"].GetString();
        m_tex = textures[CompileTimeHash::runtime_hash(texture_id, strlen(texture_id))];

        Math::IRect irect({0, 0, 0, 0});
        JsonParseMethods::ReadIRect(obj["uv"].GetObject(), &irect);
        SetUV(irect);
    }
    
    void Sprite::Start()
    {
        InitVAO();
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

    void Sprite::Release()
    {
        ReleaseVAO();
        Drawable::Release();
    }
    
    void Sprite::Draw()
    {
        static const glm::vec3 zbias(0.0f, 0.0f, 1.0f);
        
        if(nullptr != m_tex && isAvailable())
        {
            glBindVertexArray(m_vao.id);
            
            if(false == GetGameObject()->isStatic())
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
        glm::mat3x3 model(1.0f);
        GetGameObject()->GetModel(&model);
        for(Uint32 i=0; i<4; ++i)
        {
            vert_buffer[i] = model * verts[i];
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
        if(GetGameObject()->isStatic())
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
    :m_id(id), m_order(order), m_shader(nullptr), m_rendersetting(DefaultRenderSettingFun), 
    m_len(0), m_drawables(nullptr)
    {
        ;
    }
    
    DrawableStorage::~DrawableStorage()
    {
        ;
    }

    void DrawableStorage::AssignMemory(void *mem, Uint32 len)
    {
        m_len = len;
        m_drawables = (Drawable**)mem;
        memset(m_drawables, 0, sizeof(Type)*len);
    }
    
    void DrawableStorage::SetShader(ShaderProgram *shader)
    {
        m_shader = shader;
    }
    
    void DrawableStorage::SetRenderSettingFun(const std::function<void(void)> &fun)
    {
        m_rendersetting = fun;
    }
    
    Drawable *DrawableStorage::Register(Drawable *drawable)
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(!m_drawables[i])
            {
                m_drawables[i] = drawable;
                return drawable;
            }
        }
        fprintf(stderr, "there is no space in DrawableStorage\n");
        exit(-1);
        return nullptr;
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
            
            for(Uint32 i=0; i<m_len; ++i)
            {
                if(m_drawables[i] && m_drawables[i]->isAvailable())
                {
                    m_drawables[i]->Draw();
                }
            }
        }
    }
    
    void DrawableStorage::CheckDeletedDrawables()
    {
        for(Uint32 i=0; i<m_len; ++i)
            {
                if(m_drawables[i] && m_drawables[i]->GetGameObject()->isDeleted())
                {
                    m_drawables[i] = nullptr;
                }
            }
    }
    
    Uint32 DrawableStorage::GetID()const
    {
        return m_id;
    }
    
    Uint32 DrawableStorage::GetOrder()const
    {
        return m_order;
    }
}










