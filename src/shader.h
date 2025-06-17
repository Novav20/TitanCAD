// src/shader.h
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    // El ID del programa de shader
    unsigned int ID;

    // El constructor lee y construye el shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // Destructor para limpiar los recursos
    ~Shader();

    // Activa el shader
    void use();

private:
    // Función de utilidad para comprobar errores de compilación/enlace.
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif