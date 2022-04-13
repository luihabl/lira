set(IMGUI_ROOT
    lib/imgui
)

set(IMGUI_INCLUDE_DIRS
    ${IMGUI_ROOT}
    ${IMGUI_ROOT}/backends
)

set(IMGUI_SRC
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_tables.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/imgui_demo.cpp

    ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_sdl.cpp
)

