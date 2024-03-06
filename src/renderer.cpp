#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>
#include <iostream>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace opengles_workspace
{
	GLFWRenderer::GLFWRenderer(std::shared_ptr<Context> context)
		: mContext(std::move(context))
		, vertexShader(glCreateShader(GL_VERTEX_SHADER))
		, fragmentShader(glCreateShader(GL_FRAGMENT_SHADER))
		, shaderProgram (glCreateProgram())
	{
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		// Compile the Vertex Shader into machine code
		glCompileShader(vertexShader);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		// Compile the Vertex Shader into machine code
		glCompileShader(fragmentShader);

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		// Wrap-up/Link all the shaders together into the Shader Program
		glLinkProgram(shaderProgram);

		// Delete the now useless Vertex and Fragment Shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Generate the VAO and VBO with only 1 object each
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
	}

	GLFWRenderer::~GLFWRenderer()
	{
		// Delete all the objects we've created
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);
	}

	const char* GLFWRenderer::vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* GLFWRenderer::fragmentShaderSource = "#version 330 core\n"
		"uniform vec3 figColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(figColor, 1.0f);\n"
		"}\n\0";

	std::vector<GLfloat> GLFWRenderer::PopulateVertices(bool is_same_index)
	{
		GLfloat x = -0.5f;
		GLfloat y =  0.5f;
		GLfloat side_length = 0.125f;
		std::vector<GLfloat> vertices;

		for(size_t i = 1; i <= kNrRows; ++i)
		{
			for(size_t j = 1; j <= kNrColumns; ++j)
			{
				//upper triangle
				GLfloat upper_top_left_x = x;
				GLfloat upper_top_left_y = y;
				GLfloat upper_top_right_x = x + side_length;
				GLfloat upper_top_right_y = y;
				GLfloat upper_bottom_left_x = x;
				GLfloat upper_bottom_left_y = y - side_length;
				//lower triangle
				GLfloat lower_top_right_x = x + side_length;
				GLfloat lower_top_right_y = y;
				GLfloat lower_bottom_left_x = x;
				GLfloat lower_bottom_left_y = y - side_length;
				GLfloat lower_bottom_right_x = x + side_length;
				GLfloat lower_bottom_right_y = y - side_length;

				//pushing coordinates of squares needing to be colored
				//squares with first color: when i and j are both even OR when i and j are both odd
				//squares with second color: when i is even and j is odd OR viceversa
				//differentiate between squares sets with is_same_index  
				if((((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0)) && is_same_index) ||
				  (((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)) && !is_same_index))
				{
					vertices.push_back(upper_top_left_x);
					vertices.push_back(upper_top_left_y);
					vertices.push_back(upper_top_right_x);
					vertices.push_back(upper_top_right_y);
					vertices.push_back(upper_bottom_left_x);
					vertices.push_back(upper_bottom_left_y);
					vertices.push_back(lower_top_right_x);
					vertices.push_back(lower_top_right_y);
					vertices.push_back(lower_bottom_left_x);
					vertices.push_back(lower_bottom_left_y);
					vertices.push_back(lower_bottom_right_x);
					vertices.push_back(lower_bottom_right_y);
				}

				x += side_length;
			}
			x  =  -0.5f;
			y -= side_length;
		}

		return vertices;
	}

	void GLFWRenderer::DrawShapes(std::vector<GLfloat> vertices, size_t totalVerticesPerShape, std::vector<GLfloat> color)
	{
		// Make the VAO the current Vertex Array Object by binding it
		glBindVertexArray(VAO);
		// Bind the VBO specifying it's a GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Introduce the vertices into the VBO
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		// Configure the Vertex Attribute so that OpenGL knows how to read the VAO
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		// Enable the Vertex Attribute so that OpenGL knows to use it
		glEnableVertexAttribArray(0);

		// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		if (!is_background_colored)
		{
			//cbeige
			glClearColor(1.0f, 0.8f, 0.4f, 1.0f);
			//gray
			//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT);
			is_background_colored = true;
		}
		
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		//uniform for rendering different colors
		GLint uniform = glGetUniformLocation(shaderProgram, "figColor");
		glUniform3f(uniform, color[0], color[1], color[2]);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, totalVerticesPerShape);

		// GL code end
		glfwSwapBuffers(window());
	}

	void GLFWRenderer::render() {
		if(kNrTotalSquares == 0)
			return;
		
		//get first set of squares coordinates
		std::vector<GLfloat> vertices {PopulateVertices(true)};
		std::vector<GLfloat> margin_coordinates 
		{
			-0.505,  0.505,
			 0.505,  0.505,
			-0.505, -0.505,

			 0.505,  0.505,
			-0.505, -0.505,
			 0.505, -0.505
		};
		vertices.insert(vertices.end(), margin_coordinates.begin(), margin_coordinates.end());

		std::vector<GLfloat> white_color {1.0f, 1.0f, 1.0f};
		DrawShapes(vertices, kNrOfVerticesPerSquare * kNrSquaresToBeColored + kNrOfVerticesPerSquare, white_color);

		//get second set of squares coordinates
		vertices = PopulateVertices(false);
		std::vector<GLfloat> brown_color {0.7f, 0.35f, 0.0f};
		DrawShapes(vertices, kNrOfVerticesPerSquare * kNrSquaresToBeColored, brown_color);
	}

	bool GLFWRenderer::poll() {
		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}
}