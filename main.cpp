#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "types.h"
#include "render.h"
#include "camera.h"
#include "physics.h"
#include "gui.h"
#include "shaders.h"

// Глобальные переменные
std::vector<PhysicsObject> physicsObjects;
PhysicsSettings physicsSettings;
Camera camera;
glm::dvec2 lastWindowPos(0.0, 0.0);
glm::dvec2 windowVelocity(0.0, 0.0);
bool firstMove = true;
bool cursorEnabled = true;

// Callback для перемещения окна
void window_pos_callback(GLFWwindow* window, int xpos, int ypos) {
    if (!physicsObjects.empty()) {
        if (!firstMove) {
            windowVelocity = glm::dvec2(xpos - lastWindowPos.x, ypos - lastWindowPos.y);
        }
        lastWindowPos = glm::dvec2(xpos, ypos);
        firstMove = false;
    }
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

PhysicsObject createPhysicsObject(int type, const btVector3& position) {
    PhysicsObject obj;
    obj.type = type;
    
    // Создаем форму в зависимости от типа
    switch(type) {
        case 0: // куб
            obj.shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
            obj.color = physicsSettings.cubeColor;
            break;
        case 1: // сфера
            obj.shape = new btSphereShape(0.5f);
            obj.color = glm::vec3(0.2f, 0.8f, 0.3f);
            break;
        case 2: // цилиндр
            obj.shape = new btCylinderShape(btVector3(0.5f, 0.5f, 0.5f));
            obj.color = glm::vec3(0.3f, 0.2f, 0.8f);
            break;
    }

    btScalar mass = 1.0f;
    btVector3 inertia(0, 0, 0);
    obj.shape->calculateLocalInertia(mass, inertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(position);
    
    obj.motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, obj.motionState, obj.shape, inertia);
    
    // Настройка физических параметров
    rbInfo.m_restitution = physicsSettings.restitution;
    rbInfo.m_friction = physicsSettings.friction;
    
    obj.rigidBody = new btRigidBody(rbInfo);
    obj.rigidBody->setRollingFriction(physicsSettings.rollingFriction);
    obj.rigidBody->setSpinningFriction(physicsSettings.spinningFriction);
    obj.rigidBody->setDamping(physicsSettings.damping, physicsSettings.damping);
    obj.rigidBody->setActivationState(DISABLE_DEACTIVATION);
    
    return obj;
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Physics Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Инициализация GLEW
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    // Создание шейдеров
    GLuint shaderProgram = Renderer::createShaderProgram(vertexShaderSource, fragmentShaderSource);
    GLuint skyboxShader = Renderer::createShaderProgram(skyboxVertexShaderSource, skyboxFragmentShaderSource);

    // Инициализация физики
    PhysicsWorld physicsWorld;
    physicsWorld.init();
    physicsWorld.createBoundaryWalls();

    // Инициализация GUI
    GUI gui;
    gui.init(window);

    // Создание меша куба
    Mesh cubeMesh = Renderer::createCube();

    // Создание VAO для скайбокса
    GLuint skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    float skyboxVertices[] = {
        // передняя грань
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        // задняя грань
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        // левая грань
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        // правая грань
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        // нижняя грань
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        // верхняя грань
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f
    };

    GLuint skyboxVBO;
    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Настройка callbacks
    glfwSetWindowPosCallback(window, window_pos_callback);
    glfwSetErrorCallback(error_callback);

    // После создания шейдеров
    Meshes meshes;
    meshes.cube = Renderer::createCube();
    meshes.sphere = Renderer::createSphere(32, 32);
    meshes.cylinder = Renderer::createCylinder(32);

    // Основной цикл
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Обновление физики
        physicsWorld.stepSimulation(deltaTime);

        // Обработка камеры
        CameraController::processCamera(window, camera, deltaTime);
        glm::mat4 view = CameraController::getViewMatrix(camera);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

        // Очистка буфера
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Сначала рендерим скайбокс
        glUseProgram(skyboxShader);
        Renderer::renderSkybox(skyboxVAO, skyboxShader, view, projection);

        // Затем рендерим объекты
        glUseProgram(shaderProgram);
        for(const auto& obj : physicsObjects) {
            Renderer::renderPhysicsObject(obj, shaderProgram, meshes, view, projection, camera);
        }

        // Рендеринг каркаса границ
        Renderer::renderWireframeBox(shaderProgram, cubeMesh, BOUNDARY_SIZE);

        // Рендеринг GUI
        gui.beginFrame();

        gui.renderSettings(physicsSettings);
        gui.renderControls(
            [&](int type) {
                PhysicsObject obj = createPhysicsObject(type, btVector3(0, 2, 0));
                physicsWorld.addObject(obj);
                physicsObjects.push_back(obj);
            },
            [&]() {
                for(auto& obj : physicsObjects) {
                    physicsWorld.removeObject(obj);
                }
                physicsObjects.clear();
            }
        );

        // Применение сил к объектам
        if (!physicsObjects.empty() && glm::length(windowVelocity) > 0.1) {
            for(auto& obj : physicsObjects) {
                physicsWorld.applyForceToObject(obj, glm::vec2(windowVelocity), physicsSettings);
            }
            windowVelocity = glm::dvec2(0.0, 0.0); // Сбрасываем скорость после применения
        }

        gui.endFrame();

        // Обмен буферов
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка
    gui.cleanup();
    physicsWorld.cleanup();
    glDeleteProgram(shaderProgram);
    glDeleteProgram(skyboxShader);
    glDeleteVertexArrays(1, &cubeMesh.VAO);
    glDeleteBuffers(1, &cubeMesh.VBO);
    glDeleteBuffers(1, &cubeMesh.EBO);

    glfwTerminate();
    return 0;
}