#pragma once
#include <glm/glm.hpp>
class Volume;
class Shader;

class VolumeRenderer{
    private:
    unsigned int VAO, VBO;
    unsigned int textureID;
    Shader* shader;
    bool displayGas = true;
    bool displayLiquid = true;
    bool displayObjects = true;
    bool displayTerrain = true;

    public:
        VolumeRenderer();
        ~VolumeRenderer();
        void init();
        void uploadVolume(const Volume& volume);
        void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);
        void cleanup();
};