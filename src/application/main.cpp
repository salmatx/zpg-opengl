#include <random>

#include "engine.h"

#include "tree.h"
#include "bushes.h"
#include "gift.h"
#include "sphere.h"
#include "triangle.h"

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

void InitTriangle(engine::Application& t_app, engine::Scene& t_scene) {
	auto triangle_pos = engine::Transformation({
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{3.0f, 3.0f, 3.0f},
			270,
			engine::TransformationOrder::TRS
		});

	t_app.UseShaderProgram(t_scene, "phong");
	t_scene.AddObject("triangle", {triangle, sizeof(triangle), float{}, 3, float{}, 3});
	t_scene.AddTransformation("triangle", triangle_pos);
}

void InitSpehers(engine::Application& t_app, engine::Scene& t_scene) {
	auto sphere1 = engine::Transformation({
			{3.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 1.0f, 1.0f},
			0,
			engine::TransformationOrder::TRS
		});
	auto sphere2 = engine::Transformation({
			{-3.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 1.0f, 1.0f},
			0,
			engine::TransformationOrder::TRS
		});
	auto sphere3 = engine::Transformation({
			{0.0f, 0.0f, 3.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 1.0f, 1.0f},
			0,
			engine::TransformationOrder::TRS
		});
	auto sphere4 = engine::Transformation({
			{0.0f, 0.0f, -3.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 1.0f, 1.0f},
			0,
			engine::TransformationOrder::TRS
		});


	t_app.UseShaderProgram(t_scene, "phong");
	t_scene.AddObject("sphere", {sphere, sizeof(sphere), float{}, 3, float{}, 3});
	t_scene.AddTransformation("sphere", sphere1);
	t_scene.AddTransformation("sphere", sphere2);
	t_scene.AddTransformation("sphere", sphere3);
	t_scene.AddTransformation("sphere", sphere4);
}

void InitForest(engine::Application& t_app, engine::Scene& t_scene) {
	t_app.UseShaderProgram(t_scene, "phong");
	t_scene.AddObject("tree", {tree, sizeof(tree), float{}, 3, float{}, 3});
	t_scene.AddObject("bushes", {bushes, sizeof(bushes), float{}, 3, float{}, 3});
	GenerateRandomObjects(t_scene, "tree", 150);
	GenerateRandomObjects(t_scene, "bushes", 80);
}

void InitGift(engine::Application& t_app, engine::Scene& t_scene) {
	t_app.UseShaderProgram(t_scene, "phong");
	t_scene.AddObject("gift", {gift, sizeof(gift), float{}, 3, float{}, 3});
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

	engine::LightParams light_params{
		{300.0f, 1000.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	};
	auto light = app.CreateLight(light_params);
	app.UseLight(light);

	app.CreateShaderProgram("phong", "../res/shaders/phong_vertex.glsl", "../res/shaders/phong_fragment.glsl");

	auto triangle = app.CreateScene();
	auto spheres = app.CreateScene();
	auto forest = app.CreateScene();
	auto gift = app.CreateScene();

	InitTriangle(app, triangle);
	InitSpehers(app, spheres);
	InitForest(app, forest);
	InitGift(app, gift);

	float alpha = 0.0f;
	while (app.Run()) {
		gift.ClearScene();

		alpha += 1.0f;
		auto transformation = engine::Transformation({
			{0.0, 15.0, -3.0},
			{0.0f, 1.0f, 0.0f},
			{8.0, 8.0, 8.0},
			alpha,
			engine::TransformationOrder::TRS
		});

		gift.AddTransformation("gift", transformation);

		forest.DrawScene();
		gift.DrawScene();

		// spheres.DrawScene();
		//
		// triangle.DrawScene();
	}
	return 0;
}