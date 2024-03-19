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

#include "box_coordinates.hpp"
#include "program.hpp"

namespace opengles_workspace
{
class GLFWRenderer : public PolledObject
	{
	public:
		GLFWRenderer(std::shared_ptr<Context> context);

		~GLFWRenderer() = default;

		void render(std::size_t row, std::size_t column, std::size_t nr_total_rows, std::size_t nr_total_columns);

		bool poll() override;

	private:
		void ClearColour(float r, float g, float b, float a) const;

		Program program_;
		BoxCoordinates box_coordinates_;
		std::shared_ptr<Context> mContext;
		GLFWwindow* window() const { return static_cast<GLFWwindow*>(mContext->window()); }
	};
}