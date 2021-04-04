#include "Tema1.h"

#include <vector>
#include <iostream>
#include <utility>
#include <math.h>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;
float totalTime = 0;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

float this_time = 2.0;
int keys[10] = {0};
float squareSide = 100;
float shurikenSize = 20;
float arrowLength = 65;
float arrowSize = 15;
float radiusBalloon = 10;
float bowRadius = 50;
float angle = 0;
float shurikenCenterX;
float shurikenCenterY;
float my_time = 0.0;
float negative_time = 0.0;
float balloon_time = 0.0;
float shuriken_time = 0.0;
float arrow_speed = 250;
float move_arrow = 0.0;
float xShuriken, yShuriken;
float xBow, yBow;
float heart_time = 5.0;
int balloon_idx = -1;
int shuriken_idx = -1;
int bow_idx = -1;
float scale_factor = 1.0;
void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;


	/* initialize vector of pairs for my points */
	std::vector< std::pair<float, float> >points(10, std::make_pair(0.0, 0.0));

	/* initialize the shuriken center */
	shurikenCenterX = corner.x + shurikenSize;
	shurikenCenterY = corner.y + shurikenSize;
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;
	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;

	Mesh* square = Object2D::CreateSquare("square", corner, squareSide, glm::vec3(1, 0, 1), true);
	Mesh* shuriken = Object2D::CreateShuriken("shuriken", corner, shurikenSize, glm::vec3(1, 0, 0), true);
	Mesh* arrow = Object2D::CreateArrow("arrow", corner, arrowLength, arrowSize, glm::vec3(1, 0, 0), false);
	Mesh* bow = Object2D::CreateBow("bow", corner, bowRadius, glm::vec3(1, 0, 0), false);
	Mesh* balloon = Object2D::CreateBalloon("balloon", glm::vec3(0, 0, 0), radiusBalloon, glm::vec3(1, 0, 0), false);
	Mesh* heart = Object2D::CreateHeart("heart", corner, bowRadius, glm::vec3(1, 0, 0), false);
	Mesh* spark = Object2D::CreateSparks("spark", corner, glm::vec3(1, 0, 0));
	Mesh* bar = Object2D::CreatePowerBar("bar", corner, glm::vec3(1, 1, 0));
	Mesh* greenballoon = Object2D::CreateBalloon("greenballoon", glm::vec3(0, 0, 0), radiusBalloon, glm::vec3(0, 100, 0), false);
	AddMeshToList(square);
	AddMeshToList(shuriken);
	AddMeshToList(arrow);
	AddMeshToList(bow);
	AddMeshToList(balloon);
	AddMeshToList(heart);
	AddMeshToList(spark);
	AddMeshToList(greenballoon);
	AddMeshToList(bar);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

/* functions for rendering objects */
void Tema1::RenderShuriken(float deltaTimeSeconds, float x, float y) {
	/* Render mesh for a shuriken */
	float rotateSpeed = 2.5;
	float shurikenSpeed = 70.0;
	points[1] = { shurikenCenterY + 900 + (-deltaTimeSeconds * shurikenSpeed), shurikenCenterY + 500 };
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(points[1].first, points[1].second);
	modelMatrix *= Transform2D::Rotate(deltaTimeSeconds * rotateSpeed);
	modelMatrix *= Transform2D::Translate(-shurikenCenterY - 900, -shurikenCenterY - 500);
	modelMatrix *= Transform2D::Translate(900, 500);
	RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
}
void Tema1::RenderBow(float delta, std::pair<float, float>points[]) {
	/* Render mesh for the bow */
	points[0] = { 100, translateY };
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(points[0].first, points[0].second);
	modelMatrix *= Transform2D::Rotate(angle);
	modelMatrix *= Transform2D::Translate(-bowRadius, -bowRadius);
	modelMatrix *= Transform2D::Translate(bowRadius, bowRadius);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
}
void Tema1::RenderArrow(float delta, std::pair<float, float>points[], int flag) {
	/* Render mesh for an arrow */
	if (flag) arrow_speed = 250;
	points[19] = { points[0].first + move_arrow * arrow_speed, points[0].second };
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(points[19].first, points[19].second);
	modelMatrix *= Transform2D::Rotate(angle);
	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
}
void Tema1::RenderBalloons(float delta, std::pair<float, float> points[], int idx) {
	/* Respawn balloons every balloon_time */
	for (int i = 0, j = 6; i < 100; i += 15, j++) {
		/* check if a ballon was hitten */
		if (idx != j) {
			/* if wasnt render it as usual, even postion green */
			if (j % 2) {
				points[j] = { 1200.0, i * 12 + balloon_time * 70 };
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(points[j].first, points[j].second);
				modelMatrix *= Transform2D::Scale(scaleX, 1.5f);
				RenderMesh2D(meshes["balloon"], shaders["VertexColor"], modelMatrix);
			}
			/* render odd positions red balloons */
			else {
				points[j] = { 1200.0, i * 12 + balloon_time * 70 };
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(points[j].first, points[j].second);
				modelMatrix *= Transform2D::Scale(scaleX, 1.5f);
				RenderMesh2D(meshes["greenballoon"], shaders["VertexColor"], modelMatrix);
			}
		}
		/* if a balloons was hitten */
		else {
			/* in event position scale green balloons */
			if (j % 2) {
				points[j] = { 1200.0, i * 12 + balloon_time * 70 };
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(points[j].first, points[j].second);
				modelMatrix *= Transform2D::Scale(scaleX, 0.15f);
				modelMatrix *= Transform2D::Translate(-points[j].first, -points[j].second);
				modelMatrix *= Transform2D::Translate(points[j].first, points[j].second);
				modelMatrix *= Transform2D::Scale(scaleX, 1.5f);
				RenderMesh2D(meshes["balloon"], shaders["VertexColor"], modelMatrix);
			}
			/* in odd positions scale red balloons */
			else {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(points[j].first, points[j].second);
				modelMatrix *= Transform2D::Scale(scaleX, 0.15f);
				modelMatrix *= Transform2D::Translate(-points[j].first, -points[j].second);
				modelMatrix *= Transform2D::Translate(points[j].first, points[j].second);
				modelMatrix *= Transform2D::Scale(scaleX, 1.5f);
				RenderMesh2D(meshes["balloon"], shaders["VertexColor"], modelMatrix);
			}
		}
		/* first column of balloons */
		if (idx != 6 + j) {
			/* dont scale and render simple green balloons */
			points[6 + j] = { 1000.0, i * 12 + balloon_time * 70 };
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(points[j + 6].first, points[j + 6].second);
			modelMatrix *= Transform2D::Scale(scaleX, 1.5f);
			RenderMesh2D(meshes["greenballoon"], shaders["VertexColor"], modelMatrix);
		}
		else {
			/* scale all green ballons */
			points[6 + j] = { 1000.0, i * 12 + balloon_time * 70 };
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(points[j + 6].first, points[j + 6].second);
			modelMatrix *= Transform2D::Scale(scaleX, 0.15f);
			modelMatrix *= Transform2D::Translate(-points[j + 6].first, -points[j + 6].second);
			modelMatrix *= Transform2D::Translate(points[j + 6].first, points[j + 6].second);
			modelMatrix *= Transform2D::Scale(scaleX, 1.5f);
			RenderMesh2D(meshes["greenballoon"], shaders["VertexColor"], modelMatrix);
		}
	}
}
void Tema1::RenderShurikens(float  delta, std::pair<float, float>points[], int idx) {
	/* Respawn shuriken every shuriken_time */
	for (int i = 0; i < 100; i += 15) {
		/* shuriken position points[2] */
		if (2 != idx) {
			points[2] = { shurikenCenterY + 850 + (-shuriken_time * 77), shurikenCenterY + 600 };
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(points[2].first, points[2].second);
			modelMatrix *= Transform2D::Rotate(my_time * 2.5);
			modelMatrix *= Transform2D::Translate(-shurikenCenterY - 850, -shurikenCenterY - 600);
			modelMatrix *= Transform2D::Translate(850, 600);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
		}
		/* shuriken position points[3] */
		if (idx != 3) {
			points[3] = { shurikenCenterY + 850 + (-shuriken_time * 100), shurikenCenterY + 450 };
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(points[3].first, points[3].second);
			modelMatrix *= Transform2D::Rotate(my_time * 2.5);
			modelMatrix *= Transform2D::Translate(-shurikenCenterY - 850, -shurikenCenterY - 450);
			modelMatrix *= Transform2D::Translate(850, 450);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
		}
		/* shuriken position points[4] */
		if (idx != 4) {
			points[4] = { shurikenCenterY + 850 + (-shuriken_time * 77), shurikenCenterY + 300 };
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(points[4].first, points[4].second);
			modelMatrix *= Transform2D::Rotate(my_time * 2.5);
			modelMatrix *= Transform2D::Translate(-shurikenCenterY - 850, -shurikenCenterY - 300);
			modelMatrix *= Transform2D::Translate(850, 300);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
		}
		/* shuriken position points[5] */
		if (idx != 5) {
			points[5] = { shurikenCenterY + 850 + (-shuriken_time * 100), shurikenCenterY + 120 };
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(points[5].first, points[5].second);
			modelMatrix *= Transform2D::Rotate(my_time * 2.5);
			modelMatrix *= Transform2D::Translate(-shurikenCenterY - 850, -shurikenCenterY - 120);
			modelMatrix *= Transform2D::Translate(850, 120);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
		}
	}
}
void Tema1::RenderHeart() {
	for (int i = 0; i < playerLifes; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(20 + i * 40, 700);
		RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}
}
void Tema1::RenderSpark(float x, float y) {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(x, y);
	RenderMesh2D(meshes["spark"], shaders["VertexColor"], modelMatrix);
}
void Tema1::RenderBar(float x, float y, float scale_factor) {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(x, y - 75);
	modelMatrix *= Transform2D::Scale(scale_factor, 1.0);
	modelMatrix *= Transform2D::Translate(-x-35, -y-5);
	modelMatrix *= Transform2D::Translate(x, y);
	RenderMesh2D(meshes["bar"], shaders["VertexColor"], modelMatrix);
}
/* functions for computing collisions */
bool Tema1::ShurikenvsBow(float &xShuriken, float &yShuriken, int &object, float xBow, float yBow) {
	/* shurikens positions in vector of points is [2-5] */
	float radiusBow = 80.0;
	float radiusShiruken = 20.0;

	/* Compute the distance between points of each shuriken and the bow */
	for (int i = 2; i < 6; i++) {
		float dx = points[i].first - points[0].first;
		float dy = points[i].second - points[0].second;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < radiusBow + radiusShiruken) {
			//cout << "Collision detected in shuriken vs bow " << i << endl;
			xShuriken = points[i].first;
			yShuriken = points[i].second;
			object = i;
			return true;
		}
	}
	return false;
}
bool Tema1::ArrowvsBallon(float xBalloon, float yBalloon, float xArrow, float yArrow, int &idx) {
	/* balloons on positions [6-12] */
	float arrowWidth = 80;
	float arrowHeight = 15;
	float balloonWidth = 12;
	float balloonHeight = 10;
	for (int i = 6; i <= 18; i++) {
		if (points[i].first < xArrow + arrowWidth && points[i].first + balloonWidth > xArrow &&
			points[i].second < yArrow + arrowHeight && points[i].second + balloonHeight > yArrow) {
			idx = i;
			return true;
		}
	}
	return false;
}
bool Tema1::ArrowvsShuriken(float xArrow, float yArrow) {
	float radiusArrow = 20.0;
	float radiusShiruken = 25.0;

	/* Compute the distance between points of each shuriken and the arrow */
	for (int i = 2; i < 6; i++) {
		float dx = points[i].first - points[19].first;
		float dy = points[i].second - points[19].second;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < radiusArrow + radiusShiruken) {
			//cout << "Collision detected in shuriken vs arrow " << i << endl;
			return true;
		}
	}
	return false;
}
/* update function */
int flagg = 0;
float arrow_time = 0.0;
float bar_time = 0.0;
void Tema1::Update(float deltaTimeSeconds)
{
	my_time += deltaTimeSeconds * 1;
	heart_time -= deltaTimeSeconds * 1;
	this_time -= deltaTimeSeconds * 1;
	arrow_time += deltaTimeSeconds * 1;
	balloon_time += deltaTimeSeconds;
	shuriken_time += deltaTimeSeconds;
	/* render objects */
	RenderBow(my_time, points);
	RenderArrow(my_time, points, flagg);
	RenderHeart();
	/* check if an arrrow hits in shuriken */
	if (ArrowvsShuriken(points[19].first, points[19].second)) {
		flagg = 1;
		RenderArrow(my_time, points, flagg);
		arrow_time = -10.0;
		playerScore += 10;
		cout << " Your score is:" << playerScore << endl;
	}
	/* check if a shuriken collided with the bow, decrement player's lives */
	if (ShurikenvsBow(xShuriken, yShuriken, bow_idx, points[0].first, points[0].second) && heart_time < 0) {
		playerLifes--;
		heart_time = 2.5;
		RenderSpark(xShuriken, yShuriken);
	}
	/* check if an arrow hits the balloon */
	if (ArrowvsBallon(points[6].first, points[6].second, points[19].first, points[19].second, balloon_idx) && this_time < 0) {
		flagg = 1;
		RenderArrow(my_time, points, flagg);
		arrow_time = -10.0;
		if (balloon_idx > 12 || balloon_idx % 2) {
			playerScore += 10;
		}
		else {
			playerScore -= 10;
		}
		this_time = 2.5;
		cout << " Your score is:" << playerScore << endl;
	}
	/* render some objects */
	RenderBalloons(my_time, points, balloon_idx);
	RenderShurikens(my_time, points, shuriken_idx);
	RenderBar(points[0].first, points[0].second, bar_time);
	/* if arrow  was hitten by a shuriken, respawn the arrow on the bow */
	if (arrow_time == -10.0 || arrow_time >= 5.5) {
		arrow_time = 0.0;
		flagg = 0;
		move_arrow = 0.0;
	}
	/* respawn the balloons every 2.5 delta seconds */
	if (balloon_time > 2.5) {
		balloon_time = 0.0;
		balloon_idx = -1;
	}
	/* respawn shurikens every 12 delta seconds */
	if (shuriken_time >= 11.50) {
		shuriken_time = 0.0;
		bow_idx = -1;
	}
	/* end the game if lives are less than zero */
	if (!playerLifes) {
		exit(1);
	}
	if (bar_time >= 2.5) {
		bar_time = 0.0;
		scale_factor = 1.0;
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// update when W was pressed
	if (keys[3]) {
		angle += deltaTime * 1;
	}
	// update when S was pressed
	if (keys[2]) {
		angle -= deltaTime * 1;
	}
	// update when A was pressed and translate the bow
	if (keys[1]) {
		translateY -= deltaTime * 100;
	}
	if (keys[0]) {
		translateY += deltaTime * 100;
	}
	if (keys[5]) {
		bar_time += deltaTime * 1;
		arrow_speed += deltaTime * 250;
	}
	if (keys[6]) {
		move_arrow += deltaTime * 1;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	// change the angle of the bow
	if (GLFW_KEY_W == key) {
		keys[0] = 1;
	}
	// change the angle of the bow
	if (GLFW_KEY_S == key) {
		keys[1] = 1;
	}
	// change the position of the bow Oy
	if (GLFW_KEY_A == key) {
		keys[2] = 1;
	}
	if (GLFW_KEY_Q == key) {
		keys[3] = 1;
	}
	// press space key to shoot aka release the arrow
	if (GLFW_KEY_SPACE == key) {
		keys[4] = 1;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (GLFW_KEY_W == key) {
		keys[0] = 0;
	}
	if (GLFW_KEY_S == key) {
		keys[1] = 0;
	}
	if (GLFW_KEY_A == key) {
		keys[2] = 0;
	}
	if (GLFW_KEY_Q == key) {
		keys[3] = 0;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	// std::cout << mouseX << " " << mouseY << std::endl;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	/* press left button and increment bar and arrow speed */
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		keys[5] = true;
		keys[6] = false;
	}

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		keys[6] = true;
		keys[5] = false;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
