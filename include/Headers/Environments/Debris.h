#pragma once
#include "Graphics\Texture.h"
#include <SDL.h>

class Debris
{
	public:
		// angle should be a line going towards the edge of a circle, there will be an array of Debris and each elem will take care of a line inner -> outer circle
		Debris(int x, int y, float angle);
		~Debris();

		bool isDead();
		void update(float deltaTime);
		void render();
		int getX();
		int getY();
		void updatePosition(int newX, int newY);
		SDL_Color getDebriColor();
	private:
		int mXPos;
		int mYPos;
		float mAngle;
		SDL_Color debriColor;
		int mMaxFrames;
		int mframe;
};