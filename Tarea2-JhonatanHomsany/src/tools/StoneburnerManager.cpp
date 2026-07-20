#include "../../include/tools/StoneburnerManager.h"
#include "../../include/Volume.h"
#include <iostream>
using namespace std;

StoneburnerManager::StoneburnerManager()
    : actionRadius(0), alphaLowerLimit(0.0f), alphaUpperLimit(1.0f), active(false),
      selectedVoxelX(0), selectedVoxelY(0), selectedVoxelZ(0) {}

StoneburnerManager::~StoneburnerManager() {}

// bool StoneburnerManager::insideRegion(int x, int y, int z) {
//     if (z != selectedVoxelZ) return false;
//     int dx = x - selectedVoxelX;
//     int dy = y - selectedVoxelY;
//     return dx*dx + dy*dy <= actionRadius*actionRadius;
// }

bool StoneburnerManager::insideRegion(int x, int y, int z) {
    int dx = x - selectedVoxelX;
    int dz = z - selectedVoxelZ;
    return dx*dx + dz*dz <= actionRadius*actionRadius;   // any y — a vertical column
}

// void StoneburnerManager::destructVoxels(Volume* volume) {
//     if (!volume) return;
//     int X = volume->getX(), Y = volume->getY(), Z = volume->getZ();
//     vector<Voxel>& voxels = volume->getVoxels();
//     int destroyed = 0;

//     int r = actionRadius;
//     for (int y = selectedVoxelY - r; y <= selectedVoxelY + r; y++) {
//         for (int x = selectedVoxelX - r; x <= selectedVoxelX + r; x++) {
//             int z = selectedVoxelZ;
//             if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) continue;
//             if (!insideRegion(x, y, z)) continue;
//             Voxel& v = voxels[x + y*X + z*X*Y];
//             if (v.A >= alphaLowerLimit*255 && v.A <= alphaUpperLimit*255)
//                 v.A = 0;
//             destroyed++;
//         }
//     }

//     cout << "Voxeles destruidos: " << destroyed << std::endl;
// }

void StoneburnerManager::destructVoxels(Volume* volume) {
    if (!volume) return;
    int X = volume->getX(), Y = volume->getY(), Z = volume->getZ();
    vector<Voxel>& voxels = volume->getVoxels();
    int destroyed = 0;
    int r = actionRadius;

    for (int y = 0; y < Y; y++) {                                   // ALL heights (bore down)
        for (int z = selectedVoxelZ - r; z <= selectedVoxelZ + r; z++) {   // Z varies
            for (int x = selectedVoxelX - r; x <= selectedVoxelX + r; x++) {
                if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) continue;
                if (!insideRegion(x, y, z)) continue;
                Voxel& v = voxels[x + y*X + z*X*Y];
                if (v.A >= alphaLowerLimit * 255 && v.A <= alphaUpperLimit * 255) {
                    v.A = 0;
                    destroyed++;                                    // count only real destructions
                }
            }
        }
    }
    cout << "Voxeles destruidos: " << destroyed << endl;
}