#include "../include/GizmoRenderer.h"
#include "../include/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

GizmoRenderer::GizmoRenderer()
    : shader(nullptr), VAO(0), VBO(0),
      showAxis(false), showBoundingBox(false) {};
      
GizmoRenderer::~GizmoRenderer(){
    cleanup();
};

void GizmoRenderer::init(){
    shader = new Shader("../../../Tarea2-JhonatanHomsany/shaders/line.vert", "../../../Tarea2-JhonatanHomsany/shaders/line.frag");
    float lineVertices[] = {
        -1,0,0, 1,0,0,   1,0,0, 1,0,0,   
        0,-1,0, 0,1,0,   0,1,0, 0,1,0,   
        0,0,-1, 0,0,1,   0,0,1, 0,0,1,   

        0,0,0, 1,1,0,   1,0,0, 1,1,0,
        1,0,0, 1,1,0,   1,0,1, 1,1,0,
        1,0,1, 1,1,0,   0,0,1, 1,1,0,
        0,0,1, 1,1,0,   0,0,0, 1,1,0,

        0,1,0, 1,1,0,   1,1,0, 1,1,0,
        1,1,0, 1,1,0,   1,1,1, 1,1,0,
        1,1,1, 1,1,0,   0,1,1, 1,1,0,
        0,1,1, 1,1,0,   0,1,0, 1,1,0,

        0,0,0, 1,1,0,   0,1,0, 1,1,0,
        1,0,0, 1,1,0,   1,1,0, 1,1,0,
        1,0,1, 1,1,0,   1,1,1, 1,1,0,
        0,0,1, 1,1,0,   0,1,1, 1,1,0,
    };
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);              
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
};

void GizmoRenderer::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& volumeScale){
    if (!shader) return;
    glDisable(GL_DEPTH_TEST);
    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    glLineWidth(5.0f);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);
    if (showAxis) {
        glm::vec3 center = volumeScale * 0.5f;          
        float axisLen = 2.0f;                           
        glm::mat4 axisModel = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), glm::vec3(axisLen));
        shader->setMat4("model", axisModel);
        glDrawArrays(GL_LINES, 0, 6);
    }
    
    glEnable(GL_DEPTH_TEST);
    if (showBoundingBox) {
        glm::mat4 boxModel = glm::scale(glm::mat4(1.0f), volumeScale);
        shader->setMat4("model", boxModel);
        glDrawArrays(GL_LINES, 6, 24);
    }
    glBindVertexArray(0);
};

void GizmoRenderer::cleanup(){
    delete shader;
    shader = nullptr;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
};