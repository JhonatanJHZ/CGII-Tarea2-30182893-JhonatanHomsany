#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cstddef>
#include <vector>
#include <string>
struct GLFWwindow;
class Application;
class GLFWManager;

class UIManager {
    private:
        int dimX = 512, dimY = 128, dimZ = 512;
        int objectDimensionsX = 64, objectDimensionsY = 64, objectDimensionsZ = 64;
        int positionX = 0, positionY = 0, positionZ = 0;
        float color[3] = { 1.0f, 0.0f, 0.0f}; 

        void addFrameLimitUI(GLFWManager* glfwManager);
        void addInstructionsUI();
        void addFileManagementUI(Application* app);
        void addOpacityManagementUI(Application* app);
        void addVoxelSizeUI(Application* app);
        void addGizmoControlsUI(Application* app);
        void addInsertObjectUI(Application* app);
        void addStoneburnerUI(Application* app);

    public:
        UIManager(GLFWwindow* window);
        ~UIManager();
        void newFrame();
        void drawInspector(Application* app, GLFWManager* glfwManager);
        void render();
};