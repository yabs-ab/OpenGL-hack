#include "ModelBuilder.h"

ModelBuilder::ModelBuilder() :
	vertices(),
	faces()
{
	vertexBuffer = -1;
}

ModelBuilder* ModelBuilder::createSquare()
{
	faces.clear();

	vertex p[8];
	p[0].position = { -1,-1,-1 };
	p[1].position = {  1,-1,-1 };
	p[2].position = { -1, 1,-1 };
	p[3].position = {  1, 1,-1 }; 
	p[4].position = { -1,-1, 1 };
	p[5].position = {  1,-1, 1 };
	p[6].position = { -1, 1, 1 };
	p[7].position = {  1, 1, 1 };

	for ( int i =0; i < 8; i++)
	{
		p[i].color = color;
	}
	
	faces.push_back({ p[0], p[1], p[2] }); //front
	faces.push_back({ p[3], p[2], p[1] });
	faces.push_back({ p[0], p[4], p[1] }); //top
	faces.push_back({ p[4], p[5], p[1] });
	faces.push_back({ p[2], p[3], p[6] }); //bottom
	faces.push_back({ p[6], p[7], p[3] });
	faces.push_back({ p[4], p[6], p[2] }); //left side
	faces.push_back({ p[0], p[2], p[4] });
	faces.push_back({ p[7], p[1], p[3] }); //right side
	faces.push_back({ p[1], p[5], p[7] }); 
	faces.push_back({ p[5], p[4], p[7] }); //back 
	faces.push_back({ p[7], p[6], p[4] });

	return this;
}

ModelBuilder* ModelBuilder::createSphere(int resX, int resY)
{
	vertices.clear();
	faces.clear();

	float pi = glm::pi<float>();
	float xIncrement = (pi * 2) / resX;
	float yIncrement = (pi) / (resY - 1);

	vertices.resize(resX * (resY + 1), { {0,0,0}, color } );
	//vtxData* data = new vtxData[];
	float x, y, z;
	for (int i = 0; i < resY + 1; i++)
	{
		for (int j = 0; j < resX; j++) {
			x = sinf(j * xIncrement) * sinf(i * yIncrement);
			y = cosf(i * yIncrement);
			z = cosf(j * xIncrement) * sinf(i * yIncrement);
			vertices[i * resX + j].position = { x/2, y/2, z/2};
		}
	}

	vertex t = { {1,1,1},{1,1,1} };
	faces.resize(2 * resX * resY, { t,t,t });
	//face* faceData = new face[2*newRes*(newRes/2+1)];
	for (int i = 0; i < resY - 1; i++) {
		for (int j = 0; j < resX; j++) {
			int a = i * resX + j;
			int b = i * resX + (j + 1) % resX;
			int c = (i + 1) * resX + j;
			int d = (i + 1) * resX + (j + 1) % resX;
			int index = 2 * (i * (resX)+j);
			vertex va = vertices[a];
			vertex vb = vertices[b];
			vertex vc = vertices[c];
			vertex vd = vertices[d];
			/*if (j == resX - 1) { //for UV in the future
				vb.u = 1.0f;
				vd.u = 1.0f;
			}*/
			faces[index] = { va, vc, vb }; //winding order(?)
			faces[index + 1] = { vb, vc, vd }; //winding order(???)
			//sphereSize += 6;
		}
	}
	return this;
}

ModelBuilder* ModelBuilder::setColor(glm::vec3 color)
{
	this->color = color;
	for (int i = 0; i < faces.size(); i++)
	{
		faces[i].a.color = color;
		faces[i].b.color = color;
		faces[i].c.color = color;
	}
	return this;
}

ModelBuilder* ModelBuilder::setColor(glm::vec3 colorA, glm::vec3 colorB, glm::vec3 colorC)
{
	for (int i = 0; i < faces.size(); i++)
	{
		faces[i].a.color = colorA;
		faces[i].b.color = colorB;
		faces[i].c.color = colorC;
	}
	return this;
}

Model* ModelBuilder::get()
{
	int vertexCount = 3 * faces.size();
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertexCount, faces.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));

	Model* m = new Model(vertexBuffer, vertexCount);
	return m;
}