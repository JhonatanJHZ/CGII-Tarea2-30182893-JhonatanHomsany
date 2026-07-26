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
    if (ImGui::CollapsingHeader("Manejo de archivos")){
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
}

void UIManager::addOpacityManagementUI(Application* app){
    VolumeRenderer* applicationRenderer = app->volumeRenderer;
    if (ImGui::CollapsingHeader("Función de transferencia")){

        ImGui::SliderFloat("Gas (general)", &applicationRenderer->gasOpacityScale, 0.0f, 1.0f);
        ImGui::DragFloatRange2("Gas (intervalo)", &applicationRenderer->gasOpacityLowerLimit, &applicationRenderer->gasOpacityUpperLimit, 1.0f, 1.0f, 75.0f);
        ImGui::Spacing();

        ImGui::SliderFloat("Líquido (general)", &applicationRenderer->liquidOpacityScale, 0.0f, 1.0f);
        ImGui::DragFloatRange2("Líquido (intervalo)", &applicationRenderer->liquidOpacityLowerLimit, &applicationRenderer->liquidOpacityUpperLimit, 1.0f, 76.0f, 150.0f);
        ImGui::Spacing();

        ImGui::SliderFloat("Objetos (general)", &applicationRenderer->objectsOpacityScale, 0.0f, 1.0f);
        ImGui::DragFloatRange2("Objetos (intervalo)", &applicationRenderer->objectsOpacityLowerLimit, &applicationRenderer->objectsOpacityUpperLimit, 1.0f, 151.0f, 254.0f);
        ImGui::Spacing();

        ImGui::SliderFloat("Terreno", &applicationRenderer->terrainOpacityScale, 0.0f, 1.0f);
        ImGui::Spacing();
        ImGui::DragFloatRange2("Rango de densidad", &applicationRenderer->densityMin, &applicationRenderer->densityMax, 1.0f, 1.0f, 255.0f);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }
}

void UIManager::addVoxelSizeUI(Application* app){
    if (ImGui::CollapsingHeader("Controles de los vóxeles")){
        ImGui::Separator();
        ImGui::InputFloat("Tamaño de los vóxeles", &app->volumeRenderer->voxelSize);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }
}

void UIManager::addGizmoControlsUI(Application* app){
    if (ImGui::CollapsingHeader("Gizmo")){
        ImGui::Separator();
        ImGui::Checkbox("Mostrar ejes", &app->gizmoRenderer->showAxis);
        ImGui::Checkbox("Mostrar caja", &app->gizmoRenderer->showBoundingBox);
    }
}

void UIManager::addInsertObjectUI(Application* app){
    if (ImGui::CollapsingHeader("Inserción de objetos")){
        ImGui::Separator();    

        static int depthIndex = 2;                       
        ImGui::Combo("Profundidad", &depthIndex, "8 bits\0" "16 bits\0" "32 bits\0");
        int bitDepth = (depthIndex == 0) ? 8 : (depthIndex == 1) ? 16 : 32;

        ImGui::Text("Dimensiones del objeto");
        ImGui::InputInt("Dim X", &objectDimensionsX);
        ImGui::InputInt("Dim Y", &objectDimensionsY);
        ImGui::InputInt("Dim Z", &objectDimensionsZ);

        ImGui::Text("Posición del objeto");
        ImGui::InputInt("Pos X", &positionX);
        ImGui::InputInt("Pos Y", &positionY);
        ImGui::InputInt("Pos Z", &positionZ);

        ImGui::Text("Escala del objeto");
        ImGui::InputFloat("Scale X", &scaleX);
        ImGui::InputFloat("Scale Y", &scaleY);
        ImGui::InputFloat("Scale Z", &scaleZ);
    
        ImGui::ColorEdit3("Color del objeto", color);
        if(ImGui::Button("Insertar objeto")){
            if(app->volume){
                string path = app->fileManager->getFilePath();
                if (!path.empty()) {
                    Volume objectToBeInserted = Volume(app->fileManager->readVolume(path, objectDimensionsX, objectDimensionsY, objectDimensionsZ, bitDepth)); 
                    app->volume->insertObject(objectToBeInserted, positionX, positionY, positionZ, scaleX, scaleY, scaleZ, glm::vec3 (color[0], color[1], color[2]));
                    app->volumeRenderer->uploadVolume(*app->volume);
                }
            }
        }
    }
}

void UIManager::addObjectModificationUI(Application* app){
    if (ImGui::CollapsingHeader("Modificación de objetos insertados")) {
        ImGui::Separator();

        auto& objects = app->volume->getObjects();

        if (objects.empty()) {
            ImGui::Text("No hay objetos insertados.");
            return;
        }

        for (int i = 0; i < (int)objects.size(); i++) {
            std::string label = "Objeto " + std::to_string(i);
            if (ImGui::Selectable(label.c_str(), selectedObjectIndex == i))
                selectedObjectIndex = i;
        }

        if (selectedObjectIndex >= 0 && selectedObjectIndex < (int)objects.size()) {
            ObjectInstance& o = objects[selectedObjectIndex];

            ImGui::Separator();
            ImGui::DragFloat3("Posición##mod", &o.position.x, 1.0f);
            ImGui::DragFloat3("Escala##mod",   &o.scale.x, 0.05f, 0.1f, 8.0f);
            ImGui::ColorEdit3("Color##mod",    &o.color.x);

            if (ImGui::Button("Modificar objeto")) {
                app->volume->rebuildWorld();
                app->volumeRenderer->uploadVolume(*app->volume);
            }

            if (ImGui::Button("Eliminar objeto")) {
                objects.erase(objects.begin() + selectedObjectIndex);
                selectedObjectIndex = -1;
                app->volume->rebuildWorld();
                app->volumeRenderer->uploadVolume(*app->volume);
            }
        }
    }
}

void UIManager::addStoneburnerUI(Application* app){
    if (ImGui::CollapsingHeader("Stoneburner")){        
        ImGui::Separator();   
        ImGui::Checkbox("Modo stoneburner", &app->stoneburner->active);
        ImGui::Checkbox("Seguir dirección del rayo", &app->stoneburner->followRayDirection);
        ImGui::SliderFloat("Umbral de selección", &app->stoneburner->pickThreshold, 0.0f, 1.0f);
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
}

void UIManager::addProceduralGenerationUI(Application* app){
    if (ImGui::CollapsingHeader("Generar escena")){
        if (ImGui::Button("Generar escena procedural")) {
            app->volumeRenderer->generateProceduralVolume(app->volume);
            app->volumeRenderer->uploadVolume(*app->volume);
        }
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
    if(app->volume->getObjects().size() > 0){
        addObjectModificationUI(app);
    }
    addStoneburnerUI(app);
    addProceduralGenerationUI(app);
    ImGui::End();
}

void UIManager::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}