#pragma once
#include <glm/glm.hpp>
class Volume;
class Shader;

class VolumeRenderer{
    private:
        unsigned int VAO, VBO;
        unsigned int textureID;
        Shader* shader;

        float linearInterpolation(float a, float b, float t);
        float fade(float t);
        float generateNoise(float x, float y, int seed);
        float randomAt(int i, int j, int seed);
    
    public:
        glm::vec3 volumeScale;
        float gasOpacityScale = 1.0f;
        float gasOpacityLowerLimit = 1.0f;
        float gasOpacityUpperLimit = 75.0f;
        float liquidOpacityScale = 1.0f;
        float liquidOpacityLowerLimit = 76.0f;
        float liquidOpacityUpperLimit = 150.0f;
        float objectsOpacityScale = 1.0f;
        float objectsOpacityLowerLimit = 151.0f;
        float objectsOpacityUpperLimit = 254.0f;
        float terrainOpacityScale = 1.0f;
        float voxelSize = 1.0f;
        float densityMin = 1.0f;
        float densityMax = 255.0f;

        VolumeRenderer();
        ~VolumeRenderer();
        void init();
        void uploadVolume(const Volume& volume);
        void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);
        void generateProceduralVolume(Volume* volume);
        void cleanup();
};