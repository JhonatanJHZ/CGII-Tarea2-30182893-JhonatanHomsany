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
        void addFrameLimitUI(GLFWManager* glfwManager);
        void addInstructionsUI();
        void addFileManagementUI(Application* app);
    public:
        UIManager(GLFWwindow* window);
        ~UIManager();
        void newFrame();
        void drawInspector(Application* app, GLFWManager* glfwManager);
        void render();
};