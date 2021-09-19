cmake_minimum_required(VERSION 3.11)
project(micro_ninja)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/tinysdl)
add_subdirectory(lib/json)
# add_subdirectory(lib/imgui)

add_executable(micro_ninja 
src/main.cpp
src/modules/game.cpp
src/core/scene.cpp
src/core/component.cpp
src/core/entity.cpp
src/input/input.cpp
src/input/virtualbutton.cpp
src/input/virtualaxis.cpp

 src/assets/content.cpp
 src/assets/tileset.cpp
 src/microninja.cpp 
 src/microninja.h
 src/level0.h
 src/level0.cpp
 src/composer.cpp
 src/components/tilemap.cpp
 src/components/animated_sprite.cpp
 src/components/collider.cpp
 src/components/collider_grid.cpp
 src/components/actor.cpp
 src/components/timer.cpp

 # ${IMGUI_SRC}
  "src/components/multitimer.h" "src/components/multitimer.cpp")


#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS} lib/imgui)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(micro_ninja 
tinysdl
nlohmann_json::nlohmann_json
)

# copy SDL2.dll to the build directory
set(SDL2_DLL "" CACHE FILEPATH "SDL2 DLL Path")
if (EXISTS ${SDL2_DLL})
    add_custom_command(
        TARGET micro_ninja POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_DLL} $<TARGET_FILE_DIR:micro_ninja>)
endif()
