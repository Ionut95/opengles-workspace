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
		//, window_matrix(MatrixGeneration())
	{
		std::cout << "\nsize matrix: "; //<< window_matrix.window_matrix_[0][0].size();

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
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* GLFWRenderer::fragmentShaderSource = "#version 300 es\n"
		"precision lowp float;\n"
		//"uniform vec3 figColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.7f, 0.35f, 0.0f, 1.0f);\n"
		"}\n\0";

	void GLFWRenderer::DrawShapes(std::vector<GLfloat> vertices)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		glClearColor(1.0f, 0.8f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		size_t nr_of_squares = vertices.size() / kNrCoordinatesPerSquare;
		glDrawArrays(GL_TRIANGLES, 0, kNrOfVerticesPerSquare * nr_of_squares);

		glfwSwapBuffers(window());
	}

	void GLFWRenderer::render() {
		if(nr_total_squares == 0)
			return;
		
		last_rendered_ = std::chrono::system_clock::now();
		DrawShapes(vertices_);
	}

	bool GLFWRenderer::poll() {

		auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_rendered_).count() / 1000.0f;
		const int fps = 100;
		if(delta_time >= 1.0 / fps)
		{
			CheckMovementDirection();
			render();
		}

		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}

	void GLFWRenderer::CheckMovementDirection() 
	{
		GLfloat box_down_side = vertices_[11];
		GLfloat box_up_side = vertices_[1];
		GLfloat down_boundary = -1.0f;
		GLfloat up_boundary = 1.0f;

		if(box_down_side <= down_boundary)
			is_going_up_ = true;

		if(box_up_side >= up_boundary)
			is_going_up_ = false;

		if(is_going_up_)
			MoveObject(0.00625f);
		else
			MoveObject(-0.00625f);
	}

	void GLFWRenderer::MoveObject(GLfloat offset) 
	{
		for(size_t it = 0; it < vertices_.size(); ++it)
		{
			if(it % 2 != 0)
				vertices_[it] += offset;
		}
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