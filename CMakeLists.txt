# --- CMakeLists.txt ---
cmake_minimum_required(VERSION 3.20)

project(TitanCAD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Busca el paquete GLFW que instalamos con vcpkg
# El 'CONFIG' le dice que busque la configuración de CMake que vcpkg crea
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(MyCADApp 
src/main.cpp
src/shader.cpp
)

# Enlaza nuestro ejecutable con la librería GLFW
# 'glfw' es el nombre del 'target' que el paquete nos da
target_link_libraries(MyCADApp PRIVATE
    glfw
    glad::glad
)