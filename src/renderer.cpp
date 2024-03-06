#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>
#include <cmath>
#include <vector>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace opengles_workspace
{
	GLFWRenderer::GLFWRenderer(std::shared_ptr<Context> context)
		: mContext(std::move(context))
	{
	}

	void GLFWRenderer::render() {
		//kNrTotalSquares needs to be perfect square
		const size_t kNrTotalSquares = 64;
		const size_t kHalfNrSquares = kNrTotalSquares / 2;
		//calculation needed to solve the problem of even/odd numbers
		const size_t kNrSquaresToBeColored = (kNrTotalSquares % 2 == 0) ? kHalfNrSquares : kHalfNrSquares + 1;
		const size_t kNrOfVerticesPerSquare = 6;

		if(kNrTotalSquares == 0)
			return;

		// Vertex Shader source code
		const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
		//Fragment Shader source code
		const char* fragmentShaderSource = "#version 330 core\n"
		"uniform vec3 figColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(figColor, 1.0f);\n"
		"}\n\0";

		// Create Vertex Shader Object and get its reference
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Attach Vertex Shader source to the Vertex Shader Object
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(vertexShader);

		// Create Fragment Shader Object and get its reference
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Attach Fragment Shader source to the Fragment Shader Object
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(fragmentShader);

		// Create Shader Program Object and get its reference
		GLuint shaderProgram = glCreateProgram();
		// Attach the Vertex and Fragment Shaders to the Shader Program
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		// Wrap-up/Link all the shaders together into the Shader Program
		glLinkProgram(shaderProgram);

		// Delete the now useless Vertex and Fragment Shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		std::vector<GLfloat> vertices {};
		//vertices.reserve(kTotalValues);

		//top left corner of englobing square
		GLfloat x = -0.5f;
		GLfloat y =  0.5f;
		GLfloat side_length = 0.125f;
		const int kNrRows = sqrt(kNrTotalSquares);
		const int kNrColumns = sqrt(kNrTotalSquares);

		for(int i = 1; i <= kNrRows; ++i)
		{
			for(int j = 1; j <= kNrColumns; ++j)
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
				if((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0))
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

		// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
		GLuint VAO, VBO;

		// Generate the VAO and VBO with only 1 object each
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		//uniform for rendering different colors
		GLint uniform = glGetUniformLocation(shaderProgram, "figColor");
		glUniform3f(uniform, 1.0f, 0.2f, 0.0f);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, kNrOfVerticesPerSquare * kNrSquaresToBeColored);

		// Delete all the objects we've created
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);



		// GL code end
		glfwSwapBuffers(window());
	}

	bool GLFWRenderer::poll() {
		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}
}