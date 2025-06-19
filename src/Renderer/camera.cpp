#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target) : Target(target) {
    Position = position;
    Radius = glm::distance(Position, Target);
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Target, Up);
}

void Camera::ProcessMouseOrbit(float xoffset, float yoffset) {
    // Calcular ángulos de rotación usando la sensibilidad
    float yaw = -xoffset * OrbitSensitivity;
    float pitch = -yoffset * OrbitSensitivity;

    // Crear una matriz de rotación para yaw (eje Y del mundo)
    glm::mat4 rotYaw = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

    // Calcular el vector "derecha" de la cámara
    glm::vec3 right = glm::normalize(glm::cross(Position - Target, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    // Crear una matriz de rotación para pitch (eje "derecha" local)
    glm::mat4 rotPitch = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);

    // Aplicar rotaciones a la posición de la cámara
    Position = Target + glm::vec3(rotYaw * rotPitch * glm::vec4(Position - Target, 1.0f));
    
    updateCameraVectors();
}

void Camera::ProcessMousePan(float xoffset, float yoffset) {
    glm::vec3 right = glm::normalize(glm::cross(Position - Target, Up));
    glm::vec3 panUp = glm::normalize(glm::cross(right, Position - Target));

    // Usamos PanSensitivity y lo ajustamos por el radio para que el paneo sea más lento de lejos
    glm::vec3 panOffset = right * xoffset * PanSensitivity * Radius + panUp * yoffset * PanSensitivity * Radius;
    
    Position += panOffset;
    Target += panOffset;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    // Usamos ZoomSensitivity
    Radius -= yoffset * ZoomSensitivity;
    if (Radius < 1.0f) Radius = 1.0f;
    if (Radius > 100.0f) Radius = 100.0f;
    
    Position = Target + glm::normalize(Position - Target) * Radius;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Se asume un sistema de coordenadas donde Y es "arriba"
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
}
