#pragma once
#include<vector>
#include<string>
#include <glm/glm.hpp>

using namespace std;

struct Voxel{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};

struct ObjectInstance {
    vector<Voxel> sourceVoxels;   
    int dimensionX, dimensionY, dimensionZ;              
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 color;
};

class Volume{
    private:

    int X, Y, Z;
    vector<Voxel> originalWorld;
    vector<Voxel> voxels;
    vector<ObjectInstance> objects;

    public:
        Volume();
        ~Volume();

        int getX() const {return X;}
        int getY() const {return Y;} 
        int getZ() const {return Z;}
        
        void setX(int x){X = x;}
        void setY(int y){Y = y;}
        void setZ(int z){Z = z;}

        vector<Voxel>& getVoxels() { return voxels; }
        const vector<Voxel>& getVoxels() const { return voxels; }
        void setVoxels(vector<Voxel> voxels){this->voxels = voxels;}

        vector<ObjectInstance>& getObjects() { return objects; }

        void load(int X, int Y, int Z, vector<Voxel> voxels);

        void insertObject(const Volume& object, int x, int y, int z, float scaleX, float scaleY, float scaleZ, glm::vec3 color);

        void stampObject(const ObjectInstance& newObject);

        void rebuildWorld();

        void modifyObject(ObjectInstance& object, glm::vec3 newPosition, glm::vec3 newScale, glm::vec3 newColor);
};