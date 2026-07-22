#include "../../include/tools/StoneburnerManager.h"
#include "../../include/Volume.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp> 
#include <algorithm>
#include <glm/glm.hpp>
using namespace std;

StoneburnerManager::StoneburnerManager()
    : actionRadius(0), alphaLowerLimit(0.0f), alphaUpperLimit(1.0f), active(false),
      selectedVoxelX(0), selectedVoxelY(0), selectedVoxelZ(0), clickRayOrigin({0,0,0}), clickRayDirection({0,0,0}), t_max(0) {}

StoneburnerManager::~StoneburnerManager() {
}

bool StoneburnerManager::insideRegion(int x, int y, int z) {
    int dx = x - selectedVoxelX;
    int dz = z - selectedVoxelZ;
    return dx*dx + dz*dz <= actionRadius*actionRadius;
}

void StoneburnerManager::destructVoxels(Volume* volume) {
    if (!volume) return;
    int X = volume->getX(), Y = volume->getY(), Z = volume->getZ();
    vector<Voxel>& voxels = volume->getVoxels();
    int destroyed = 0;
    int r = actionRadius;

    for (int y = 0; y < Y; y++) {                                   
        for (int z = selectedVoxelZ - r; z <= selectedVoxelZ + r; z++) {  
            for (int x = selectedVoxelX - r; x <= selectedVoxelX + r; x++) {
                if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) continue;
                if (!insideRegion(x, y, z)) continue;
                Voxel& v = voxels[x + y*X + z*X*Y];
                if (v.A >= alphaLowerLimit * 255 && v.A <= alphaUpperLimit * 255) {
                    v.A = 0;
                    destroyed++;                                 
                }
            }
        }
    }
    cout << "Voxeles destruidos: " << destroyed << endl;
}


glm::vec3 getMouseWorldPos(double mouseX, double mouseY, int width, int height, const glm::mat4& view, const glm::mat4& proj, float depthZ) {
    float x = (float)mouseX;
    float y = (float)height - (float)mouseY;
    glm::vec3 screenPos = glm::vec3(x, y, depthZ);
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)width, (float)height);
    return glm::unProject(screenPos, view, proj, viewport);
}

bool StoneburnerManager::pick(double mouseX, double mouseY, int width, int height, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& volumeScale, const Volume& volume)
{
    clickRayOrigin = getMouseWorldPos(mouseX, mouseY, width, height, view, proj, 0.0f);
    glm::vec3 farPoint = getMouseWorldPos(mouseX, mouseY, width, height, view, proj, 1.0f);
    clickRayDirection = glm::normalize(farPoint - clickRayOrigin);

    glm::vec3 pos    = clickRayOrigin / volumeScale;
    glm::vec3 dirTex = glm::normalize(clickRayDirection / volumeScale);

    int X = volume.getX(), Y = volume.getY(), Z = volume.getZ();
    const vector<Voxel>& voxels = volume.getVoxels();

    float stepSize = 1.0f / 512.0f;
    for (int i = 0; i < 4096; i++) {
        if (pos.x >= 0.0f && pos.x <= 1.0f &&
            pos.y >= 0.0f && pos.y <= 1.0f &&
            pos.z >= 0.0f && pos.z <= 1.0f)
        {
            int vx = std::min((int)(pos.x * X), X - 1);
            int vy = std::min((int)(pos.y * Y), Y - 1);
            int vz = std::min((int)(pos.z * Z), Z - 1);
            const Voxel& v = voxels[vx + vy*X + vz*X*Y];
            if (v.A >= pickThreshold * 255.0f) {
                selectedVoxelX = vx;
                selectedVoxelY = vy;
                selectedVoxelZ = vz;
                return true;
            }
        }
        pos += dirTex * stepSize;
    }
    return false;
}
