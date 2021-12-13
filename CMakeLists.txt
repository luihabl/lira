cmake_minimum_required(VERSION 3.11)
project(lira)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall") #-Wextra
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W3")
endif()

add_subdirectory(lib/tinysdl)
add_subdirectory(lib/json)
# add_subdirectory(lib/imgui)

add_executable(lira 

src/modules/game.cpp

src/core/scene.cpp
src/core/component.cpp
src/core/entity.cpp

src/input/input.cpp
src/input/virtualbutton.cpp
src/input/virtualaxis.cpp

src/assets/content.cpp
src/assets/tileset.cpp
src/assets/spritesheet.h
src/assets/map.cpp

src/components/tilemap.cpp
src/components/animated_sprite.cpp
src/components/collider.cpp
src/components/collider_grid.cpp
src/components/actor.cpp
src/components/timer.cpp
src/components/multitimer.h
src/components/multitimer.cpp

# ${IMGUI_SRC}

src/composer.cpp
"src/lira.cpp" 
"src/lira.h"
src/level.h
src/level.cpp
src/main.cpp
   "src/collision_layers.h")


#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS} lib/imgui)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(lira 
tinysdl
nlohmann_json::nlohmann_json
)

# copy SDL2.dll to the build directory
set(SDL2_DLL "" CACHE FILEPATH "SDL2 DLL Path")
if (EXISTS ${SDL2_DLL})
    add_custom_command(
        TARGET lira POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_DLL} $<TARGET_FILE_DIR:lira>)
endif()
