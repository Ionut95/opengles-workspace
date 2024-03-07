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
		size_t nr_rows = 0;
		size_t nr_columns = 0;
		size_t nr_total_squares = 0;
		const size_t kNrCoordinatesPerSquare = 12;
		const size_t kNrOfVerticesPerSquare = 6;
		bool is_background_colored = false;

		GLuint VAO; 
		GLuint VBO;
		GLuint shaderProgram;

		static const char* vertexShaderSource;
		static const char* fragmentShaderSource;

		std::vector<GLfloat> PopulateVertices(bool is_same_index);
		void DrawShapes(std::vector<GLfloat> vertices, std::vector<GLfloat> color);
		std::tuple<size_t, size_t> ReadData() const;
		size_t GetValueFromLine(std::string line) const;

		std::shared_ptr<Context> mContext;
		GLFWwindow* window() const { return static_cast<GLFWwindow*>(mContext->window()); }
	};
}