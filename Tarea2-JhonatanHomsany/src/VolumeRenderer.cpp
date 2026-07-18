#include "../include/Shader.h"
#include "../include/Volume.h"
#include "../include/VolumeRenderer.h"
#include <glad/glad.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
using namespace std;

VolumeRenderer::VolumeRenderer()
    : VAO(0), VBO(0), textureID(0), shader(nullptr) {};
VolumeRenderer::~VolumeRenderer(){
    cleanup();
};

void VolumeRenderer::init(){
    shader = new Shader("../../../Tarea2-JhonatanHomsany/shaders/volume.vert",
                        "../../../Tarea2-JhonatanHomsany/shaders/volume.frag");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    static const float cubeVertices[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};

void VolumeRenderer::uploadVolume(const Volume& volume){
    if(volume.getVoxels().empty()){
        return;
    }
    if (textureID == 0){
        glGenTextures(1, &textureID);
    } 

    glBindTexture(GL_TEXTURE_3D, textureID); 
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, volume.getX(), volume.getY(), volume.getZ(), 0, GL_RGBA, GL_UNSIGNED_BYTE, volume.getVoxels().data());
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_3D, 0);

    float maxDim = std::max(volume.getX(), std::max(volume.getY(), volume.getZ()));
    volumeScale = glm::vec3(volume.getX(), volume.getY(), volume.getZ()) / maxDim;
};

void VolumeRenderer::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos){
    if (!shader || VAO == 0) return;
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, textureID);
    shader->setInt("volumeTex", 0);
    shader->setVec3("cameraPosition", cameraPos);
    glm::mat4 model = glm::scale(glm::mat4(1.0f), volumeScale);  
    shader->setMat4("model", model);
    shader->setVec3("volumeScale", volumeScale);                 
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setFloat("gasOpacityScale", gasOpacityScale);
    shader->setFloat("liquidOpacityScale", liquidOpacityScale);
    shader->setFloat("objectsOpacityScale", objectsOpacityScale);
    shader->setFloat("terrainOpacityScale", terrainOpacityScale);
    shader->setFloat("voxelSize", voxelSize);
    shader->setFloat("densityMin", densityMin);
    shader->setFloat("densityMax", densityMax);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
};

void VolumeRenderer::cleanup(){
    delete shader;
    shader = nullptr;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureID);
};