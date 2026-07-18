# Force CMake Cache Refresh 1
cmake_minimum_required (VERSION 3.10)
project ("Tarea2-JhonatanHomsany")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("vendor/glad")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")
add_subdirectory("vendor/imgui")
add_subdirectory("vendor/tinygltf")

add_executable(Tarea2
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/Tarea2-JhonatanHomsany.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/Camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/tools/GLFWManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/Volume.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/VolumeRenderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/GizmoRenderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/tools/UIManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/tools/FileManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/src/tools/tinyfiledialogs.c"
)

set_target_properties(Tarea2 PROPERTIES
    OUTPUT_NAME "Tarea2-JhonatanHomsany"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(Tarea2 PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Tarea2-JhonatanHomsany/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinygltf"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
)

target_link_libraries(Tarea2 PRIVATE 
    glad_gl46
    glfw 
    glm 
    tinygltf
    imgui_lib
    Comdlg32
    Ole32
)