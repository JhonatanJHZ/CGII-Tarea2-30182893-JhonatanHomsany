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