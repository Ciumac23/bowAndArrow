#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, float arrowSize, glm::vec3 color, bool fill = false);
	Mesh* CreateBow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateBalloon(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateHeart(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateSparks(std::string name, glm::vec3 leftBottomCorner,glm::vec3 color);
	Mesh* CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);

}

