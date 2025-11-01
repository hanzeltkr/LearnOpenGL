#ifndef SHADER_H  
#define SHADER_H  

#include <string>  
#include <fstream>  
#include <sstream>  
#include <iostream>  
using namespace std;  

#include <glad/glad.h>  
#include <GLFW/glfw3.h>  

class Shader {  
public:  
    // Program ID
    GLuint Program;  
    // Reads and builds shader
    Shader(const char* vertexSourcePath, const char* fragmentSourcePath);  
    // Use program
    void Use();  
};  

#endif