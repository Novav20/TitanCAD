// src/Core/Application.cpp
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer/Shader.h"
#include "Renderer/Camera.h"

// --- Estructura para contener el estado de la escena ---
struct SceneData {
    std::unique_ptr<Shader> cubeShader;
    std::unique_ptr<Shader> gridShader;

    unsigned int cubeVAO, cubeVBO;
    unsigned int gridVAO, gridVBO;
    int gridVertexCount;
    
    float cubeColor[3] = {1.0f, 0.5f, 0.2f};
};

// --- Variables de estado estáticas para callbacks ---
// (Necesario porque los callbacks de C no pueden capturar 'this')
static Camera g_camera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
static float g_lastX;
static float g_lastY;
static bool g_firstMouse = true;

// Declaramos aquí la función de generar rejilla para mantenerla local a este archivo
std::vector<float> generateGridVertices(int size, int steps) {
    std::vector<float> vertices;
    float halfSize = (float)size / 2.0f;
    float stepSize = (float)size / (float)steps;
    for (int i = 0; i <= steps; ++i) {
        float pos = -halfSize + (float)i * stepSize;
        vertices.insert(vertices.end(), {pos, 0.0f, -halfSize, pos, 0.0f, halfSize});
        vertices.insert(vertices.end(), {-halfSize, 0.0f, pos, halfSize, 0.0f, pos});
    }
    return vertices;
}


namespace Core {
    Application::Application(const std::string& title)
        : m_width(1280), m_height(720) {
        
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glfwSetWindowUserPointer(m_window, this); // Guardamos 'this' en la ventana
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetCursorPosCallback(m_window, mouse_callback);
        glfwSetScrollCallback(m_window, scroll_callback);

        g_lastX = static_cast<float>(m_width) / 2.0f;
        g_lastY = static_cast<float>(m_height) / 2.0f;

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_width, m_height);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    Application::~Application() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Application::run() {
        SceneData scene;
        
        // --- Carga de Shaders ---
        scene.cubeShader = std::make_unique<Shader>("assets/shaders/simple.vert", "assets/shaders/simple.frag");
        scene.gridShader = std::make_unique<Shader>("assets/shaders/grid.vert", "assets/shaders/grid.frag");

        // --- Carga de Geometría ---
        float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        std::vector<float> gridVertices = generateGridVertices(50, 50);
        scene.gridVertexCount = gridVertices.size() / 3;

        glGenVertexArrays(1, &scene.cubeVAO);
        glGenBuffers(1, &scene.cubeVBO);
        glBindVertexArray(scene.cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, scene.cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenVertexArrays(1, &scene.gridVAO);
        glGenBuffers(1, &scene.gridVBO);
        glBindVertexArray(scene.gridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, scene.gridVBO);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // --- Bucle Principal ---
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            processInput();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            { ImGui::Begin("Tools"); ImGui::ColorEdit3("Cube Color", scene.cubeColor); ImGui::End(); }

            glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 1000.0f);
            glm::mat4 view = g_camera.GetViewMatrix();

            // Dibujar Cubo
            scene.cubeShader->use();
            scene.cubeShader->setMat4("projection", projection);
            scene.cubeShader->setMat4("view", view);
            scene.cubeShader->setMat4("model", glm::mat4(1.0f));
            scene.cubeShader->setVec3("objectColor", glm::vec3(scene.cubeColor[0], scene.cubeColor[1], scene.cubeColor[2]));
            glBindVertexArray(scene.cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Dibujar Rejilla
            scene.gridShader->use();
            scene.gridShader->setMat4("projection", projection);
            scene.gridShader->setMat4("view", view);
            scene.gridShader->setMat4("model", glm::mat4(1.0f));
            glBindVertexArray(scene.gridVAO);
            glDrawArrays(GL_LINES, 0, scene.gridVertexCount);

            glBindVertexArray(0);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_window);
        }

        // RAII para buffers
        glDeleteVertexArrays(1, &scene.cubeVAO);
        glDeleteVertexArrays(1, &scene.gridVAO);
        glDeleteBuffers(1, &scene.cubeVBO);
        glDeleteBuffers(1, &scene.gridVBO);
    }

    void Application::processInput() {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
    }

    // --- Implementación de Callbacks Estáticos ---
    void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->m_width = width;
            app->m_height = height;
        }
    }

    void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        if (g_firstMouse) { g_lastX = xpos; g_lastY = ypos; g_firstMouse = false; }
        float xoffset = xpos - g_lastX;
        float yoffset = g_lastY - ypos;
        g_lastX = xpos;
        g_lastY = ypos;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) g_camera.ProcessMouseOrbit(xoffset, yoffset);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) g_camera.ProcessMousePan(xoffset, yoffset);
    }

    void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        g_camera.ProcessMouseScroll(yoffset);
    }
}