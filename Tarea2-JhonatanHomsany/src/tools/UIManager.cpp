#include "../../include/Application.h"
#include "../../include/VolumeRenderer.h"
#include "../../include/GizmoRenderer.h"
#include "../../include/Volume.h"
#include "../../include/tools/UIManager.h"
#include "../../include/tools/StoneburnerManager.h"
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
    ImGui::NewLine();
    ImGui::Text("Dimensiones del volumen");
    ImGui::InputInt("X", &dimX);
    ImGui::InputInt("Y", &dimY);
    ImGui::InputInt("Z", &dimZ);
    ImGui::Separator();
    
    if (ImGui::Button("Cargar volumen")) {
        if(app->volume){
            string path = app->fileManager->getFilePath();
            if (!path.empty()) {
                app->setVolume(new Volume(app->fileManager->readVolume(path, dimX, dimY, dimZ)));
                app->volumeRenderer->uploadVolume(*app->volume);
            }
        }
    }

    if(ImGui::Button("Guardar volumen")){
        string path = app->fileManager->getSavePath();
        if(!path.empty()){
            app->fileManager->saveRawFile(path, app->volume);
        }
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void UIManager::addOpacityManagementUI(Application* app){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Función de transferencia");
    ImGui::NewLine();
    ImGui::SliderFloat("Gas", &app->volumeRenderer->gasOpacityScale, 0.0f, 1.0f);
    ImGui::SliderFloat("Liquido", &app->volumeRenderer->liquidOpacityScale, 0.0f, 1.0f);
    ImGui::SliderFloat("Objetos", &app->volumeRenderer->objectsOpacityScale, 0.0f, 1.0f);
    ImGui::SliderFloat("Terreno", &app->volumeRenderer->terrainOpacityScale, 0.0f, 1.0f);
    ImGui::Spacing();
    ImGui::DragFloatRange2("Rango de densidad", &app->volumeRenderer->densityMin, &app->volumeRenderer->densityMax, 0.005f, 0.0f, 1.0f);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void UIManager::addVoxelSizeUI(Application* app){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Tamaño de los voxeles");
    ImGui::Separator();
    ImGui::InputFloat("Tamaño de los voxeles", &app->volumeRenderer->voxelSize);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void UIManager::addGizmoControlsUI(Application* app){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Controles del gizmo");
    ImGui::Separator();
    ImGui::Checkbox("Mostrar ejes", &app->gizmoRenderer->showAxis);
    ImGui::Checkbox("Mostrar caja", &app->gizmoRenderer->showBoundingBox);
}

void UIManager::addInsertObjectUI(Application* app){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Insertar objeto");
    ImGui::Separator();    

    ImGui::Text("Dimensiones del objeto");
    ImGui::InputInt("Dim X", &objectDimensionsX);
    ImGui::InputInt("Dim Y", &objectDimensionsY);
    ImGui::InputInt("Dim Z", &objectDimensionsZ);

    ImGui::Text("Posición del objeto");
    ImGui::InputInt("Pos X", &positionX);
    ImGui::InputInt("Pos Y", &positionY);
    ImGui::InputInt("Pos Z", &positionZ);
 
    ImGui::ColorEdit3("Color del objeto", color);
    if(ImGui::Button("Insertar objeto")){
        if(app->volume){
            string path = app->fileManager->getFilePath();
            if (!path.empty()) {
                Volume objectToBeInserted = Volume(app->fileManager->readVolume(path, objectDimensionsX, objectDimensionsY, objectDimensionsZ)); 
                app->volume->insertObject(objectToBeInserted, positionX, positionY, positionZ, glm::vec3 (color[0], color[1], color[2]));
                app->volumeRenderer->uploadVolume(*app->volume);
            }
        }
    }
}

void UIManager::addStoneburnerUI(Application* app){
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Modo Stoneburner");
    ImGui::Separator();   
    ImGui::Checkbox("Modo stoneburner", &app->stoneburner->active);
    ImGui::InputInt("Centro X", &app->stoneburner->selectedVoxelX);
    ImGui::InputInt("Centro Y", &app->stoneburner->selectedVoxelY);
    ImGui::InputInt("Centro Z", &app->stoneburner->selectedVoxelZ);
    ImGui::InputInt("Radio", &app->stoneburner->actionRadius);
    ImGui::DragFloatRange2("Rango alfa", &app->stoneburner->alphaLowerLimit, &app->stoneburner->alphaUpperLimit, 0.005f, 0.0f, 1.0f);
    if (ImGui::Button("Ejecutar")) {
        app->stoneburner->destructVoxels(app->volume);
        app->volumeRenderer->uploadVolume(*app->volume);
    }
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
    addOpacityManagementUI(app);
    addVoxelSizeUI(app);
    addGizmoControlsUI(app);
    addInsertObjectUI(app);
    addStoneburnerUI(app);
    ImGui::End();
}

void UIManager::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}