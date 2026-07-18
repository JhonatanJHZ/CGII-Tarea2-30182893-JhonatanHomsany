#include "../include/tools/FileManager.h"
#include "../include/tools/tinyfiledialogs.h"
#include "../../include/Volume.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

FileManager::FileManager(){}
FileManager::~FileManager(){}

string FileManager::getSavePath(){
    const char* filterPatterns[1] = { "*.raw" };
    const char* path = tinyfd_saveFileDialog(
        "Guardar volumen",   
        "volumen.raw",       
        1,                   
        filterPatterns,      
        "Volumen (*.raw)"    
    );
    if (path) return path;
    return "";
}

string FileManager::getFilePath(){
    const char* filterPatterns[2] = { "*.nrrd", "*.raw" };
    const char* path = tinyfd_openFileDialog("Importar volumen", "", 2, filterPatterns, "Volumenes (*.nrrd, *.raw)", 0);

    if(path) {
        return path;
    }
    return "";
}

Volume FileManager::readVolume(const string& filePath, int x, int y, int z){
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

    if(fileExtension == "raw"){
        voxels = readRawFile(filePath);

    }
    else if(fileExtension == "nrrd"){
        volume = readNrrdFile(filePath);
        return volume;
    }

    if(voxels.size() != (size_t)x * y * z){
        cout << "Error: El archivo no coincide con el tamaño de los voxeles" << endl;
        return volume;
    }

    volume.load(x, y, z, std::move(voxels));
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

Volume FileManager::readNrrdFile(const string& filePath){
    ifstream nrrdFile(filePath, ios::binary | ios::in);

    if(!nrrdFile.is_open()){
        cout << "Error cargando el archivo " << filePath << endl;
        return {};
    }

    string line;
    int X = 0, Y = 0, Z = 0;
    int components = 1;
    string encoding, type;

    while (getline(nrrdFile, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break;                                     
        if (line[0] == '#' || line.rfind("NRRD", 0) == 0) continue;  

        size_t colon = line.find(": ");
        if (colon == string::npos) continue;
        string key   = line.substr(0, colon);
        string value = line.substr(colon + 2);

        if (key == "type")          type = value;
        else if (key == "encoding") encoding = value;
        else if (key == "sizes") {
            stringstream ss(value);
            vector<int> dims; int n;
            while (ss >> n) dims.push_back(n);
            if (dims.size() == 4) { components = dims[0]; X = dims[1]; Y = dims[2]; Z = dims[3]; } 
            else if (dims.size() == 3) { X = dims[0]; Y = dims[1]; Z = dims[2]; }                  
        }
    }

    streamsize dataStart = nrrdFile.tellg();
    nrrdFile.seekg(0, ios::end);
    streamsize dataSize = nrrdFile.tellg() - dataStart;
    nrrdFile.seekg(dataStart, ios::beg);

    vector<unsigned char> bytes(dataSize);
    nrrdFile.read(reinterpret_cast<char*>(bytes.data()), dataSize);

    nrrdFile.close();
    vector<Voxel> fileVoxels;
    Voxel v;

    for(int i = 0; i + 3 < bytes.size(); i += 4){
        v.R = static_cast<int>(bytes[i]);
        v.G = static_cast<int>(bytes[i+1]);
        v.B = static_cast<int>(bytes[i+2]);
        v.A = static_cast<int>(bytes[i+3]);
        fileVoxels.push_back(v);
    }

    Volume volumeLoaded;
    volumeLoaded.load(X, Y, Z, std::move(fileVoxels)); 
    return volumeLoaded;
}

// NRRD0003
// # Complete NRRD file format specification at:
// # http://teem.sourceforge.net/nrrd/format.html
// type: unsigned char
// dimension: 4
// sizes: 4 512 128 512
// kinds: RGBA-color space space space
// encoding: raw

bool FileManager::saveRawFile(const string& filePath, Volume* volume){
    ofstream rawFile(filePath, ios::binary | ios::out);

    if(!rawFile.is_open()){
        cout << "Error guardando en el archivo " << filePath << endl;
        return false;
    }

    const vector<Voxel>& volumeVoxels = volume->getVoxels();
    rawFile.write(reinterpret_cast<const char*>(volumeVoxels.data()), volumeVoxels.size() * sizeof(Voxel));
    rawFile.close();
    cout << "Volumen guardado exitosamente!" << endl;
    return true;
}