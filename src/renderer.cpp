#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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
		, texture(Texture((char*)("../Textures/chess_img.jpg")))
	{
		texture.LoadTexture();

		std::tuple<size_t,size_t> rows_columns(ReadData());
		nr_rows = std::get<0>(rows_columns);
		nr_columns = std::get<1>(rows_columns);
		nr_total_squares = nr_rows * nr_columns;

		PrepareVertexShader();
		PrepareFragmentShader();
		LinkProgram();

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

	void GLFWRenderer::PrepareVertexShader()
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

	void GLFWRenderer::PrepareFragmentShader()
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

	void GLFWRenderer::LinkProgram()
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

	const char* GLFWRenderer::vertexShaderSource = "#version 300 es\n"
		"precision lowp float;\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 tex;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   TexCoord  = tex;\n"
		"}\0";
	const char* GLFWRenderer::fragmentShaderSource = "#version 300 es\n"
		"precision lowp float;\n"
		"uniform sampler2D theTexture;\n"
		"in vec2 TexCoord;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = texture(theTexture, TexCoord);\n"
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

				//squares with 
				//		- first texture: when i and j are same parity
				//		- second texture: when i and j are different parity
				//differentiate between squares sets with is_same_index
				bool is_same_parity = ((i & 1) == (j & 1));
				if(is_same_parity == is_same_index)
				{
					float offset = is_same_index ? 0.0f : 0.5f;
					vertices.insert
					(
						//				upper triangle
						vertices.end(), {
										upper_top_left_x, upper_top_left_y, 0.0f + offset, 1.0f, 
										upper_top_right_x, upper_top_right_y, 0.5f + offset, 1.0f,
										upper_bottom_left_x, upper_bottom_left_y, 0.0f + offset, 0.0f,
						//				lower triangle
										lower_top_right_x, lower_top_right_y, 0.5f + offset, 1.0f,
										lower_bottom_left_x, lower_bottom_left_y, 0.0f + offset, 0.0f,
										lower_bottom_right_x, lower_bottom_right_y, 0.5f + offset, 0.0f
										}
					);
				}

				x += side_length;
			}
			
			x  =  -1.0f;
			y -= side_width;
		}

		return vertices;
	}

	void GLFWRenderer::DrawShapes(std::vector<GLfloat> vertices)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
		glEnableVertexAttribArray(1);
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
		glBindVertexArray(VAO);
		//eliminate the number of texture coordinates - they are not needed
		size_t total_nr_squares_coordinates = vertices.size() / 2;
		size_t nr_of_squares = total_nr_squares_coordinates / kNrCoordinatesPerSquare;
		glDrawArrays(GL_TRIANGLES, 0, kNrOfVerticesPerSquare * nr_of_squares);

		glfwSwapBuffers(window());
	}

	void GLFWRenderer::render() {
		if(nr_total_squares == 0)
			return;
		
		//get first set of squares coordinates
		std::vector<GLfloat> vertices {PopulateVertices(true)};
		texture.UseTexture();
		DrawShapes(vertices);

		//get second set of squares coordinates
		vertices = PopulateVertices(false);
		DrawShapes(vertices);
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