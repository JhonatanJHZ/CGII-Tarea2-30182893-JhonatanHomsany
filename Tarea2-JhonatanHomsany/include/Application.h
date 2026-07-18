#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
struct GLFWwindow;
class GLFWManager;
class UIManager;
class FileManager;
class Volume;
class Shader;
class VolumeRenderer;
class Camera;
class GizmoRenderer;

using namespace std;

class Application {
public:
    GLFWwindow* window;
    GLFWManager* glfwManager;
    UIManager* uiManager;
    FileManager* fileManager;
    Volume* volume;
    VolumeRenderer* volumeRenderer;
    Camera* camera;
    GizmoRenderer* gizmoRenderer;

    bool init();
    void updateAndRender();
    void setVolume(Volume* newVolume);
    void cleanup();
    Application();
    ~Application();
    void run();
private:
    void handleKeyboardEvents(float deltaTime);
    void handleMouseEvents();
};
