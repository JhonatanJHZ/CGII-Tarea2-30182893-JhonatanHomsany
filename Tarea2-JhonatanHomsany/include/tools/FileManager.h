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

class FileManager{
    public:

    FileManager();
    ~FileManager();
    
    string getFilePath();
    Volume readVolume(const string& filePath);

    private:

    vector<Voxel> readRawFile(const string& filePath);
    vector<Voxel> readNrrdFile(const string& filePath);
};