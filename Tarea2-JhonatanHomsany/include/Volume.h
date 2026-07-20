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

class Volume{
    private:

    int X, Y, Z;
    vector<Voxel> voxels;

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

        void load(int X, int Y, int Z, vector<Voxel> voxels);

        void insertObject(const Volume& object, int x, int y, int z, glm::vec3 color);
};