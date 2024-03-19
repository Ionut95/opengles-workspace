#pragma once

#include <string>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace opengles_workspace
{
class Program
{
    public:
        Program();
        ~Program();

        void useProgram();

    private:
        void PrepareVertexShader(const char* vertexShaderSource);
		void PrepareFragmentShader(const char* fragmentShaderSource);
		void LinkProgram();

        std::string ReadFile(std::string file_location);

        GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;
};
} // opengles_workspace
