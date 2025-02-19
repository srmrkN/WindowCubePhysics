#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>

struct PhysicsObject {
    btRigidBody* rigidBody;
    btCollisionShape* shape;
    btMotionState* motionState;
    int type; // 0 - куб, 1 - сфера, 2 - цилиндр
    glm::vec3 color;
};

struct PhysicsSettings {
    float forceScale = 0.01f;
    float horizontalScale = 1.0f;
    float verticalScale = 1.0f;
    float zScale = 1.0f;
    float maxForce = 10.0f;
    float torqueScale = 0.0002f;
    float maxTorque = 5.0f;
    float restitution = 0.5f;
    float friction = 0.5f;
    float rollingFriction = 0.1f;
    float spinningFriction = 0.4f;
    float damping = 0.1f;
    float massScale = 1.0f;
    glm::vec3 cubeColor = glm::vec3(0.8f, 0.3f, 0.2f);
};

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 10.0f, 17.5f);
    glm::vec3 front = glm::vec3(0.0f, -0.3f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    float yaw = -90.0f;    // Горизонтальный угол
    float pitch = -20.0f;  // Вертикальный угол
    float movementSpeed = 5.0f;
    float rotationSpeed = 50.0f;
};

struct Mesh {
    GLuint VAO, VBO, EBO;
    int indexCount;
};

// Глобальные константы
const float BOUNDARY_SIZE = 5.0f;