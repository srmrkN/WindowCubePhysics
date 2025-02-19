#pragma once

#include "types.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

struct Meshes {
    Mesh cube;
    Mesh sphere;
    Mesh cylinder;
};

class Renderer {
public:
    static GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
    static void renderWireframeBox(GLuint shaderProgram, const Mesh& cube, float boundarySize);
    static Mesh createCube();
    static Mesh createSphere(int latitudes, int longitudes);
    static Mesh createCylinder(int segments);
    static void renderSkybox(GLuint skyboxVAO, GLuint skyboxShader, const glm::mat4& view, const glm::mat4& projection);
    static void renderPhysicsObject(const PhysicsObject& obj, GLuint shaderProgram, 
        const Meshes& meshes, const glm::mat4& view, const glm::mat4& projection, 
        const Camera& camera);
};