#pragma once

#include "types.h"
#include <GLFW/glfw3.h>

class CameraController {
public:
    static void processCamera(GLFWwindow* window, Camera& camera, float deltaTime);
    static void updateCameraVectors(Camera& camera);
    static glm::mat4 getViewMatrix(const Camera& camera);
};