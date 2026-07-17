#include "../include/Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

Shader::Shader(const string& vertexPath, const string& fragmentPath){
    string vertexProgram = readFile(vertexPath);
    string fragmentProgram = readFile(fragmentPath);

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexProgram);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentProgram);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
Shader::~Shader(){
    glDeleteProgram(ID);
}

void Shader::use() const{
    glUseProgram(ID);
};
void Shader::setInt(const string& name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};
void Shader::setFloat(const string& name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
};
void Shader::setVec3(const string& name, const glm::vec3& value) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
};
void Shader::setMat4(const string& name, const glm::mat4& value) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
};

string Shader::readFile(const string& path){
    ifstream shaderFile(path);

    if(!shaderFile.is_open()){
        cout << "Error abriendo el shader: " << path << endl;
        throw runtime_error("No se pudo abrir el shader: " + path);
    }

    stringstream ss;
    ss << shaderFile.rdbuf();
    shaderFile.close();
    return ss.str();
};
unsigned int Shader::compileShader(unsigned int type, const string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    checkErrors(id, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return id;
};

void Shader::checkErrors(unsigned int object, const string& type){
    int success;
    char infoLog[1024];

    if (type == "PROGRAM") {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        }
    } else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << endl;
        }
    }
}