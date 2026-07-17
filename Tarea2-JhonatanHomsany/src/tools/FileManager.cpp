#include "../include/tools/FileManager.h"
#include "../include/tools/tinyfiledialogs.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

FileManager::FileManager(){}
FileManager::~FileManager(){}

string FileManager::getFilePath(){
    const char* filterPatterns[2] = { "*.nrrd", "*.raw" };
    const char* path = tinyfd_openFileDialog("Importar volumen", "", 2, filterPatterns, "Volumenes (*.nrrd, *.raw)", 0);

    if(path) {
        return path;
    }
    return "";
}

Volume FileManager::readVolume(const string& filePath){
    size_t dotPosition = filePath.rfind('.');
    string fileName, fileExtension;
    Volume volume;
    vector<Voxel> voxels;

    if(dotPosition != string::npos){
        fileName = filePath.substr(0, dotPosition);
        fileExtension = filePath.substr(dotPosition + 1);
    }
    else{
        fileName = filePath;
    }

    string name = filePath.substr(filePath.find_last_of("/\\") + 1);

    stringstream ss(name);
    string token;
    int X = 512, Y = 128, Z = 512;
    while (getline(ss, token, '_')) {
        if (count(token.begin(), token.end(), 'x') == 2 && isdigit((unsigned char)token[0])) {
            char sep;                       
            stringstream dims(token);
            dims >> X >> sep >> Y >> sep >> Z;
            break;
        }
    }

    if(fileExtension == "raw"){
        voxels = readRawFile(filePath);

    }
    else if(fileExtension == "nrrd"){
        voxels = readNrrdFile(filePath);
    }

    if(voxels.size() != (size_t)X * Y * Z){
        cout << "Error: El archivo no coincide con el tamaño de los voxeles" << endl;
        return volume;
    }

    volume.load(X, Y, Z, std::move(voxels));
    cout << "Volumen cargado exitosamente!" << endl;
    return volume;
}

vector<Voxel> FileManager::readRawFile(const string& filePath){
    ifstream rawFile(filePath, ios::binary | ios::in);

    if(!rawFile.is_open()){
        cout << "Error cargando el archivo " << filePath << endl;
        return {};
    }

    rawFile.seekg(0, ios::end);
    streamsize size = rawFile.tellg();
    rawFile.seekg(0, ios::beg);

    vector<unsigned char> bytes(size);
    rawFile.read(reinterpret_cast<char*>(bytes.data()), size);

    rawFile.close();
    vector<Voxel> fileVoxels;
    Voxel v;

    for(int i = 0; i + 3 < bytes.size(); i += 4){
        v.R = static_cast<int>(bytes[i]);
        v.G = static_cast<int>(bytes[i+1]);
        v.B = static_cast<int>(bytes[i+2]);
        v.A = static_cast<int>(bytes[i+3]);
        fileVoxels.push_back(v);
    }

    return fileVoxels;
}

vector<Voxel> FileManager::readNrrdFile(const string& filePath){
    return {};
}
