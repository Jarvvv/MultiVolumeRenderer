include(FetchContent)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.79
        GIT_SHALLOW true
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_ROOT ${CMAKE_BINARY_DIR}/_deps/imgui-src)

set(IMGUI_INCLUDE_DIR ${IMGUI_ROOT})

set(IMGUI_LIB ${IMGUI_ROOT}/imgui-build)

aux_source_directory(${IMGUI_ROOT} IMGUI_SRC)

add_library(imgui STATIC ${IMGUI_SRC}
        ${IMGUI_ROOT}/examples/imgui_impl_glfw.cpp
        ${IMGUI_ROOT}/examples/imgui_impl_opengl3.cpp)
target_include_directories(imgui PRIVATE ${IMGUI_ROOT})