#include "render.h"
#define _USE_MATH_DEFINES
#include <math.h>

// Если всё ещё не определено
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint Renderer::createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    // Компиляция вершинного шейдера
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Проверка компиляции вершинного шейдера
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        // В реальном приложении здесь должна быть обработка ошибок
    }

    // Компиляция фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Проверка компиляции фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        // В реальном приложении здесь должна быть обработка ошибок
    }

    // Создание и линковка программы
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Проверка линковки
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        // В реальном приложении здесь должна быть обработка ошибок
    }

    // Очистка
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void Renderer::renderWireframeBox(GLuint shaderProgram, const Mesh& cube, float boundarySize) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(boundarySize * 2));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(cube.VAO);
    glDrawElements(GL_TRIANGLES, cube.indexCount, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Mesh Renderer::createCube() {
    Mesh mesh;
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    float vertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        8, 9, 10,
        10, 11, 8,
        12, 13, 14,
        14, 15, 12,
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    mesh.indexCount = sizeof(indices)/sizeof(indices[0]);
    return mesh;
}

void Renderer::renderSkybox(GLuint skyboxVAO, GLuint skyboxShader, const glm::mat4& view, const glm::mat4& projection) {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(skyboxShader);

    // Удаляем translation из матрицы вида
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

Meshes createMeshes() {
    Meshes meshes;
    meshes.cube = Renderer::createCube();
    meshes.sphere = Renderer::createSphere(32, 32);
    meshes.cylinder = Renderer::createCylinder(32);
    return meshes;
}

void Renderer::renderPhysicsObject(const PhysicsObject& obj, GLuint shaderProgram, 
    const Meshes& meshes, const glm::mat4& view, const glm::mat4& projection, 
    const Camera& camera) {
    // Включаем тест глубины
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Отключаем смешивание (если оно было включено)
    glDisable(GL_BLEND);

    btTransform trans;
    obj.rigidBody->getMotionState()->getWorldTransform(trans);

    glm::mat4 model;
    trans.getOpenGLMatrix(glm::value_ptr(model));

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Изменяем позицию света для лучшего освещения
    glm::vec3 lightPos(5.0f, 5.0f, 5.0f);  // Дальше от объектов
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 viewPos = camera.position;

    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "cubeColor"), 1, glm::value_ptr(obj.color));

    // Выбираем нужный меш в зависимости от типа объекта
    const Mesh* currentMesh;
    switch(obj.type) {
        case 0:
            currentMesh = &meshes.cube;
            break;
        case 1:
            currentMesh = &meshes.sphere;
            break;
        case 2:
            currentMesh = &meshes.cylinder;
            break;
        default:
            currentMesh = &meshes.cube;
    }

    glBindVertexArray(currentMesh->VAO);
    glDrawElements(GL_TRIANGLES, currentMesh->indexCount, GL_UNSIGNED_INT, 0);
}

Mesh Renderer::createSphere(int latitudes, int longitudes) {
    Mesh mesh;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Генерация вершин сферы
    for(int lat = 0; lat <= latitudes; lat++) {
        float theta = lat * M_PI / latitudes;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for(int lon = 0; lon <= longitudes; lon++) {
            float phi = lon * 2 * M_PI / longitudes;
            float x = sinTheta * cos(phi);
            float y = cosTheta;
            float z = sinTheta * sin(phi);
            
            // Позиция
            vertices.push_back(x * 0.5f);
            vertices.push_back(y * 0.5f);
            vertices.push_back(z * 0.5f);
            
            // Нормаль (для сферы совпадает с позицией)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
    
    // Генерация индексов
    for(int lat = 0; lat < latitudes; lat++) {
        for(int lon = 0; lon < longitudes; lon++) {
            int first = lat * (longitudes + 1) + lon;
            int second = first + longitudes + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    // Создание буферов
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    
    glBindVertexArray(mesh.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    mesh.indexCount = indices.size();
    return mesh;
}

Mesh Renderer::createCylinder(int segments) {
    Mesh mesh;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Вершина пирамиды
    vertices.push_back(0.0f);    // x
    vertices.push_back(0.5f);    // y (вершина)
    vertices.push_back(0.0f);    // z
    vertices.push_back(0.0f);    // normal x
    vertices.push_back(1.0f);    // normal y
    vertices.push_back(0.0f);    // normal z

    // Основание пирамиды (квадрат)
    float baseSize = 0.4f;
    float height = 1.0f;
    
    // Вычисляем нормали для боковых граней
    float normalAngle = atan2(height, baseSize);
    float normalY = sin(normalAngle);
    float normalXZ = cos(normalAngle);

    // Передняя грань
    vertices.push_back(-baseSize); vertices.push_back(-0.5f); vertices.push_back(-baseSize);
    vertices.push_back(-normalXZ); vertices.push_back(normalY); vertices.push_back(-normalXZ);
    
    vertices.push_back(baseSize); vertices.push_back(-0.5f); vertices.push_back(-baseSize);
    vertices.push_back(normalXZ); vertices.push_back(normalY); vertices.push_back(-normalXZ);
    
    vertices.push_back(baseSize); vertices.push_back(-0.5f); vertices.push_back(baseSize);
    vertices.push_back(normalXZ); vertices.push_back(normalY); vertices.push_back(normalXZ);
    
    vertices.push_back(-baseSize); vertices.push_back(-0.5f); vertices.push_back(baseSize);
    vertices.push_back(-normalXZ); vertices.push_back(normalY); vertices.push_back(normalXZ);
    
    // Индексы для граней
    // Основание
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(4);
    
    // Боковые грани
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(4);
    
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(1);
    
    // Создание буферов
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    
    glBindVertexArray(mesh.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    mesh.indexCount = indices.size();
    return mesh;
}