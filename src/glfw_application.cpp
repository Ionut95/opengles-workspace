#include "glfw_application.hpp"
#include "exception.hpp"
#include "context.hpp"
#include "input.hpp"
#include "main_loop.hpp"
#include "renderer.hpp"
#include "bouncing_box_state.hpp"
#include "manager.hpp"

#include <memory>
#include <iostream>
#include <cassert>

#define GLFW_WINDOW(ptr) reinterpret_cast<GLFWwindow*>(ptr)

namespace opengles_workspace
{
typedef void(*GLFWwindowDeleter)(GLFWwindow*);

void error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW error: %s\n", description);
}

void destroyGlfwWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
}

GlfwApplication::GlfwApplication(size_t width, size_t height)
	: mWidth(width)
	, mHeight(height)
{
	if(!glfwInit()) {
        throw Exception("Failed to initialize GLFW");
    }
    glfwSetErrorCallback(error_callback);
}

GlfwApplication::~GlfwApplication() {
	glfwTerminate();
}

int GlfwApplication::run() {
	auto pWindow = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(glfwCreateWindow(mWidth, mHeight, "OpenGL ES workspace", nullptr, nullptr), destroyGlfwWindow);
	if(!pWindow) {
		throw Exception("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(pWindow.get());
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	std::shared_ptr<BouncingBoxState> pBouncingBoxState = std::make_shared<BouncingBoxState>();
	

	MainLoop loop;
	auto ctx = std::make_shared<Context>(pWindow.get());
	std::shared_ptr<Input> pInput(Input::create(ctx));
	std::shared_ptr<GLFWRenderer> pRenderer = std::make_shared<GLFWRenderer>(ctx);
	pInput->registerKeyCallback([&](Key key, KeyMode keyMode) {
			if (key == Key::ESCAPE && keyMode == KeyMode::PRESS) {
				glfwSetWindowShouldClose(pWindow.get(), GLFW_TRUE);
				return false;
			}
			return true;
		});
	
	std::shared_ptr<Manager> pManager = std::make_shared<Manager>(pBouncingBoxState, pRenderer);

	loop.addPolledObject(pManager);
	loop.addPolledObject(pInput);
	//loop.addPolledObject(pRenderer);
	pRenderer->render(pBouncingBoxState->GetObjectRow(),pBouncingBoxState->GetObjectColumn(),pBouncingBoxState->GetNrRows(),pBouncingBoxState->GetNrColumns());
	loop.run();
	return 0;
}
}