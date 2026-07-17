#include "../../include/Application.h"
#include "../../include/VolumeRenderer.h"
#include "../../include/Volume.h"
#include "../../include/tools/UIManager.h"
#include "../../include/tools/GLFWManager.h"
#include "../../include/tools/tinyfiledialogs.h"
#include "../../include/tools/FileManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
using namespace std;

void UIManager::addFrameLimitUI(GLFWManager* glfwManager){
    ImGui::Checkbox("Limitar tasa de frames (V-Sync)", &glfwManager->limitFrameRate);
    ImGui::Spacing();
}

void UIManager::addInstructionsUI(){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Controles");
    ImGui::Separator();
    ImGui::Text("Mover camara: WASD");
    ImGui::Text("Space para ascender, Shift para descender");
    ImGui::Text("Rotar camara: Mantener presionado Z o click derecho y mover mouse");
    ImGui::Text("Zoom: Rueda del mouse");
    ImGui::Text("Acercar/Alejar: Rueda del mouse");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void UIManager::addFileManagementUI(Application* app){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Cargar/guardar volumen");
    ImGui::Separator();
    if (ImGui::Button("Cargar volumen")) {
        std::string path = app->fileManager->getFilePath();
        if (!path.empty()) {
            app->setVolume(new Volume(app->fileManager->readVolume(path)));
            app->volumeRenderer->uploadVolume(*app->volume);
        }
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

UIManager::UIManager(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
UIManager::~UIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void UIManager::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void UIManager::drawInspector(Application* app, GLFWManager* glfwManager) {
    float targetWidth = 500.0f;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(targetWidth, viewport->WorkSize.y), ImGuiCond_Always);
    ImGui::Begin("Instrucciones de uso", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    addFrameLimitUI(glfwManager);
    addInstructionsUI();
    addFileManagementUI(app);
    ImGui::End();
}

void UIManager::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}