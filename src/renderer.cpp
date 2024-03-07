#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace opengles_workspace
{
	GLFWRenderer::GLFWRenderer(std::shared_ptr<Context> context)
		: mContext(std::move(context))
		, shaderProgram (glCreateProgram())
	{
		std::tuple<size_t,size_t> rows_columns(ReadData());
		nr_rows = std::get<0>(rows_columns);
		nr_columns = std::get<1>(rows_columns);
		nr_total_squares = nr_rows * nr_columns;

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
	}

	GLFWRenderer::~GLFWRenderer()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);
	}

	const char* GLFWRenderer::vertexShaderSource = "#version 300 es\n"
		"precision lowp float;\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* GLFWRenderer::fragmentShaderSource = "#version 300 es\n"
		"precision lowp float;\n"
		"uniform vec3 figColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(figColor, 1.0f);\n"
		"}\n\0";

	std::vector<GLfloat> GLFWRenderer::PopulateVertices(bool is_same_index)
	{
		GLfloat x = -1.0f;
		GLfloat y =  1.0f;
		GLfloat side_length = 2.0 / static_cast<GLfloat>(nr_columns);
		GLfloat side_width = 2.0 / static_cast<GLfloat>(nr_rows);
		std::vector<GLfloat> vertices;

		for(size_t i = 1; i <= nr_rows; ++i)
		{
			for(size_t j = 1; j <= nr_columns; ++j)
			{
				//upper triangle
				GLfloat upper_top_left_x = x;
				GLfloat upper_top_left_y = y;
				GLfloat upper_top_right_x = x + side_length;
				GLfloat upper_top_right_y = y;
				GLfloat upper_bottom_left_x = x;
				GLfloat upper_bottom_left_y = y - side_width;
				//lower triangle
				GLfloat lower_top_right_x = x + side_length;
				GLfloat lower_top_right_y = y;
				GLfloat lower_bottom_left_x = x;
				GLfloat lower_bottom_left_y = y - side_width;
				GLfloat lower_bottom_right_x = x + side_length;
				GLfloat lower_bottom_right_y = y - side_width;

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
			x  =  -1.0f;
			y -= side_width;
		}

		return vertices;
	}

	void GLFWRenderer::DrawShapes(std::vector<GLfloat> vertices, std::vector<GLfloat> color)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		if (!is_background_colored)
		{
			//beige
			glClearColor(1.0f, 0.8f, 0.4f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			is_background_colored = true;
		}
		
		glUseProgram(shaderProgram);
		GLint uniform = glGetUniformLocation(shaderProgram, "figColor");
		glUniform3f(uniform, color[0], color[1], color[2]);
		glBindVertexArray(VAO);
		size_t nr_of_squares = vertices.size() / kNrCoordinatesPerSquare;
		glDrawArrays(GL_TRIANGLES, 0, kNrOfVerticesPerSquare * nr_of_squares);

		glfwSwapBuffers(window());
	}

	void GLFWRenderer::render() {
		if(nr_total_squares == 0)
			return;
		
		//get first set of squares coordinates
		std::vector<GLfloat> vertices {PopulateVertices(true)};
		std::vector<GLfloat> white_color {0.0f, 0.0f, 0.0f};
		DrawShapes(vertices, white_color);

		//get second set of squares coordinates
		vertices = PopulateVertices(false);
		std::vector<GLfloat> brown_color {0.7f, 0.35f, 0.0f};
		DrawShapes(vertices, brown_color);
	}

	bool GLFWRenderer::poll() {
		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}

	std::tuple<size_t, size_t> GLFWRenderer::ReadData() const
	{
		std::ifstream file("../data_files/data.txt");

		if(!file.is_open())
		{
			std::cout << "Error: file not open" << std::endl;
			return {0, 0};
		}

		std::string line;

		getline(file, line);
		size_t nr_rows = GetValueFromLine(line);

		getline(file, line);
		size_t nr_columns = GetValueFromLine(line);
		return {nr_rows, nr_columns};
	}

	size_t GLFWRenderer::GetValueFromLine(std::string line) const
	{
		std::stringstream stream(line);
		std::string str;

		getline(stream, str, ':');
		getline(stream, str, ':');

		std::stringstream to_size_t(str);
		size_t value;
		to_size_t >> value;
		return value;
	}
}