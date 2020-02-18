#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "Model.h"

class ModelBuilder
{
private:
	struct vertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};
	struct face
	{
		vertex a, b, c;
	};
	GLuint vertexBuffer;
	std::vector<vertex> vertices;
	std::vector<face> faces;
	glm::vec3 color;
public:
	ModelBuilder();
	ModelBuilder* createSquare();
	ModelBuilder* createSphere(int resX, int resY);
	ModelBuilder* setColor(glm::vec3 color);
	ModelBuilder* setColor(glm::vec3 colorA, glm::vec3 colorB, glm::vec3 colorC);
	Model* get();
};