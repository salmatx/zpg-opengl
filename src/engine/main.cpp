#include <random>

#include "application.h"
#include "epch.h"
#include "drawable_object.h"
#include "scene.h"
#include "shader.h"

#include "tree.h"
#include "bushes.h"
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

int main() {
	engine::Application app(800, 600, "ZPG");


	engine::CameraDepth depth{0.1f, 100.0f};
	engine::CameraPosition position{
		{0.0f, 1.0f, 10.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 1.0f, 0.0f}
	};
	app.CreateCamera(position, depth);

	auto scene = app.CreateScene();
	auto scene1 = app.CreateScene();

	app.LinkShader("basic", "../res/shaders/basic.glsl");
	app.SetShader(scene, "basic");
	app.SetShader(scene1, "basic");

	scene.AddObject("tree", {tree, sizeof(tree), float{}, 3, float{}, 3});
	scene.AddObject("bushes", {bushes, sizeof(bushes), float{}, 3, float{}, 3});
	scene1.AddObject("gift", {gift, sizeof(gift), float{}, 3, float{}, 3});

	GenerateRandomObjects(scene, "tree", 150);
	GenerateRandomObjects(scene, "bushes", 80);

	float alpha = 0.0f;
	while (app.Run()) {
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
	}
	return 0;
}