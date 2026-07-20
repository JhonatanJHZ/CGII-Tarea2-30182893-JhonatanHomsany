#pragma once
#include <vector>
class Volume;

class StoneburnerManager {
public:
    int actionRadius;
    float alphaLowerLimit, alphaUpperLimit;
    bool active;
    int selectedVoxelX, selectedVoxelY, selectedVoxelZ;

    StoneburnerManager();
    ~StoneburnerManager();

    void destructVoxels(Volume* volume);         

private:
    bool insideRegion(int x, int y, int z);
};