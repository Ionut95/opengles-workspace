#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "rectangle.hpp"

namespace opengles_workspace
{
	GLFWRenderer::GLFWRenderer(std::shared_ptr<Context> context)
		: mContext(std::move(context))
		, box_coordinates_(BoxCoordinates()) 
	{
		glViewport(0, 0, 800, 800);
	}

	void GLFWRenderer::render(std::size_t row, std::size_t column, std::size_t nr_total_rows, std::size_t nr_total_columns) {
		ClearColour(1.0f, 0.8f, 0.4f, 1.0f);
		program_.useProgram();
		Rectangle rectangle = Rectangle(box_coordinates_.CalculateBoxCoordinates(row, column, nr_total_rows, nr_total_columns));
		rectangle.RenderRectangle();

		glfwSwapBuffers(window());
	}

	void GLFWRenderer::ClearColour(float r, float g, float b, float a) const
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	bool GLFWRenderer::poll() {
		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}
}