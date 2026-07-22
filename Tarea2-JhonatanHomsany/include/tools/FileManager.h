#pragma once
#include "../Volume.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

class Volume;

class FileManager{
    public:

    FileManager();
    ~FileManager();
    
    string getFilePath();
    string getSavePath();
    Volume readVolume(const string& filePath, int x, int y, int z, int bitDepth = 32);

    bool saveRawFile(const string&filePath,  Volume* volume);

    private:

    vector<Voxel> readRawFile(const string& filePath, int bitDepth = 32);
    Volume readNrrdFile(const string& filePath);
};