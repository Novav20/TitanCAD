// src/simple.vert
#version 460 core

// Este es un "atributo" de vértice. Recibe la posición del VBO (layout (location = 0)).
layout (location = 0) in vec3 aPos;

void main() {
    // gl_Position es una variable especial de salida que determina
    // la posición final del vértice en el espacio de la pantalla.
    // Por ahora, simplemente pasamos la posición de entrada directamente.
    gl_Position = vec4(aPos, 1.0);
}