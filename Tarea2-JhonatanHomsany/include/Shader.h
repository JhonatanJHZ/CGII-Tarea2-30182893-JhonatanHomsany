#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace std;

class Shader{
    
    private:

        string readFile(const string& path);
        unsigned int compileShader(unsigned int type, const string& source);
        void checkErrors(unsigned int object, const string& type);
        
    public:
        unsigned int ID;
        Shader(const string& vertexPath, const string& fragmentPath);
        ~Shader();
        void use() const;
        void setInt(const string& name, int value) const;
        void setFloat(const string& name, float value) const;
        void setVec3(const string& name, const glm::vec3& value) const;
        void setMat4(const string& name, const glm::mat4& value) const;

};