#pragma once
#include <vector>
#include <glm/glm.hpp>
class Volume;

class StoneburnerManager {
public:
    float pickThreshold = 0.5f;
    int actionRadius;
    float alphaLowerLimit, alphaUpperLimit;
    bool active;
    int selectedVoxelX, selectedVoxelY, selectedVoxelZ;
    glm::vec3 clickRayOrigin;
    glm::vec3 clickRayDirection;
    float t_max;

    StoneburnerManager();
    ~StoneburnerManager();

    bool pick(double mouseX, double mouseY, int width, int height, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& volumeScale, const Volume& volume);
    void destructVoxels(Volume* volume);         

private:
    bool insideRegion(int x, int y, int z);
};