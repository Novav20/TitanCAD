// src/simple.frag
#version 460 core

// Esta es la variable de salida que define el color final del píxel.
out vec4 FragColor;

// Nuevo uniform para recibir el color desde la CPU
uniform vec3 objectColor;

void main() {
    FragColor = vec4(objectColor, 1.0f);
}