#pragma once

#include "types.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <functional>

class GUI {
public:
    GUI();
    ~GUI();

    void init(GLFWwindow* window);
    void cleanup();
    void beginFrame();
    void endFrame();
    void renderSettings(PhysicsSettings& settings);
    void renderControls(std::function<void(int)> spawnCallback, std::function<void()> clearCallback);

private:
    bool initialized;
};