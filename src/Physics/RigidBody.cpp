#include "Physics\RigidBody.h"
#include "globalConstants.h"
#include <SDL.h>
#include <string>
#include <format>
#include <memory>
#include <iostream>

RigidBody::RigidBody()
{
	this->bodyRect = { 0, 0, 0, 0 };
	this->previousBodyRect = { 0, 0, 0, 0 };
	this->rotateDeg = 0;
	this->collidable = false;
	this->active = false;
	this->bodyType = StaticBody;
	this->bodyName = "NoName";
}

RigidBody::~RigidBody()
{
	
}

void RigidBody::onCollision(std::shared_ptr<RigidBody> bodyCollidingInto)
{
	std::cout << std::format("{} is colliding with {}", this->bodyName, bodyCollidingInto->getBodyTypeStr()) << std::endl;
}

RigidBodyType RigidBody::getBodyType()
{
	return this->bodyType;
}

std::string RigidBody::getBodyTypeStr()
{
	switch (this->bodyType)
	{
	case(PlayerBody):
		return "Player Body";
	case(EnemyBody):
		return "Enemy Body";
	case(BulletBody):
		return "Bullet Body";
	case(HazardBody):
		return "Hazard Body";
	case(StaticBody):
		return "Static Body";
	default:
		break;
	}
}

float RigidBody::getXPos()
{
	return this->bodyRect.x;
}

float RigidBody::getYPos()
{
	return this->bodyRect.y;
}

float RigidBody::getPreviousXPos()
{
	return this->previousBodyRect.x;
}

float RigidBody::getPreviousYPos()
{
	return this->previousBodyRect.y;
}
float RigidBody::getRotateDeg()
{
	return this->rotateDeg;
}
float RigidBody::getWidth()
{
	return this->bodyRect.w;
}
float RigidBody::getHeight()
{
	return this->bodyRect.h;
}

std::string RigidBody::getBodyName()
{
	return this->bodyName;
}

SDL_FRect RigidBody::getRigidBodyRect()
{
	return this->bodyRect;
}

SDL_FRect RigidBody::getPreviousBodyRect()
{
	return this->previousBodyRect;
}

void RigidBody::setCollidable(bool isCollidable)
{
	this->collidable = isCollidable;
}

void RigidBody::setBodyType(RigidBodyType type)
{
	this->bodyType = type;
}

void RigidBody::setXPos(float newX)
{
	this->previousBodyRect.x = this->bodyRect.x;
	this->bodyRect.x = newX;
}

void RigidBody::setYPos(float newY)
{
	this->previousBodyRect.y = this->bodyRect.y;
	this->bodyRect.y = newY;
}

void RigidBody::setWidth(float newWidth)
{
	this->previousBodyRect.w = this->bodyRect.w;
	this->bodyRect.w = newWidth;
}

void RigidBody::setHeight(float newHeight)
{
	this->previousBodyRect.h = this->bodyRect.h;
	this->bodyRect.h = newHeight;
}

void RigidBody::scaleUpRigidBody(int scaler)
{
	this->bodyRect.w += scaler;
	this->bodyRect.h *= scaler;
}

void RigidBody::setRotateDeg(float newRotateDeg)
{
	this->rotateDeg = newRotateDeg;

	if (this->rotateDeg > 360)
	{
		this->rotateDeg = this->rotateDeg - 360;
	}
	if (this->rotateDeg < 0)
	{
		this->rotateDeg = 360 - this->rotateDeg;
	}
}

void RigidBody::setRigidBodyRect(SDL_FRect rect)
{
	this->previousBodyRect = this->bodyRect;
	this->bodyRect = rect;
}

void RigidBody::setBodyName(std::string name)
{
	this->bodyName = name;
}


void RigidBody::setActive(bool isActive)
{
	this->active = isActive;
}

bool RigidBody::hasMoved()
{
	return (this->bodyRect.x != this->previousBodyRect.x || this->bodyRect.y != this->previousBodyRect.y);
}

bool RigidBody::isActive()
{
	return this->active;
}

bool RigidBody::isCollidable()
{
	return this->collidable;
}

bool RigidBody::isValid()
{
	/*if (this->bodyRect.x + this->bodyRect.w > SCREEN_WIDTH)
	{
		return false;
	}
	if (this->bodyRect.x < SCREEN_WIDTH - SCREEN_WIDTH)
	{
		return false;
	}

	if (this->bodyRect.y + this->bodyRect.h > SCREEN_HEIGHT)
	{
		return false;
	}
	if (this->bodyRect.y < SCREEN_HEIGHT - SCREEN_HEIGHT)
	{
		return false;
	}*/

	return	true;
}