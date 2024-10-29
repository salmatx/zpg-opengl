#include <random>

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
#include "bushes.h"
#include "sphere.h"
#include "gift.h"

using RandGenType = std::mt19937_64;

std::random_device rd{};
RandGenType rand_gen{rd()};
std::uniform_real_distribution uniform_position(-50.0f, 50.0f);
std::uniform_real_distribution uniform_rotation(0.0f, 100.0f);
std::uniform_real_distribution uniform_scale(0.6f, 1.2f);

static void GenerateRandomObjects(engine::Scene& t_scene, const std::string& t_name, const int t_count) {
	for (int i = 0; i < t_count; ++i) {
		float x_pos = uniform_position(rand_gen);
		float y_pos = 0.0f;
		float z_pos = uniform_position(rand_gen);

		float rot = uniform_rotation(rand_gen);

		float scale = uniform_scale(rand_gen);

		auto transformation = engine::Transformation({
			{x_pos, y_pos, z_pos},
			{0.0f, 1.0f, 0.0f},
			{scale, scale, scale},
			rot,
			engine::TransformationOrder::RTS
		});

		t_scene.AddTransformation(t_name, transformation);
	}
}


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
		{0.0f, 1.0f, 10.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 1.0f, 0.0f}
	};

	engine::Camera camera{window, width, height, depth, position};

	engine::Scene scene(camera);
	engine::Scene scene1(camera);

	scene.AddShader("basic", "../res/shaders/basic.glsl");
	scene1.AddShader("basic", "../res/shaders/basic.glsl");

	camera.InitCamera();

	scene.AddObject("tree", {tree, sizeof(tree), float{}, 3, float{}, 3});
	scene.AddObject("bushes", {bushes, sizeof(bushes), float{}, 3, float{}, 3});

	scene1.AddObject("gift", {gift, sizeof(gift), float{}, 3, float{}, 3});

	scene.SelectShader("basic");
	scene1.SelectShader("basic");

	GenerateRandomObjects(scene, "tree", 150);
	GenerateRandomObjects(scene, "bushes", 80);

	float alpha = 0.0f;

	while (window.RenderLoop()) {
		scene1.ClearScene();

		alpha += 1.0f;
		auto transformation = engine::Transformation({
			{0.0, 15.0, -3.0},
			{0.0f, 1.0f, 0.0f},
			{8.0, 8.0, 8.0},
			alpha,
			engine::TransformationOrder::TRS
		});

		scene1.AddTransformation("gift", transformation);

		scene.DrawScene();
		scene1.DrawScene();

		// update other events like input handling
		window.PollEvents();
		// put the stuff we’ve been drawing onto the display
		window.SwapBuffers();
	}

	return 0;
}