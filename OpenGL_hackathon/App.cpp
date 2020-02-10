#include "App.h"
#include <iostream>
#include <fstream>

void App::keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
	keys[key] = action;
}

App::App() :
	running(false)
{
	
	/*Create window and bind the rendering to it*/
	glfwInit();
	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL hack", 0, 0);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, this->keyCallback);

	int error = glewInit();
	if (error)
		std::cout << ((char*)glewGetErrorString(error));

	std::vector<GLuint> shaders;
	shaders.push_back(createShader(GL_VERTEX_SHADER, "forward.vert.glsl"));
	shaders.push_back(createShader(GL_FRAGMENT_SHADER, "forward.frag.glsl"));

	forwardProgram = createProgram(shaders);

	std::vector<vertex> vetexData = {
		{{0,0.5f,0}, {1,0,0}},
		{{0.5f,-0.5f,0}, {0,1,0}},
		{{-0.5f,-0.5f,0}, {0,0,1}},
	};
	vertexBuffer = createVertexBuffer(vetexData);
}

App::~App()
{
	glDeleteProgram(forwardProgram);
	glfwDestroyWindow(window);
}
void printShaderError(GLuint shader) {
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	std::vector<char> errorLog(maxLength);
	if (maxLength != 0)
	{
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shader);
		std::printf("%s\n", &(errorLog[0]));
	}
	else
	{
		std::printf("shader ok\n");
	}
}
void printProgramError(GLuint program) {
	GLint maxLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	std::vector<char> errorLog(maxLength);
	if (maxLength != 0)
	{
		glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
		std::printf("%s\n", &(errorLog[0]));
	}
	else
	{
		std::printf("program ok\n");
	}
}

GLuint App::createShader(GLuint shaderType, std::string filename)
{
	// allocate shader resource
	GLuint shader = glCreateShader(shaderType);
	std::fstream f;

	// read file into string
	f.open(filename);
	std::string shadersource((std::istreambuf_iterator<char>(f)), 
			(std::istreambuf_iterator<char>()));

	// set shader source and compile
	const char* data = shadersource.c_str();
	glShaderSource(shader, 1, &data, 0);
	glCompileShader(shader);

	printShaderError(shader);

	return shader;
}

GLuint App::createProgram(std::vector<GLuint>& shaders)
{
	// allocate a shader program resource
	GLuint program = glCreateProgram();

	//attach shaders to the program
	for (GLuint shader : shaders)
	{
		glAttachShader(program, shader);
	}
	//link program, program is ready to be used after this
	glLinkProgram(program);

	//delete allocated resources which are not needed anymore
	for (GLuint shader : shaders)
	{
		glDetachShader(program, shader);
		glDeleteShader(shader);
	}

	printProgramError(program);

	return program;
}

GLuint App::createVertexBuffer(std::vector<vertex>& vertices)
{
	GLuint vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
	
	return vb;
}

void App::update(double dt)
{
	if (keys[GLFW_KEY_ESCAPE])
		running = false;


	timeCounter += dt;
	glm::mat4 a = glm::lookAt(glm::vec3{ 0,0,5 }, { 0,0,-5 }, { 0,1,0 });
	glm::mat4 p = glm::perspective(120.f, 1.f, 0.1f, 10.f);
	world = p * a * glm::rotate(timeCounter, glm::vec3(1, 1, 1));
}

bool App::keys[GLFW_KEY_LAST] = {false};

void App::render()
{
	glUseProgram(forwardProgram);
	glBindBuffer(GL_VERTEX_ARRAY, vertexBuffer);
	glUniformMatrix4fv(2, 1, GL_FALSE, &world[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	glUseProgram(0);
}

void App::run()
{
	running = true;
	double previousTime = glfwGetTime(), currentTime = previousTime;
	double deltaTime;
	while (running)
	{
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime; //time since last frame
		previousTime = currentTime;
		update(deltaTime);
		render();
	}
}