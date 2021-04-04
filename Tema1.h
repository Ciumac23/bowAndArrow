#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void RenderHeart();
		void RenderSpark(float x, float y);
		void RenderBar(float x, float y, float scale_factor);
		void RenderShuriken(float delta, float x, float y);
		void RenderBow(float delta, std::pair<float, float>points[]);
		void RenderArrow(float delta, std::pair<float, float>points[], int idx);
		void RenderBalloons(float delta, std::pair<float, float> points[], int idx);
		void RenderShurikens(float  delta, std::pair<float, float>points[], int idx);
		bool ShurikenvsBow(float &xShuriken, float &yShuriken, int &object, float xBow, float yBow);
		bool ArrowvsBallon(float xBalloon, float yBalloon, float xArrow, float yArrow, int &idx);
		bool ArrowvsShuriken(float xArrow, float yArrow);
	protected:
		glm::mat3 modelMatrix;
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;
		int playerLifes = 10;
		int playerScore = 0;
		std::pair<float, float> points[25];
};

/* Initialize a vector of points for every 
rendered mesh to be places in this vector.

--- [zero] index - points for bow
--- [first] index - points for first shuriken
--- [2-5] index - points for shurikens
--- [6-7] index - points for balloons
--- [8] index - points for arrow
*/