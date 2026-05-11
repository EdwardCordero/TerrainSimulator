#include "Environments\Debris.h"
#include "globalConstants.h"

Debris::Debris(int x, int y, float angle)
{
	mXPos = x;
	mYPos = y;
	mAngle = angle;
	mframe = 0;
	mMaxFrames = 50;
	debriColor = { 255, 255, 255, 255 };
	//mDebrisTexture = Texture();
}

Debris::~Debris(){}

bool Debris::isDead()
{
	return mframe > mMaxFrames;
}

void Debris::update(float deltaTime)
{
	mXPos += 500 * cos(mAngle) * deltaTime;
	mYPos += 500 * sin(mAngle) * deltaTime;
}

void Debris::render()
{
	//this->mDebrisTexture.render(mXPos, mYPos);

	/*SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
	SDL_RenderDrawPoint(gRenderer, mXPos, mYPos);*/
	mframe += 1;
}

int Debris::getX()
{
	return this->mXPos;
}
int Debris::getY()
{
	return this->mYPos;
}

void Debris::updatePosition(int newX, int newY)
{
	this->mXPos = newX;
	this->mYPos = newY;
}

SDL_Color Debris::getDebriColor()
{
	return this->debriColor;
}