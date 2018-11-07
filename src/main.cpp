#include "base.hpp"
#include "game.hpp"
using namespace Base;

int32 JsonAppInit(const char *filename);
void AddFactoryFuncs();

int main()
{
    SDL::InitSDL(nullptr);

    String128 path = Directories::res;
    path += "init.json";
    if(RET_FAILED == JsonAppInit(path.C_Str()))
        return -1;

    Application &app = Application::Get();
    app.Run();

    #ifndef NDEBUG 
    printf("%llubyte used\n", app.GetAllocator().GetTopMarker());
    #endif

    app.Release(nullptr);
    
    return 0;
}

GLint GLSetting()
{
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    return RET_SUCC;
}

int32 JsonAppInit(const char *filename)
{
    // Open JsonFile
    rapidjson::Document doc;
    OpenJsonFile(filename, &doc);
    
    // create window
    assert(doc.HasMember("title"));
    assert(doc.HasMember("windowsize"));
    assert(doc["title"].IsString());
    assert(doc["windowsize"].IsObject());

    SDL::Window window;
    Math::Size windowsize;
    JsonParseMethods::ReadSize(doc["windowsize"].GetObject(), &windowsize);
    window.Create(windowsize.w, windowsize.h, doc["title"].GetString());

    // Initialize application
    Application &app = Application::Get();
    if(RET_FAILED == app.Create(window, GLSetting))
        return RET_FAILED;

    // alloc memories
    assert(doc.HasMember("allocator_size"));
    assert(doc.HasMember("shaderstorage_size"));
    assert(doc.HasMember("texturestorage_size"));
    assert(doc.HasMember("animationstorage_size"));
    assert(doc["allocator_size"].IsInt());
    assert(doc["shaderstorage_size"].IsInt());
    assert(doc["texturestorage_size"].IsInt());
    assert(doc["animationstorage_size"].IsInt());

    StackAllocator &allocator = app.GetAllocator();
    auto &shaders = app.GetShaderStorage();
    auto &textures = app.GetTextureStorage();
    auto &animations = app.GetAnimationStorage();
    int shaders_size = doc["shaderstorage_size"].GetInt();
    int textures_size = doc["texturestorage_size"].GetInt();
    int animations_size = doc["animationstorage_size"].GetInt();
    
    allocator.ReAllocBuffer(doc["allocator_size"].GetInt());
    shaders.AssignMemory(allocator.Alloc<Storage<ShaderProgram>::Type>(shaders_size), shaders_size);
    textures.AssignMemory(allocator.Alloc<Storage<Texture>::Type>(textures_size), textures_size);
    animations.AssignMemory(allocator.Alloc<Storage<Animation>::Type>(animations_size), animations_size);

    // setting camera
    assert(doc.HasMember("camera_range"));
    assert(doc["camera_range"].IsObject());
    glm::vec2 camera_range(0.0f, 0.0f);
    JsonParseMethods::ReadVector2(doc["camera_range"].GetObject(), &camera_range);

    OrthoCamera *maincam = new (allocator.Alloc<OrthoCamera>()) OrthoCamera();
    maincam->SetView(glm::vec3(0.0f, 0.0f, 1.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));  
    maincam->SetProjection(-0.5f*camera_range.x, 0.5f*camera_range.x,
                           -0.5f*camera_range.y, 0.5f*camera_range.y,
                           0.1f, 8.0f);
    Camera::SetMain(maincam);

    // add factory funcs
    AddFactoryFuncs();

    // set scene
    assert(doc.HasMember("default_scene"));
    assert(doc["default_scene"].IsString());
    if(RET_FAILED == app.SetScene(doc["default_scene"].GetString()))
        return RET_FAILED;

    return RET_SUCC;
}

void AddFactoryFuncs()
{
    auto &allocator = Application::Get().GetAllocator();;

    // component factory
    auto &cfactory = ComponentFactory::GetGlobal();
    cfactory.AssignMemory(allocator.Alloc<ComponentFactory::Type>(4096), 4096);
    cfactory.AddFunction<Collider>(Collider::Factory);
    cfactory.AddFunction<CircleCollider>();
    cfactory.AddFunction<BoxCollider>();
    cfactory.AddFunction<Rigidbody>();
    cfactory.AddFunction<Component>(Component::Factory);
    cfactory.AddFunction<Drawable>(Drawable::Factory);
    cfactory.AddFunction<Sprite>();
    cfactory.AddFunction<AnimatedSprite>();
    cfactory.AddFunction<Game::Alpaca::Alpaca>();
    cfactory.AddFunction<Game::Alpaca::Head>();
    cfactory.AddFunction<Game::Ground>();
    cfactory.AddFunction<Game::HitGauge>();
    cfactory.AddFunction<Game::Alpaca::Spit>();
    cfactory.AddFunction<Game::GameManager>();
    cfactory.AddFunction<Game::CameraController>();

    // action factory
    auto &afactory = Game::Alpaca::ActionFactory::GetGlobal();
    afactory.AssignMemory(allocator.Alloc<Game::Alpaca::ActionFactory::Type>(128), 128);
    afactory.AddFunction<Game::Alpaca::Heading>();
    afactory.AddFunction<Game::Alpaca::Dash>();
    afactory.AddFunction<Game::Alpaca::Spitting>();
}
