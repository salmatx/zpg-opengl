#include <random>
#include <utility>

#include "engine.h"

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

void InitForest(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
	std::vector<std::shared_ptr<engine::Light>> t_lights) {
	t_app.UseShaderProgram(t_scene, "phong", t_camera, t_lights);
	t_scene.AddObject("tree", {tree, sizeof(tree), float{}, 3, float{}, 3});
	t_scene.AddObject("bushes", {bushes, sizeof(bushes), float{}, 3, float{}, 3});
	GenerateRandomObjects(t_scene, "tree", 150);
	GenerateRandomObjects(t_scene, "bushes", 80);
}

void InitGift(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	t_app.UseShaderProgram(t_scene, "phong", t_camera, t_lights);
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
	auto camera = app.CreateCamera(position, depth);

	engine::DirectionalLightParams_t light_params{
		{-0.2f, -1.0f, -0.3f},
		{0.05f, 0.05f, 0.05f},
		{0.4f, 0.4f, 0.4f},
		{0.5f, 0.5f, 0.5f}
	};

	engine::PointLightParams_t pointLightParams {
		glm::vec3(0.0f, 10.0f, 5.0f),
		1.0f,
		0.09f,
		0.032f,
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};

	engine::FlashLightParams_t flashLightParams {
		15.0f,
		20.0f,
		0.07f,
		1.0f,
		0.032f,
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};

	std::vector<std::shared_ptr<engine::Light>> lights;
	// lights.emplace_back(app.CreateDirectionalLight(light_params));
	// lights.emplace_back(app.CreatePointLight(pointLightParams));
	lights.emplace_back(app.CreateFlashLight(flashLightParams, camera));

	app.CreateShaderProgram("phong", "../res/shaders/phong_vertex.glsl", "../res/shaders/flashlight.glsl");

	auto forest = app.CreateScene();
	auto gift = app.CreateScene();

	InitForest(app, forest, camera, lights);
	InitGift(app, gift, camera, lights);

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

	}
	return 0;
}