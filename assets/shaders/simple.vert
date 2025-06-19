// src/simple.vert
#version 460 core

// Este es un "atributo" de vértice. Recibe la posición del VBO (layout (location = 0)).
layout (location = 0) in vec3 aPos;

// Nuestros uniforms para las matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}