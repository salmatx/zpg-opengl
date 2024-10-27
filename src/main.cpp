#include "epch.h"
#include "gl_common.h"
#include "index_buffer.h"
#include "drawable_object.h"
#include "vertex_buffer.h"
#include "model.h"
#include "model_rotate.h"
#include "model_scale.h"
#include "model_translate.h"
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

glm::vec3 positions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  1.0f, -5.0f)
};

//GLM test

// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f,
100.0f);

// Camera matrix
glm::mat4 View = glm::lookAt(
	glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
	glm::vec3(0, 0, 0), // and looks at the origin
	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
);

int main() {
	engine::Window window(800, 600, "ZPG");
	window.InitWindow();
	PrintOpenGLInfo();
	window.SetKeyCallbacks();


	// engine::Model vao(tree, sizeof(tree));
	// vao.SetLayout(float{}, 3, float{}, 3);
	// vao.InitModel();

	engine::Shader shader_basic("../res/shaders/basic.glsl");
	shader_basic.Bind();

	engine::DrawableObject renderer(tree, sizeof(tree), float{}, 3, float{}, 3);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 M = glm::mat4(1.0f);

	float alpha = 0;
	// std::vector<float> alpha;
	// for (int i = 0; i < 10; i++) {
	// 	alpha.push_back(i * 100.0f);
	// }

	GLint matrixID;
	glEnable(GL_DEPTH_TEST); //Z-buffer

	while (window.RenderLoop()) {
		renderer.Clear();


		M = glm::lookAt(glm::vec3(5.0f,10.0f,0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		shader_basic.SetUniformMat4f("u_view_matrix", M);

		M = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 100.0f);
		shader_basic.SetUniformMat4f("u_project_matrix", M);

		alpha += 1.0f;
		renderer.AddTransformation({
			std::make_unique<engine::ModelRotate>(alpha, glm::vec3(0.0f, 1.0f, 0.0f)),
			std::make_unique<engine::ModelTranslate>(positions[0]),
			std::make_unique<engine::ModelScale>(glm::vec3(1.0f, 1.0f, 1.0f)),
			});
		renderer.AddTransformation({
			std::make_unique<engine::ModelRotate>(-alpha, glm::vec3(0.0f, 1.0f, 0.0f)),
			std::make_unique<engine::ModelTranslate>(positions[0]),
			std::make_unique<engine::ModelScale>(glm::vec3(1.0f, 1.0f, 1.0f)),
			});
		renderer.Draw(shader_basic);

		// M = glm::rotate(glm::mat4(1.0f), alpha, glm::vec3(0.0f, 1.0f, 1.0f));
		// shader_basic.SetUniformMat4f("u_model_matrix", M);
		// renderer.Draw(vao, shader_basic);

		// M = glm::rotate(glm::mat4(1.0f), -alpha, glm::vec3(0.0f, 0.0f, 1.0f));
		// shader_basic.SetUniformMat4f("u_model_matrix", M);
		// renderer.Draw(shader_basic);

		// update other events like input handling
		window.PollEvents();
		// put the stuff we’ve been drawing onto the display
		window.SwapBuffers();
	}

	return 0;
}