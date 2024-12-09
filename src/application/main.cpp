#include <random>
#include <utility>

#include "engine.h"

#include "tree.h"
#include "bushes.h"
#include "gift.h"
#include "sphere.h"
#include "skycube.h"

using RandGenType = std::mt19937_64;

std::random_device rd{};
RandGenType rand_gen{rd()};
std::uniform_real_distribution uniform_position(-30.0f, 30.0f);
std::uniform_real_distribution uniform_rotation(0.0f, 100.0f);
std::uniform_real_distribution uniform_scale(0.1f, 0.2f);

static void GenerateRandomObjects(engine::Scene& t_scene, const std::string& t_name, const int t_count) {
	for (int i = 0; i < t_count; ++i) {
		float x_pos = uniform_position(rand_gen);
		float y_pos = 0.0f;
		float z_pos = uniform_position(rand_gen);

		if (x_pos < 10.0f && x_pos > -10.0f) {
			x_pos *= 2;
		}

		if (z_pos < 10.0f && z_pos > -10.0f) {
			z_pos *= 2;
		}

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

void InitHouse(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObjectWithTexture("house", "../res/models/house.obj", shader);

	auto transformation = engine::Transformation({
			{0.0f, 0.0f, 10.0f},
			{0.0f, 1.0f, 0.0f},
			{0.8f, 0.8f, 0.8f},
			0,
			engine::TransformationOrder::RTS
		});

	t_scene.AddTransformation("house", transformation);

	t_scene.AddTexture("house", {"../res/textures/house.png"});
}

void InitGround(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObjectWithTexture("ground", "../res/models/teren.obj", shader);

	auto transformation = engine::Transformation({
			{0.0f, 0.0f, 10.0f},
			{0.0f, 1.0f, 0.0f},
			{2.0f, 2.0f, 2.0f},
			0,
			engine::TransformationOrder::RTS
		});

	t_scene.AddTransformation("ground", transformation);

	t_scene.AddTexture("ground", {"../res/textures/grass.png"});
}

void InitTree(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObjectWithTexture("tree", "../res/models/tree.obj", shader);

	GenerateRandomObjects(t_scene, "tree", 100);

	t_scene.AddTexture("tree", {"../res/textures/tree.png"});
}

void InitSkycube(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	t_app.CreateShaderProgram("skycube", "../res/shaders/skycube_vertex.glsl", "../res/shaders/skycube_fragment.glsl");
	auto shader = t_app.UseShaderProgram("skycube", t_camera, t_lights);
	t_scene.AddObjectWithoutTexture("skycube",
		std::make_unique<engine::DrawableObject>(skycube, shader, sizeof(skycube), float{}, 3));

	auto transformation = engine::Transformation({
			{0.0f, 0.0f, 0.0f},
			{1.0f, 1.0f, 1.0f},
			{0.2f, 0.2f, 0.2f},
			0,
			engine::TransformationOrder::TRS
		});

	t_scene.AddTransformation("skycube", transformation);

	t_scene.AddCubeMap("skycube",
{"../res/textures/cubemap/posx.jpg",
		"../res/textures/cubemap/negx.jpg",
		"../res/textures/cubemap/posy.jpg",
		"../res/textures/cubemap/negy.jpg",
		"../res/textures/cubemap/posz.jpg",
		"../res/textures/cubemap/negz.jpg"});
}

void InitSpehre(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
	std::vector<std::shared_ptr<engine::Light>>& t_lights, const std::vector<glm::vec3>& t_positions) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObjectWithoutTexture("sphere",
		std::make_unique<engine::DrawableObject>(sphere, shader, sizeof(sphere), float{}, 3, float{}, 3));

	for (const auto& position : t_positions) {

		auto transformation = engine::Transformation({
			position,
			{0.0f, 1.0f, 0.0f},
			{0.1f, 0.1f, 0.1f},
			0,
			engine::TransformationOrder::TRS
		});

		t_scene.AddTransformation("sphere", transformation);
	}
}

void InitLightBulbs(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
	std::vector<std::shared_ptr<engine::Light>>& t_lights) {
	std::vector<glm::vec3> positions {
		{-3.0f, 1.7f, 3.5f},
		{-3.0f, 1.7f, 6.3f},
		{-3.0f, 1.7f, 9.0f},
		{-3.0f, 1.7f, 11.0f},
		{-3.0f, 1.7f, 13.7f},
		{-3.0f, 1.7f, 16.6f}
	};

	for (const auto& position : positions) {
		engine::PointLightParams_t pointLightParams {
				position,
			1.0f,
			0.15f,
		0.25f,
		glm::vec3(0.15f, 0.15f, 0.15f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
			};
		t_lights.emplace_back(t_app.CreatePointLight(pointLightParams));
	}

	InitSpehre(t_app, t_scene, t_camera, t_lights, positions);
}

int main() {
	engine::Application app(800, 600, "ZPG");

	engine::CameraDepth depth{0.1f, 100.0f};
	engine::CameraPosition position{
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 1.0f, 0.0f}
	};
	auto camera = app.CreateCamera(position, depth);

	engine::DirectionalLightParams_t light_params{
		glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),
	{0.4f, 0.4f, 0.4f},
	 {1.2f, 1.0f, 0.8f},
	{0.7f, 0.6f, 0.5f}

	};

	engine::FlashLightParams_t flashLightParams {
		12.5f,
		17.5f,
		0.07f,
		1.0f,
		0.032f,
		glm::vec3(0.3f, 0.3f, 0.3f),
	glm::vec3(1.2f, 1.2f, 1.2f),
	glm::vec3(1.5f, 1.5f, 1.5f)
	};

	app.CreateShaderProgram("phong", "../res/shaders/phong_vertex.glsl", "../res/shaders/multiple_lights_fragment.glsl");

	std::vector<std::shared_ptr<engine::Light>> lights;
	lights.emplace_back(app.CreateDirectionalLight(light_params));
	lights.emplace_back(app.CreateFlashLight(flashLightParams, camera));

	auto light_bulbs = app.CreateScene();
	InitLightBulbs(app, light_bulbs, camera, lights);


	auto skycube = app.CreateScene();
	InitSkycube(app, skycube, camera, lights);

	auto ground = app.CreateScene();
	InitGround(app, ground, camera, lights);

	auto house_trees = app.CreateScene();
	InitHouse(app, house_trees, camera, lights);

	InitTree(app, house_trees, camera, lights);

	while (app.Run()) {
		skycube.DrawSkybox();
		ground.DrawScene();
		house_trees.DrawScene();
		light_bulbs.DrawScene();
	}
	return 0;
}