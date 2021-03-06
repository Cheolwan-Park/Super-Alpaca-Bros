cmake_minimum_required( VERSION 3.0 )

project( super-alpaca-bros )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "module path: ${CMAKE_MODULE_PATH}")

add_compile_options( -Os )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -Os" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )

add_executable(
        super-alpaca-bros
        src/main.cpp
)

add_library(
        Game STATIC
        src/game/Alpaca.cpp
        src/game/AlpacaHead.cpp
        src/game/AlpacaAction.cpp
        src/game/AlpacaWalkSound.cpp
        src/game/Ground.cpp
        src/game/HitGauge.cpp
        src/game/AlpacaSpit.cpp
        src/game/GameManager.cpp
        src/game/CameraController.cpp
        src/game/OutsidedEffect.cpp
        src/game/CooltimeViewer.cpp
        src/game/Button.cpp
        src/game/Banner.cpp
        src/title/TitleBanner.cpp )
include_directories( src/title )
include_directories( src/game )
target_link_libraries( Game Base )
target_link_libraries( super-alpaca-bros Game )

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
        src/base/SDLMixer.cpp
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
        src/base/Physics.cpp
)
include_directories( src/base )
target_link_libraries( super-alpaca-bros Base )

# header only libraries
include_directories("include")

# sdl
find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )
find_package( SDL2_mixer REQUIRED ) 
include_directories( ${SDL2_INCLUDE_DIR}
                     ${SDL2_IMAGE_INCLUDE_DIR}
                     ${SDL2_MIXER_INCLUDE_DIR} )
target_link_libraries( Base ${SDL2_LIBRARY}
                            ${SDL2_IMAGE_LIBRARIES}
                            ${SDL2_MIXER_LIBRARIES} )

# opengl
find_package ( OpenGL REQUIRED )
if (OPENGL_FOUND )
    target_link_libraries ( Base ${OPENGL_LIBRARY} )
    target_link_libraries ( Base ${OPENGiL_gl_LIBRARY} )
    target_link_libraries ( Base ${OPENGL_glu_LIBRARY} )
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
    target_link_libraries( Base ${GLEW_LIBRARIES} )
endif()

# rapidjson
find_package( rapidjson REQUIRED )
if( RAPIDJSON_FOUND )
    include_directories( ${RAPIDJSON_INCLUDE_DIRS} )
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
endif()

# steamworks
find_package( STEAMWORKS REQUIRED )
if( STEAMWORKS_FOUND )
    include_directories( ${STEAMWORKS_INCLUDE_DIRS} )
    target_link_libraries( Base ${STEAMWORKS_LIBRARY} )
endif()