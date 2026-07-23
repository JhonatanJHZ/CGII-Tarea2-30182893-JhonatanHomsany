#include "../include/Shader.h"
#include "../include/Volume.h"
#include "../include/VolumeRenderer.h"
#include <glad/glad.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <random>
#include <iostream>
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

void VolumeRenderer::generateProceduralVolume(Volume* volume) {
    if (!volume) return;

    srand(static_cast<unsigned int>(std::time(nullptr)));
    int seed = rand();

    int X = volume->getX(), Y = volume->getY(), Z = volume->getZ();
    std::vector<Voxel> voxels(X * Y * Z);

    int waterLevel = Y / 3;
    float frequency = 0.02f;

    for (int z = 0; z < Z; z++) {
        for (int x = 0; x < X; x++) {
            float value = generateNoise(x * frequency, z * frequency, seed);
            int height = (int)(Y * 0.3f + value * (Y * 0.4f));
            height = std::min(std::max(height, 1), Y - 1);

            float tint = generateNoise(x * 0.1f, z * 0.1f, seed + 999);
            uint8_t grassG = (uint8_t)(120 + tint * 50);          

            for (int y = 0; y < Y; y++) {
                Voxel v = {0, 0, 0, 0};                           
                if (y < height) {
                    int depth = height - y;                       
                    if (depth <= 2) {
                        if (height <= waterLevel + 2)
                            v = {194, 178, 128, 255};             
                        else
                            v = {70, grassG, 55, 255};            
                    } else {
                        v = {134, 96, 67, 255};                    
                    }
                } else if (y < waterLevel) {
                    v = {40, 90, 200, (uint8_t)(76 + rand() % 75)};
                }
                voxels[x + y*X + z*X*Y] = v;
            }
        }
    }

    int numDeposits = 12;
    for (int d = 0; d < numDeposits; d++) {
        int ox = rand() % X, oz = rand() % Z;
        float value = generateNoise(ox * frequency, oz * frequency, seed);
        int surf = std::min(std::max((int)(Y * 0.3f + value * (Y * 0.4f)), 1), Y - 1);
        int oy = surf;
        int r = 3 + rand() % 4;
        Voxel objectVoxel = {200, 120, 40, (uint8_t)(151 + rand() % 104)};
        for (int y = oy - r; y <= oy + r; y++)
            for (int z = oz - r; z <= oz + r; z++)
                for (int x = ox - r; x <= ox + r; x++) {
                    if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) continue;
                    int dx = x-ox, dy = y-oy, dz = z-oz;
                    if (dx*dx + dy*dy + dz*dz > r*r) continue;
                    voxels[x + y*X + z*X*Y] = objectVoxel;
                }
    }

    int numClouds = 8;
    for (int c = 0; c < numClouds; c++) {
        int cx = rand() % X, cz = rand() % Z;
        int cy = waterLevel + Y/4 + rand() % (Y/4);
        int r = 8 + rand() % 12;
        for (int y = cy - r; y <= cy + r; y++)
            for (int z = cz - r; z <= cz + r; z++)
                for (int x = cx - r; x <= cx + r; x++) {
                    if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) continue;
                    int dx = x-cx, dy = y-cy, dz = z-cz;
                    if (dx*dx + dy*dy + dz*dz > r*r) continue;
                    int idx = x + y*X + z*X*Y;
                    if (voxels[idx].A == 0)
                        voxels[idx] = {230, 230, 235, (uint8_t)(1 + rand() % 75)};
                }
    }

    volume->load(X, Y, Z, std::move(voxels));
}

void VolumeRenderer::cleanup(){
    delete shader;
    shader = nullptr;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureID);
};

float VolumeRenderer::linearInterpolation(float a, float b, float t){
    return a + t * (b-a);
};

float VolumeRenderer::fade(float t){
    return t * t * (3.0f-2.0f*t);
};

float VolumeRenderer::generateNoise(float x, float y, int seed){
    int x0 = (int) x;
    int x1 = x0 + 1;
    int y0 = (int) y;
    int y1 = y0 + 1;

    float fractionalPointX = x - x0;
    float fractionalPointY = y - y0;

    float r00 = this->randomAt(x0, y0, seed), r10 = this->randomAt(x1, y0, seed);
    float r01 = this->randomAt(x0, y1, seed), r11 = this->randomAt(x1, y1, seed);
    
    float u = fade(fractionalPointX);
    float v = fade(fractionalPointY);

    float bottom = linearInterpolation(r00, r10, u);
    float top    = linearInterpolation(r01, r11, u);
    float result = linearInterpolation(bottom, top, v);

    return result;
}

float VolumeRenderer::randomAt(int i, int j, int seed) {
    int n = i * 374761393 + j * 668265263 + seed * 1013904223;
    n = (n ^ (n >> 13)) * 1274126177;
    return (n & 0x7fffffff) / (float)0x7fffffff;
}