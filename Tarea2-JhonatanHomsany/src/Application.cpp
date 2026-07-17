#include "../include/Application.h"
#include "../include/tools/GLFWManager.h"
#include "../include/tools/UIManager.h"
#include "../include/tools/FileManager.h"
#include "../include/Volume.h"
#include "../include/VolumeRenderer.h"
#include "../include/Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>

Application::Application()
    : window(nullptr), glfwManager(nullptr), uiManager(nullptr), fileManager(nullptr), volume(nullptr), volumeRenderer(nullptr), camera(nullptr) {}
Application::~Application() {
    cleanup();
}

bool Application::init() {
    glfwManager = new GLFWManager();
    this->window = glfwManager->createWindow(800, 1000, "Tarea 2 - 30182893 - Jhonatan Homsany");
    if (!this->window) {
        return false;
    }
    glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
    uiManager = new UIManager(window);
    fileManager = new FileManager();
    camera = new Camera(glm::vec3(0.5f, 0.5f, 5.0f));
    volumeRenderer = new VolumeRenderer();
    volumeRenderer->init();
    setVolume(new Volume(fileManager->readVolume(
    "../../../Tarea2-JhonatanHomsany/assets/full/raw_world_512x128x512_rgba.raw")));
    volumeRenderer->uploadVolume(*volume);
    return true;
}

void Application::run() {
    if (!init()) {
        return;
    }

    while (!glfwManager->shouldClose()) {
        updateAndRender();
    }
}

void Application::updateAndRender() {
    static float lastFrame = 0.0f;
    float currentFrame = (float)glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    uiManager->newFrame();

    handleKeyboardEvents(deltaTime);
    handleMouseEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int w, h;
    glfwManager->getFrameBufferSize(&w, &h);
    if (h == 0) h = 1;
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix((float)w / (float)h);
    volumeRenderer->draw(view, projection, camera->getPosition());
    uiManager->drawInspector(this, this->glfwManager);
    uiManager->render();
    glfwManager->showFPS(this->window);
    glfwManager->update();
}

void Application::cleanup() {
    delete uiManager;
    uiManager = nullptr;
    delete volumeRenderer;
    volumeRenderer = nullptr;
    delete glfwManager;
    glfwManager = nullptr;
    delete fileManager;
    fileManager = nullptr;
    delete volume;
    volume = nullptr;
    delete camera;
    camera = nullptr;
}

void Application::handleKeyboardEvents(float deltaTime){
    ImGuiIO& io = ImGui::GetIO();

    if(!io.WantCaptureKeyboard){
        if(glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS){
            camera->movement(CameraMovement::FORWARD, deltaTime);
        }
        if(glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS){
            camera->movement(CameraMovement::BACKWARD, deltaTime);
        }
        if(glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS){
            camera->movement(CameraMovement::LEFT, deltaTime);
        }
        if(glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS){
            camera->movement(CameraMovement::RIGHT, deltaTime);
        }
        if(glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS){
            camera->movement(CameraMovement::UP, deltaTime);
        }
        if(glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            camera->movement(CameraMovement::DOWN, deltaTime);
        }
    }
};

void Application::handleMouseEvents(){
    static bool isMouseCaptured = false;
    static double lastMouseX = 0.0;
    static double lastMouseY = 0.0;

    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse && io.MouseWheel != 0.0f) {
        camera->zoom(io.MouseWheel);
    }

    if (glfwGetKey(this->window, GLFW_KEY_Z) == GLFW_PRESS || (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)) {
        if (!isMouseCaptured) {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(this->window, &lastMouseX, &lastMouseY);
            isMouseCaptured = true;
        } else {
            double mouseX, mouseY;
            glfwGetCursorPos(this->window, &mouseX, &mouseY);
            float xOffset = (float)(mouseX - lastMouseX);
            float yOffset = (float)(lastMouseY - mouseY);
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            camera->rotate(xOffset, yOffset);
        }
    } else {
        if (isMouseCaptured) {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            isMouseCaptured = false;
        }
    }
}