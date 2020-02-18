#pragma once
#include <glm/glm.hpp>
#include <GL\glew.h>

class Model
{
public:
	Model(GLuint vb, int vc);
	void render(glm::mat4* viewProj);
	void update(float dt);
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 world;

	GLuint vertexBuffer;
	int vertexCount;
};