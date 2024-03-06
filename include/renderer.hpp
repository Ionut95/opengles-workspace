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

namespace opengles_workspace
{
class GLFWRenderer : public PolledObject
	{
	public:
		GLFWRenderer(std::shared_ptr<Context> context);

		~GLFWRenderer();

		void render();

		bool poll() override;
	private:
		//kNrTotalSquares needs to be perfect square
		const size_t kNrTotalSquares = 64;
		const size_t kHalfNrSquares = kNrTotalSquares / 2;
		//calculation needed to solve the problem of even/odd numbers
		const size_t kNrSquaresToBeColored = (kNrTotalSquares % 2 == 0) ? kHalfNrSquares : kHalfNrSquares + 1;
		const size_t kNrOfVerticesPerSquare = 6;
		const size_t kNrRows = sqrt(kNrTotalSquares);
		const size_t kNrColumns = sqrt(kNrTotalSquares);
		bool is_background_colored = false;

		GLuint VAO; 
		GLuint VBO;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;
		//GLint uniform;

		static const char* vertexShaderSource;
		static const char* fragmentShaderSource;

		std::vector<GLfloat> PopulateVertices(bool is_same_index);
		void DrawShapes(std::vector<GLfloat> vertices, size_t totalVerticesPerShape, std::vector<GLfloat> color);

		std::shared_ptr<Context> mContext;
		GLFWwindow* window() const { return static_cast<GLFWwindow*>(mContext->window()); }
	};
}