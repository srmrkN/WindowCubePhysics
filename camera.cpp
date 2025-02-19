#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

void CameraController::processCamera(GLFWwindow* window, Camera& camera, float deltaTime) {
    float velocity = camera.movementSpeed * deltaTime;

    // Движение вперед/назад
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= camera.front * velocity;

    // Движение влево/вправо
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= camera.right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += camera.right * velocity;

    // Движение вверх/вниз
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.position += camera.up * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.position -= camera.up * velocity;

    // Вращение камеры
    float rotationVelocity = camera.rotationSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.yaw -= rotationVelocity;
        updateCameraVectors(camera);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.yaw += rotationVelocity;
        updateCameraVectors(camera);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.pitch += rotationVelocity;
        if(camera.pitch > 89.0f)
            camera.pitch = 89.0f;
        updateCameraVectors(camera);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.pitch -= rotationVelocity;
        if(camera.pitch < -89.0f)
            camera.pitch = -89.0f;
        updateCameraVectors(camera);
    }
}

void CameraController::updateCameraVectors(Camera& camera) {
    // Вычисляем новый вектор направления
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(front);
    
    // Пересчитываем правый и верхний векторы
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0.0f, 1.0f, 0.0f)));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}

glm::mat4 CameraController::getViewMatrix(const Camera& camera) {
    return glm::lookAt(camera.position, camera.position + camera.front, camera.up);
}