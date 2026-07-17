#include "../../include/tools/GLFWManager.h"
#include <iomanip>
#include <sstream>
using namespace std;
GLFWManager::GLFWManager(){
    if (!glfwInit()) {
        cerr << "Fallo al inicializar GLFW" << endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
}
GLFWManager::~GLFWManager(){
    glfwTerminate();
}
void GLFWManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
GLFWwindow* GLFWManager::createWindow(int width, int height, const char* title){
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Fallo al inicializar GLAD" << endl;
        return nullptr;
    }

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, GLFWManager::framebuffer_size_callback);
    return window;
}

void GLFWManager::showFPS(GLFWwindow* window)
{
    static double lastTime = glfwGetTime();
    static int nbFrames = 0;

    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if ( delta >= 1.0 ){
        double fps = double(nbFrames) / delta;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << fps;
        std::string title = "Tarea 2 - 30182893 - Jhonatan Homsany FPS: " + ss.str();
        glfwSetWindowTitle(window, title.c_str());
        nbFrames = 0;
        lastTime = currentTime;
     }
}

void GLFWManager::getFrameBufferSize(int* width, int* height) {
    glfwGetFramebufferSize(window, width, height);
}
void GLFWManager::update() {
    glfwSwapInterval(limitFrameRate ? 1 : 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
bool GLFWManager::shouldClose() {
    return glfwWindowShouldClose(window);
}