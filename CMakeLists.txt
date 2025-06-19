cmake_minimum_required(VERSION 3.20)

project(TitanCAD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Añadimos los subdirectorios a la ruta de inclusión
include_directories(src)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Definimos nuestro ejecutable con los nuevos archivos fuente
add_executable(MyCADApp
    src/main.cpp
    src/Core/Application.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Camera.cpp
)

target_link_libraries(MyCADApp PRIVATE 
    glfw
    glad::glad
    glm::glm
    imgui::imgui
)

# Comando de copia actualizado para la nueva carpeta de assets
add_custom_command(TARGET MyCADApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/assets/shaders"
        "$<TARGET_FILE_DIR:MyCADApp>/assets/shaders"
    COMMENT "Copying shaders to output directory"
)