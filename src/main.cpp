//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include GLM
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include the standard C++ headers
#include <cstdlib>
#include <cstdio>

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

float points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

unsigned int indices[] = {
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


int main()
{
	engine::Window window(800, 600, "ZPG");
	window.InitWindow();

	PrintOpenGLInfo();

	window.SetKeyCallbacks();

	engine::VertexArray vao;
	engine::VertexBuffer vbo(points, sizeof(points));
	engine::VertexBufferLayout layout;
	layout.Push<float>(3);
	vao.AddBuffer(vbo, layout);

	engine::IndexBuffer ibo(indices, std::size(indices));

	engine::Shader shader("../res/shaders/basic.glsl");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);

	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();
	shader.Unbind();

	engine::Renderer renderer;

	while (window.RenderLoop()) {
		renderer.Clear();

		renderer.Draw(vao, ibo, shader);

		// update other events like input handling
		window.PollEvents();
		// put the stuff we’ve been drawing onto the display
		window.SwapBuffers();
	}

	return 0;
}