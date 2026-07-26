#include "../include/Volume.h"
#include <vector>
using namespace std;

Volume::Volume(){
    X = 512;
    Y = 128;
    Z = 512;
}

Volume::~Volume(){}

void Volume::load(int X, int Y, int Z, vector<Voxel> voxels) {
    this->X = X; 
    this->Y = Y; 
    this->Z = Z;
    this->voxels = move(voxels);
    this->originalWorld = this->voxels;
    this->objects.clear();
}

void Volume::insertObject(const Volume& object, int x, int y, int z, float scaleX, float scaleY, float scaleZ, glm::vec3 color) {
    ObjectInstance newObject;
    newObject.sourceVoxels = object.getVoxels();
    newObject.dimensionX = object.getX();
    newObject.dimensionY = object.getY();
    newObject.dimensionZ = object.getZ();
    newObject.position = glm::vec3(x, y, z);
    newObject.scale    = glm::vec3(scaleX, scaleY, scaleZ);
    newObject.color    = color;
    objects.push_back(newObject);
    rebuildWorld();
}

void Volume::stampObject(const ObjectInstance& newObject) {
    const vector<Voxel>& objVoxels = newObject.sourceVoxels;
    int objectX = newObject.dimensionX; 
    int objectY = newObject.dimensionY;
    int objectZ = newObject.dimensionZ;

    int px = (int)newObject.position.x, py = (int)newObject.position.y, pz = (int)newObject.position.z;
    float scaleX = newObject.scale.x, scaleY = newObject.scale.y, scaleZ = newObject.scale.z;

    int destX = (int)(objectX * scaleX);
    int destY = (int)(objectY * scaleY);
    int destZ = (int)(objectZ * scaleZ);

    for (int oz = 0; oz < destZ; oz++) {
      for (int oy = 0; oy < destY; oy++) {
        for (int ox = 0; ox < destX; ox++) {

            int worldX = px + ox;
            int worldY = py + oy;
            int worldZ = pz + oz;
            if (worldX < 0 || worldX >= X || worldY < 0 || worldY >= Y || worldZ < 0 || worldZ >= Z)
                continue;

            int dx = (int)(ox / scaleX);
            int dy = (int)(oy / scaleY);
            int dz = (int)(oz / scaleZ);
            if (dx >= objectX || dy >= objectY || dz >= objectZ) continue;

            Voxel v = objVoxels[dx + dy*objectX + dz*objectX*objectY];
            if (v.A == 0) continue;            

            v.R = (uint8_t)(newObject.color.r * 255.0f);
            v.G = (uint8_t)(newObject.color.g * 255.0f);
            v.B = (uint8_t)(newObject.color.b * 255.0f);

            voxels[worldX + worldY*X + worldZ*X*Y] = v;
        }
      }
    }
}

void Volume::rebuildWorld() {
    voxels = originalWorld;                 
    for (const auto& o : objects)
        stampObject(o);                     
}

void Volume::modifyObject(ObjectInstance& object, glm::vec3 newPosition, glm::vec3 newScale, glm::vec3 newColor){
    object.position = newPosition;
    object.scale = newScale;
    object.color = newColor;
}