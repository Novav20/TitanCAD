// src/Core/Application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>

struct GLFWwindow; // Forward declaration

namespace Core {
    class Application {
    public:
        Application(const std::string& title);
        ~Application();

        // Deshabilitar copia y asignación
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run();

    private:
        // Métodos privados para organizar la lógica
        void setupScene();
        void processInput();
        void renderUI();
        void renderScene();

        // Callbacks estáticos para GLFW
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    private:
        GLFWwindow* m_window;
        int m_width;
        int m_height;
        // Usaremos punteros inteligentes para gestionar la vida de nuestros objetos
        // aunque por ahora los inicializamos directamente
    };
}

#endif