#include "epch.h"
#include "gl_common.h"
#include "index_buffer.h"
#include "renderer.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "window.h"

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

float points_rectangle[] = {
	-1.0f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, -0.5f, 0.0f,    1.0f, 1.0f, 0.0f, 1.0f
};

float points_triangle[] = {
     0.0f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f
};

unsigned int indices_rectangle[] = {
	0, 1, 2,
	0, 2, 3
};

unsigned int indices_triangle[] = {
	0, 1, 2
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
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model = glm::mat4(1.0f);


int main() {
	engine::Window window(800, 600, "ZPG");
	window.InitWindow();
	PrintOpenGLInfo();
	window.SetKeyCallbacks();

	engine::VertexArray vao;
	engine::VertexBuffer vbo(points_rectangle, sizeof(points_rectangle));
	engine::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	vao.AddBuffer(vbo, layout);
	engine::IndexBuffer ibo(indices_rectangle, std::size(indices_rectangle));

	engine::VertexArray vao1;
	engine::VertexBuffer vbo1(points_triangle, sizeof(points_triangle));
	engine::VertexBufferLayout layout1;
	layout1.Push<float>(3);
	vao1.AddBuffer(vbo1, layout1);
	engine::IndexBuffer ibo1(indices_triangle, std::size(indices_triangle));

	engine::Shader shader_basic("../res/shaders/basic.glsl");
	shader_basic.Bind();
	shader_basic.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);

	engine::Shader shader_gradient("../res/shaders/gradient.glsl");
	shader_gradient.Bind();

	engine::Renderer renderer;

	while (window.RenderLoop()) {
		renderer.Clear();

		renderer.Draw(vao, ibo, shader_gradient);
		renderer.Draw(vao1, ibo1, shader_basic);

		// update other events like input handling
		window.PollEvents();
		// put the stuff we’ve been drawing onto the display
		window.SwapBuffers();
	}

	return 0;
}