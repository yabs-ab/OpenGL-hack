#include "App.h"
#include <iostream>
#include <fstream>
#include "ModelBuilder.h"

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

	/*std::vector<vertex> vetexData = {
		{{0,0.5f,0}, {1,0,0}},
		{{0.5f,-0.5f,0}, {0,1,0}},
		{{-0.5f,-0.5f,0}, {0,0,1}},
	};
	vertexBuffer = createVertexBuffer(vetexData);*/
	ModelBuilder m;
	models.push_back(m.createSphere(20, 20)->setColor({1,1,1}, {150,150,150}, {255,0,0})->get());
	//models.push_back(m.createSquare()->setColor({ 1,1,1 }, { 150,150,150 }, { 255,0,0 })->get());
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

void App::update(double dt)
{
	if (keys[GLFW_KEY_ESCAPE])
		running = false;

	for (Model* m : models)
	{
		m->update(dt);
	}

	timeCounter += dt;
	glm::mat4 a = glm::lookAt(glm::vec3{ 0,0,5 }, { 0,0,-5 }, { 0,1,0 });
	glm::mat4 p = glm::perspective(120.f, 1.f, 0.1f, 10.f);
	viewProjectionMatrix = p * a;
}

bool App::keys[GLFW_KEY_LAST] = {false};

void App::render()
{
	glUseProgram(forwardProgram);
	for (Model* m : models)
	{
		m->render(&viewProjectionMatrix);
	}
	glUseProgram(0);
}

void App::run()
{
	running = true;
	double previousTime = glfwGetTime(), currentTime = previousTime;
	double deltaTime;
	glEnable(GL_DEPTH_TEST);
	while (running)
	{
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime; //time since last frame
		previousTime = currentTime;
		update(deltaTime);
		render();
	}
}