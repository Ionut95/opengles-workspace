#include "program.hpp"

#include <iostream>
#include <fstream>

namespace opengles_workspace
{
Program::Program()
    : vertexShader(glCreateShader(GL_VERTEX_SHADER))
	, fragmentShader(glCreateShader(GL_FRAGMENT_SHADER))
	, shaderProgram(glCreateProgram())
{
    std::string vertexShaderSource = ReadFile("../data_files/vertex_shader.txt");
	std::string fragmentShaderSource = ReadFile("../data_files/fragment_shader.txt");

    PrepareVertexShader(vertexShaderSource.c_str());
    PrepareFragmentShader(fragmentShaderSource.c_str());
    LinkProgram();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Program::~Program()
{
    glDeleteProgram(shaderProgram);
}

void Program::useProgram()
{
    glUseProgram(shaderProgram);
}

std::string Program::ReadFile(std::string file_location)
{
    std::string content = "";
    std::ifstream file(file_location);

    if(!file.is_open())
    {
        std::cout << "Error when reading file " << file_location << "\n";
        return content; 
    }

    std::string line = "";

    while(std::getline(file, line))
    {
        content.append(line + "\n");
    }

    return content;
}

void Program::PrepareVertexShader(const char* vertexShaderSource)
{
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success_vertex_compilation;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_vertex_compilation);
    if (!success_vertex_compilation) {
        char errorLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
    }
}

void Program::PrepareFragmentShader(const char* fragmentShaderSource)
{
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    int success_fragment_compilation;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_fragment_compilation);
    if (!success_fragment_compilation) {
        char errorLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
    }
}

void Program::LinkProgram()
{
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success_link;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_link);
    if (!success_link) {
        char errorLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
        std::cout << "\nShader linking error:\n" << errorLog << '\n';
    }
}
}