#pragma once

#include <vector>
#include <cstddef>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace opengles_workspace
{
class Rectangle
{
    public:
        Rectangle(std::vector<GLfloat> vertices);
        ~Rectangle();

        void RenderRectangle();

    private:
        void CreateRectangle(std::vector<GLfloat> vertices);

        const std::size_t kNrCoordinatesPerRectangle = 12;
		const std::size_t kNrOfVerticesPerRectangle = 6;
        std::size_t vertices_size_;
        std::size_t total_nr_of_vertices = vertices_size_ / kNrCoordinatesPerRectangle * kNrOfVerticesPerRectangle;

        GLuint VAO_; 
		GLuint VBO_;
};
}