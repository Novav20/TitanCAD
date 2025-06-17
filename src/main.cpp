// --- src/main.cpp ---
#include <glad/glad.h>   // <-- ¡Importante! GLAD debe incluirse ANTES que GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h" // <-- INCLUIR NUESTRA CLASE SHADER

// --- Configuration Constants ---
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "TitanCAD v0.0.1";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // --- 1. Initialization ---
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // --- 2. Setting OpenGL Version and Profile ---
    // Best practice: Explicitly request a modern OpenGL Core Profile.
    // We're aiming for OpenGL 4.6 (muy moderno).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // --- 3. Window Creation ---
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // This line is CRUCIAL. It tells GLFW to make the context of our window the main
    // context on the current thread. All OpenGL calls will affect this window.
    glfwMakeContextCurrent(window);

    // --- 4. GLAD Initialization ---
    // We initialize GLAD *after* we have a window and an active OpenGL context.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // --- 5. Setting up the Viewport ---
    // The viewport is the part of the window where we will render.
    // For now, it's the full window size.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Best practice: Register a callback to resize the viewport if the user resizes the window.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- Shader Creation ---
    // Usamos rutas relativas a donde se ejecutará el programa (la carpeta build)
    Shader ourShader("src/simple.vert", "src/simple.frag");

    // --- Vertex Data & Buffers ---
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Vértice 1 (abajo izquierda)
         0.5f, -0.5f, 0.0f, // Vértice 2 (abajo derecha)
         0.0f,  0.5f, 0.0f  // Vértice 3 (arriba centro)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. "Enlazar" el Vertex Array Object
    glBindVertexArray(VAO);

    // 2. Copiar nuestros datos de vértices en el Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. Configurar los punteros a los atributos de vértice
    // Le decimos a OpenGL cómo interpretar los datos del VBO
    // Location = 0 (corresponde a 'aPos' en el vertex shader), 3 floats por vértice, etc.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Es seguro "desenlazar" el VBO ahora. El VAO recuerda este enlace.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // También desenlazamos el VAO para no modificarlo por accidente.
    glBindVertexArray(0);


    // --- 6. The Main Loop (Application Loop) ---
    while (!glfwWindowShouldClose(window)) {
        // --- Rendering ---
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ¡DIBUJAR NUESTRO TRIÁNGULO!
        ourShader.use();             // 1. Activar el programa de shader
        glBindVertexArray(VAO);      // 2. Enlazar el VAO que tiene nuestra configuración
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3. Dibujar! (modo, vértice inicial, número de vértices)

        // --- Swapping buffers and polling events ---
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- 7. Cleanup ---
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // El destructor de `ourShader` se encargará de `glDeleteProgram`.

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// Callback function definition
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // When the user resizes the window, update the OpenGL viewport.
    glViewport(0, 0, width, height);
}