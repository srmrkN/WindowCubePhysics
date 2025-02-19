#include "gui.h"

#include <glm/gtc/type_ptr.hpp>

GUI::GUI() : initialized(false) {}

GUI::~GUI() {
    cleanup();
}

void GUI::init(GLFWwindow* window) {
    if (!initialized) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Настройка стиля ImGui
        ImGui::StyleColorsDark();
        ImGui::GetStyle().WindowRounding = 5.0f;
        ImGui::GetStyle().FrameRounding = 3.0f;
        ImGui::GetStyle().GrabRounding = 3.0f;

        initialized = true;
    }
}

void GUI::cleanup() {
    if (initialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        initialized = false;
    }
}

void GUI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::renderSettings(PhysicsSettings& settings) {
    ImGui::Begin("Physics Settings");
    
    if (ImGui::CollapsingHeader("Force Settings")) {
        ImGui::SliderFloat("Force Scale", &settings.forceScale, 0.001f, 0.01f, "%.5f");
        ImGui::SliderFloat("Horizontal Force", &settings.horizontalScale, 0.1f, 2.0f);
        ImGui::SliderFloat("Vertical Force", &settings.verticalScale, 0.1f, 2.0f);
        ImGui::SliderFloat("Z Force", &settings.zScale, 0.1f, 2.0f);
    }
    
    if (ImGui::CollapsingHeader("Physics Properties")) {
        ImGui::SliderFloat("Restitution", &settings.restitution, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Friction", &settings.friction, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Rolling Friction", &settings.rollingFriction, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Air Resistance", &settings.damping, 0.0f, 1.0f, "%.2f");
    }
    
    if (ImGui::CollapsingHeader("Object Properties")) {
        ImGui::ColorEdit3("Cube Color", glm::value_ptr(settings.cubeColor));
        ImGui::SliderFloat("Mass Scale", &settings.massScale, 0.1f, 2.0f);
    }
    
    ImGui::End();
}

void GUI::renderControls(std::function<void(int)> spawnCallback, std::function<void()> clearCallback) {
    ImGui::Begin("Controls");
    
    if (ImGui::Button("Spawn Cube")) {
        spawnCallback(0);
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Spawn Sphere")) {
        spawnCallback(1);
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Spawn Cylinder")) {
        spawnCallback(2);
    }

    if (ImGui::Button("Clear All Objects")) {
        clearCallback();
    }
    
    ImGui::End();
}