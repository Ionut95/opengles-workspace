#include "rectangle.hpp"

namespace opengles_workspace
{
Rectangle::Rectangle(std::vector<GLfloat> vertices)
    : vertices_size_(vertices.size())
{
    glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);

    CreateRectangle(vertices);
}

Rectangle::~Rectangle()
{
    glDeleteVertexArrays(1, &VAO_);
	glDeleteBuffers(1, &VBO_);
}

void Rectangle::RenderRectangle()
{
    glBindVertexArray(VAO_);
	glDrawArrays(GL_TRIANGLES, 0, total_nr_of_vertices);
}

void Rectangle::CreateRectangle(std::vector<GLfloat> vertices)
{
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
}