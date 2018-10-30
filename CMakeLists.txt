cmake_minimum_required( VERSION 3.0 )

project( alpacas )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "module path: ${CMAKE_MODULE_PATH}")

add_compile_options( -Os )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )

add_executable(
    alpacas 
    src/main.cpp
)

add_library(
    Game STATIC
    src/game/Alpaca.cpp
    src/game/AlpacaHead.cpp
    src/game/AlpacaAction.cpp
)
include_directories( src/game )
target_link_libraries( alpacas Game )

add_library(
    Base STATIC 
    src/base/Allocator.cpp 
    src/base/Application.cpp 
    src/base/Camera.cpp 
    src/base/FileIO.cpp 
    src/base/GameObject.cpp 
    src/base/GameObject.cpp 
    src/base/SDL.cpp 
    src/base/SDLInput.cpp 
    src/base/SDLWindow.cpp 
    src/base/Math.cpp 
    src/base/Scene.cpp 
    src/base/Schedule.cpp 
    src/base/ShaderProgram.cpp 
    src/base/Sprite.cpp 
    src/base/TextureStorage.cpp 
    src/base/Time.cpp 
    src/base/Vector.cpp 
    src/base/SceneLoadFunctions.cpp
    src/base/Json.cpp 
    src/base/Animation.cpp 
    src/base/AnimatedSprite.cpp 
    src/base/Component.cpp
    src/base/Collider.cpp
    # src/base/LuaPlayer.cpp
)
include_directories( src/base )
target_link_libraries( alpacas Base )

# header only libraries
include_directories("include")

# sdl
find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )
find_package( SDL2_ttf REQUIRED )
find_package( SDL2_mixer REQUIRED ) 
include_directories( ${SDL2_INCLUDE_DIR}
                     ${SDL2_IMAGE_INCLUDE_DIR}
                     ${SDL2_TTF_INCLUDE_DIR}
                     ${SDL2_MIXER_INCLUDE_DIR} )
target_link_libraries( alpacas ${SDL2_LIBRARY}
                                ${SDL2_IMAGE_LIBRARIES}
                                ${SDL2_TTF_LIBRARIES}
                                ${SDL2_MIXER_LIBRARIES} )

# opengl
find_package ( OpenGL REQUIRED )
if (OPENGL_FOUND )
    target_link_libraries ( alpacas ${OPENGL_gl_LIBRARY} )
    target_link_libraries ( alpacas ${OPENGL_glu_LIBRARY} )
endif()

# glm
find_package( GLM REQUIRED )
if( GLM_FOUND )
    include_directories( ${GLM_INCLUDE_DIRS} )
endif()

# glew 
find_package( GLEW REQUIRED )
if( GLEW_FOUND )
    include_directories( ${GLEW_INCLUDE_DIRECTORIES} )
    target_link_libraries( alpacas ${GLEW_LIBRARIES} )
endif()

# rapidjson
find_package( rapidjson REQUIRED )
if( RAPIDJSON_FOUND )
    include_directories( ${RAPIDJSON_INCLUDE_DIRS} )
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
endif()

# lua
find_package( lua REQUIRED )
if( LUA_FOUND )
    include_directories(${LUA_INCLUDE_DIR})
    target_link_libraries( alpacas ${LUA_LIBRARIES} )
endif()