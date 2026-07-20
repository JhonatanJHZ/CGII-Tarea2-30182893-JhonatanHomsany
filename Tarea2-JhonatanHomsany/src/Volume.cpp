#include "../include/Volume.h"
#include <vector>
using namespace std;

Volume::Volume(){
    X = 512;
    Y = 128;
    Z = 512;
}

Volume::~Volume(){}

void Volume::load(int X, int Y, int Z, vector<Voxel> voxels){
    this->X = X;
    this->Y = Y;
    this->Z = Z;
    this->voxels = move(voxels);
}

void Volume::insertObject(const Volume& object, int x, int y, int z, glm::vec3 color){
    const vector<Voxel>& objVoxels = object.getVoxels();
    int objectX = object.getX(); 
    int objectY = object.getY(); 
    int objectZ = object.getZ();

    for (int oz = 0; oz < objectZ; oz++) {
      for (int oy = 0; oy < objectY; oy++) {
        for (int ox = 0; ox < objectX; ox++) {

            int worldX = ox + x;
            int worldY = oy + y;
            int worldZ = oz + z;

            if (worldX < 0 || worldX >= X || worldY < 0 ||  worldY >= Y || worldZ < 0 || worldZ >= Z){
                continue;
            }

            int objIndex = ox + oy*objectX + oz*objectX*objectY;
            Voxel v = objVoxels[objIndex];

            if (v.A == 0){ 
                continue;
            }

            v.R = (uint8_t)(color.r * 255.0f);
            v.G = (uint8_t)(color.g * 255.0f);
            v.B = (uint8_t)(color.b * 255.0f);

            int worldIndex = worldX + worldY*X + worldZ*X*Y;   
            voxels[worldIndex] = v;               
        }
      }
    }
}