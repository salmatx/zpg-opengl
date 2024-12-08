#include <random>
#include <utility>

#include "engine.h"

#include "tree.h"
#include "bushes.h"
#include "gift.h"
#include "sphere.h"

using RandGenType = std::mt19937_64;

std::random_device rd{};
RandGenType rand_gen{rd()};
std::uniform_real_distribution uniform_position(-50.0f, 50.0f);
std::uniform_real_distribution uniform_rotation(0.0f, 100.0f);
std::uniform_real_distribution uniform_scale(0.6f, 1.2f);

float points[] = {
0.000000f, -0.500000f, 0.500000f, -0.872900f, 0.218200f, 0.436400f, 0.836598f, 0.477063f,
0.000000f, 0.500000f, 0.000000f, -0.872900f, 0.218200f, 0.436400f, 0.399527f, 0.286309f,
-0.500000f, -0.500000f, -0.500000f, -0.872900f, 0.218200f, 0.436400f, 0.836598f, 0.000179f,
-0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.381686f, 0.999821f,
0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.000179f, 0.809067f,
0.000000f, -0.500000f, 0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.381686f, 0.522937f,
0.500000f, -0.500000f, -0.500000f, 0.872900f, 0.218200f, 0.436400f, 0.399169f, 0.000179f,
0.000000f, 0.500000f, 0.000000f, 0.872900f, 0.218200f, 0.436400f, 0.399169f, 0.522579f,
0.000000f, -0.500000f, 0.500000f, 0.872900f, 0.218200f, 0.436400f, 0.000179f, 0.261379f,
-0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.447200f, -0.894400f, 0.788901f, 0.477421f,
0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.447200f, -0.894400f, 0.788901f, 0.999821f,
0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.447200f, -0.894400f, 0.399527f, 0.651554f
};

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

std::vector<glm::vec3> GenerateRandomVertices(const int t_count) {
	auto vertices = std::vector<glm::vec3>(t_count);
	for (int i = 0; i < t_count; ++i) {
		vertices.emplace_back(uniform_position(rand_gen), 7.0f, uniform_position(rand_gen));
	}

	return vertices;
}

void InitForest(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
	std::vector<std::shared_ptr<engine::Light>> t_lights) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObject("tree", std::make_unique<engine::DrawableObject>
		(tree, shader, sizeof(tree), float{}, 3, float{}, 3));
	t_scene.AddObject("bushes", std::make_unique<engine::DrawableObject>
		(bushes, shader, sizeof(bushes), float{}, 3, float{}, 3));
	GenerateRandomObjects(t_scene, "tree", 150);
	GenerateRandomObjects(t_scene, "bushes", 80);
	t_scene.AddTexture("tree", {"../res/textures/wooden_fence.png"});
	t_scene.AddTexture("bushes", {"../res/textures/wooden_fence.png"});
}

void InitGift(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObject("gift", std::make_unique<engine::DrawableObject>
		(gift, shader, sizeof(gift), float{}, 3, float{}, 3));
}

void InitTriangle(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
std::vector<std::shared_ptr<engine::Light>> t_lights) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObject("triangle", std::make_unique<engine::DrawableObject>
		(points, shader, sizeof(points), float{}, 3, float{}, 3, float{}, 2));

	auto transformation = engine::Transformation({
			{0.0f, 1.0f, 10.0f},
			{0.0f, 1.0f, 0.0f},
			{0.1f, 0.1f, 0.1f},
			0,
			engine::TransformationOrder::RTS
		});

	t_scene.AddTransformation("triangle", transformation);

	t_scene.AddTexture("triangle", {"../res/textures/wooden_fence.png"});
}

void GenerateFireFlights(engine::Application& t_app, std::vector<std::shared_ptr<engine::Light>>& t_lights, const std::vector<glm::vec3>& t_positions) {
	for (const auto& position : t_positions) {
		engine::PointLightParams_t pointLightParams {
				position,
				1.0f,
				0.09f,
				0.032f,
				glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(0.9f, 0.9f, 0.9f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			};
		t_lights.emplace_back(t_app.CreatePointLight(pointLightParams));
	}
}

void InitSpehre(engine::Application& t_app, engine::Scene& t_scene, std::shared_ptr<engine::Camera> t_camera,
	std::vector<std::shared_ptr<engine::Light>>& t_lights, const std::vector<glm::vec3>& t_positions) {
	auto shader = t_app.UseShaderProgram("phong", t_camera, t_lights);
	t_scene.AddObject("sphere", std::make_unique<engine::DrawableObject>
		(sphere, shader, sizeof(sphere), float{}, 3, float{}, 3));

	for (const auto& position : t_positions) {

		auto transformation = engine::Transformation({
			position,
			{0.0f, 1.0f, 0.0f},
			{0.1f, 0.1f, 0.1f},
			0,
			engine::TransformationOrder::RTS
		});

		t_scene.AddTransformation("sphere", transformation);
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
	auto camera = app.CreateCamera(position, depth);

	engine::DirectionalLightParams_t light_params{
		{-0.2f, -1.0f, -0.3f},
		{0.02f, 0.02f, 0.02f},
		{0.1f, 0.1f, 0.1f},
		{0.5f, 0.5f, 0.5f}
	};

	engine::FlashLightParams_t flashLightParams {
		12.5f,
		17.5f,
		0.07f,
		1.0f,
		0.032f,
		{0.2f, 0.2f, 0.2f},
		{0.8f, 0.8f, 0.8f},
		{1.0f, 1.0f, 1.0f}
	};

	auto positions = GenerateRandomVertices(10);
	std::vector<std::shared_ptr<engine::Light>> lights;
	lights.emplace_back(app.CreateDirectionalLight(light_params));
	lights.emplace_back(app.CreateFlashLight(flashLightParams, camera));
	GenerateFireFlights(app, lights, positions);

	app.CreateShaderProgram("phong", "../res/shaders/phong_vertex.glsl", "../res/shaders/multiple_lights_fragment.glsl");

	auto forest = app.CreateScene();
	// auto gift = app.CreateScene();
	// auto sphere = app.CreateScene();
	//
	InitForest(app, forest, camera, lights);
	// InitGift(app, gift, camera, lights);
	// InitSpehre(app, sphere, camera, lights, positions);

	auto triangle = app.CreateScene();
	InitTriangle(app, triangle, camera, lights);

	float alpha = 0.0f;
	while (app.Run()) {
		// gift.ClearScene();
		//
		// alpha += 1.0f;
		// auto transformation = engine::Transformation({
		// 	{0.0, 20.0, -3.0},
		// 	{0.0f, 1.0f, 0.0f},
		// 	{8.0, 8.0, 8.0},
		// 	alpha,
		// 	engine::TransformationOrder::TRS
		// });
		//
		// gift.AddTransformation("gift", transformation);
		//
		forest.DrawScene();
		// gift.DrawScene();
		// sphere.DrawScene();

		triangle.DrawScene();
	}
	return 0;
}