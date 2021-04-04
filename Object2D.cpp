#include "Object2D.h"
#include <Core/Engine.h>
#include <iostream>
Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}
Mesh* Object2D::CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(0, length, 0), color), // 1
		VertexFormat(corner + glm::vec3(length, length, 0), color), // 2
		VertexFormat(corner + glm::vec3(length, 0, 0), color), // 3
		VertexFormat(corner + glm::vec3(2*length, 0, 0), color), // 4
		VertexFormat(corner + glm::vec3(length, 2*length, 0), color), // 5
		VertexFormat(corner + glm::vec3(0, 2*length, 0), color), // 6
		VertexFormat(corner + glm::vec3(2*length, length, 0), color), // 7
		VertexFormat(corner + glm::vec3(2*length, 2*length, 0), color) // 8

	};

	Mesh* shuriken = new Mesh(name);
	std::vector<unsigned short> indices =
	{
		0, 1, 2,	// indices for first triangle
		2, 4, 3,	// indices for second triangle
		2, 5, 6,			// TODO: Complete indices data
		2, 7, 8,
	};

	if (!fill) {
		shuriken->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		//indices.push_back(2);
	}

	shuriken->InitFromData(vertices, indices);
	return shuriken;
}
Mesh* Object2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, float arrowSize, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, 0, 0), color), // 0
		VertexFormat(corner + glm::vec3(length, 0, 0), color), // 1
		VertexFormat(corner + glm::vec3(length, arrowSize, 0), color), // 2
		VertexFormat(corner + glm::vec3(length, -arrowSize, 0), color), // 3
		VertexFormat(corner + glm::vec3(length + arrowSize, 0, 0), color) // 4
	};

	Mesh* arrow = new Mesh(name);
	std::vector<unsigned short> indices = 
	{
		0, 1,
		2, 3, 4
	};

	arrow->SetDrawMode(GL_LINES);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	arrow->InitFromData(vertices, indices);
	return arrow;
}
Mesh* Object2D::CreateBow(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;
	float r = radius;
	int steps = 360;
	std::vector<VertexFormat> vertices;

	for (int i = 0; i < steps; i++) {
		float angle = i * (3.141592) * 2 / steps;
		int x = r * cos(angle);
		int y = r * sin(angle);
		if ((x >= 0 && y >= 0) || (x >= 0 && y <= 0)) {
				vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
		}
	}
	Mesh* bow = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i <= steps/2; i++) {
		indices.push_back(i);
	}
	indices.push_back(1);
	bow->SetDrawMode(GL_LINE_STRIP);
	bow->InitFromData(vertices, indices);
	return bow;
}
Mesh* Object2D::CreateBalloon(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;
	float r = radius;
	int steps = 20000;
	std::vector<VertexFormat> vertices;


	vertices.push_back(VertexFormat(corner + glm::vec3(0, -radius, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(0, -radius - 10, 0), color));
	for (int i = 0; i < steps; i++) {
		float angle = i * (3.141592) * 2 / steps;
		int x = r * cos(angle);
		int y = r * sin(angle);
		vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
	}

	Mesh* bow = new Mesh(name);
	std::vector<unsigned short> indices;
	indices.push_back(0);
	indices.push_back(1);
	for (int i = 2; i < (steps); i++) {
		indices.push_back(0 * i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}
	bow->SetDrawMode(GL_LINES);
	bow->InitFromData(vertices, indices);
	return bow;
}
Mesh* Object2D::CreateHeart(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;
	float r = radius;
	int steps = 360;
	std::vector<VertexFormat> vertices;

	for (int i = 0; i < steps; i++) {
		float angle = i * (3.141592) * 2 / steps;
		float x = 16 *sin(angle)*sin(angle)*sin(angle);
		float y = 13*cos(angle) - 5*cos(2*angle) - 2*cos(3*angle) - cos(angle);
		vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
	}
	Mesh* heart = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i <= steps; i++) {
		indices.push_back(i);
	}
	indices.push_back(1);
	heart->SetDrawMode(GL_LINE_STRIP);
	heart->InitFromData(vertices, indices);
	return heart;
}
Mesh* Object2D::CreateSparks(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color) {
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, 0, 0), color), // 0
		VertexFormat(corner + glm::vec3(10, 10, 0), color), // 1
		VertexFormat(corner + glm::vec3(40, 0, 0), color), // 2
		VertexFormat(corner + glm::vec3(30, 10, 0), color), // 3

		VertexFormat(corner + glm::vec3(30, 20, 0), color), // 4
		VertexFormat(corner + glm::vec3(40, 30, 0), color), // 5
		VertexFormat(corner + glm::vec3(10, 20, 0), color), // 6
		VertexFormat(corner + glm::vec3(0, 30, 0), color), // 7
	};

	Mesh* spark = new Mesh(name);
	std::vector<unsigned short> indices =
	{
		0, 1,
		2, 3,
		4, 5,
		7, 6
	};

	spark->SetDrawMode(GL_LINES);

	spark->InitFromData(vertices, indices);
	return spark;
}
Mesh* Object2D::CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color) {
	glm::vec3 corner = leftBottomCorner;
	int height = 10;
	int lenght = 70;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, 0, 0), color), // 0
		VertexFormat(corner + glm::vec3(lenght, 0, 0), color), // 1
		VertexFormat(corner + glm::vec3(lenght, height, 0), color), // 2
		VertexFormat(corner + glm::vec3(0, height, 0), color), // 3
	};

	Mesh* bar = new Mesh(name);
	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		1, 3, 2
	};

	bar->SetDrawMode(GL_TRIANGLES);

	bar->InitFromData(vertices, indices);
	return bar;
}