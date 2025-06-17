// src/simple.frag
#version 460 core

// Esta es la variable de salida que define el color final del píxel.
out vec4 FragColor;

void main() {
    // Simplemente establecemos un color fijo para cada píxel del triángulo.
    // Naranja: (R=1.0, G=0.5, B=0.2, A=1.0)
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}