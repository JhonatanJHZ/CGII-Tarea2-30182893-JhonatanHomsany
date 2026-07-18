#pragma once
#include <glm/glm.hpp>
class Volume;
class Shader;

class VolumeRenderer{
    private:
    unsigned int VAO, VBO;
    unsigned int textureID;
    Shader* shader;
    
    public:
        glm::vec3 volumeScale;
        float gasOpacityScale = 1.0f;
        float liquidOpacityScale = 1.0f;
        float objectsOpacityScale = 1.0f;
        float terrainOpacityScale = 1.0f;
        float voxelSize = 1.0f;
        float densityMin = 0.0f;
        float densityMax = 1.0f;

        VolumeRenderer();
        ~VolumeRenderer();
        void init();
        void uploadVolume(const Volume& volume);
        void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);
        void cleanup();
};