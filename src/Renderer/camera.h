#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Atributos de la cámara
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;
    float Radius; // Distancia desde el Target

    // Controles de sensibilidad
    float OrbitSensitivity = 0.25f; // Reducimos la sensibilidad de órbita (antes era 1.0)
    float PanSensitivity   = 0.002f; // Un valor más fino para el paneo
    float ZoomSensitivity  = 1.0f;   // La sensibilidad del zoom suele estar bien en 1.0

    // Constructor
    Camera(glm::vec3 position, glm::vec3 target);

    // Devuelve la matriz de vista (View Matrix)
    glm::mat4 GetViewMatrix();

    // Procesa la entrada del ratón
    void ProcessMouseOrbit(float xoffset, float yoffset);
    void ProcessMousePan(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

private:
    // Recalcula los vectores de la cámara cuando la posición cambia
    void updateCameraVectors();
};

#endif
