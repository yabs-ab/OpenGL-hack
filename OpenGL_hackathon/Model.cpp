#include "Model.h"
#include <glm/gtx/transform.hpp>

Model::Model(GLuint vb, int vc)
{
	this->vertexBuffer = vb;
	this->vertexCount = vc;
}

void Model::render(glm::mat4* viewProj)
{
	world = *viewProj * world;
	glUniformMatrix4fv(2, 1, GL_FALSE, &world[0][0]);
	glBindVertexArray(vertexBuffer);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Model::update(float dt)
{
	rotation += glm::vec3(dt,dt,dt);
	world = glm::rotate(rotation.x, glm::vec3(1,1,1));
	
}