#pragma once
#include <GL\glew.h>
#include <gl\GL.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Model.h"

class App
{
public:
	App();
	~App();
	void run();
	GLuint createShader(GLuint shaderType, std::string filename);
	GLuint createProgram(std::vector<GLuint>& shaders);
	void render();

	static void keyCallback(GLFWwindow* w, int key, int scancode, int actions, int mods);
	
private:
	void update(double dt);

	const unsigned int WIDTH = 400;
	const unsigned int HEIGHT = 400;
	GLFWwindow* window;
	bool running;
	GLuint forwardProgram;
	GLuint vertexBuffer;
	
	std::vector<Model*> models;
	glm::mat4 viewProjectionMatrix;
	float timeCounter;

	static bool keys[GLFW_KEY_LAST];
};