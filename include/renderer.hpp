#pragma once
#include <memory>
#include <optional>
#include <context.hpp>
#include <polled_object.hpp>
#include <cmath>
#include <vector>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "texture.h"
#include "timer.hpp"
#include "matrix_generation.hpp"

namespace opengles_workspace
{
class GLFWRenderer : public PolledObject
	{
	public:
		GLFWRenderer(std::shared_ptr<Context> context);

		~GLFWRenderer();

		void render();

		bool poll() override;

		void CheckMovementDirection();
		void MoveObject(GLfloat offset);

	private:
		std::vector<GLfloat> vertices_ {
			-0.0625f, -0.875f,
			 0.0625f, -0.875f,
			-0.0625f, -1.0f,

			 0.0625f, -0.875f,
			-0.0625f, -1.0f,
			 0.0625f, -1.0f 
		};

		MatrixGeneration window_matrix;

		std::chrono::system_clock::time_point last_rendered_ = std::chrono::system_clock::now();
		bool is_going_up_ = true;

		size_t nr_rows = 0;
		size_t nr_columns = 0;
		size_t nr_total_squares = 0;
		const size_t kNrCoordinatesPerSquare = 12;
		const size_t kNrOfVerticesPerSquare = 6;

		GLuint VAO; 
		GLuint VBO;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;

		static const char* vertexShaderSource;
		static const char* fragmentShaderSource;

		void DrawShapes(std::vector<GLfloat> vertices);

		std::tuple<size_t, size_t> ReadData() const;
		size_t GetValueFromLine(std::string line) const;

		void PrepareVertexShader();
		void PrepareFragmentShader();
		void LinkProgram();

		std::shared_ptr<Context> mContext;
		GLFWwindow* window() const { return static_cast<GLFWwindow*>(mContext->window()); }
	};
}