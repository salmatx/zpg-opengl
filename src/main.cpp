#include "epch.h"
#include "gl_common.h"
#include "index_buffer.h"
#include "drawable_object.h"
#include "vertex_buffer.h"
#include "model.h"
#include "model_rotate.h"
#include "model_scale.h"
#include "model_translate.h"
#include "scene.h"
#include "shader.h"
#include "window.h"

#include "tree.h"
#include "sphere.h"

void PrintOpenGLInfo() {
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

int main() {
	int width = 800;
	int height = 600;

	engine::Window window(width, height, "ZPG");
	PrintOpenGLInfo();
	window.SetKeyCallbacks();

	engine::CameraDepth depth{0.1f, 100.0f};
	engine::CameraPosition position{
		glm::vec3(0.0f, 1.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	engine::Camera camera{window, width, height, depth, position};

	engine::Scene scene(camera);

	scene.AddShader("basic", "../res/shaders/basic.glsl");

	camera.InitCamera();

	scene.AddObject("tree", {tree, sizeof(tree), float{}, 3, float{}, 3});


	float alpha = 0;

	scene.SelectShader("basic");

	while (window.RenderLoop()) {
		scene.ClearScene();

		alpha += 1.0f;

		auto transformation = engine::Transformation({
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			alpha,
			engine::TransformationOrder::RTS
		});

		scene.AddTransformation("tree", transformation);
		scene.DrawScene();

		// update other events like input handling
		window.PollEvents();
		// put the stuff we’ve been drawing onto the display
		window.SwapBuffers();
	}

	return 0;
}