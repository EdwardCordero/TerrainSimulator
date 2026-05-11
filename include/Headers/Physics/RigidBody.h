#pragma once
#include <SDL.h>
#include <string>
#include <memory>

enum RigidBodyType {
	PlayerBody, EnemyBody, BulletBody, HazardBody, StaticBody, NullBody
};

class RigidBody
{
	public:
		RigidBody();
		virtual ~RigidBody() = 0;
		/* Virtual */
		void virtual onCollision(std::shared_ptr<RigidBody> bodyCollidingInto);
		/* Status */
		bool hasMoved();
		bool isActive();
		bool isValid();
		bool isCollidable();
		/* GETTERS */
		// Position
		float getXPos();
		float getYPos();
		float getPreviousXPos();
		float getPreviousYPos();
		float getRotateDeg();
		float getWidth();
		float getHeight();
		SDL_FRect getRigidBodyRect();
		SDL_FRect getPreviousBodyRect();

		// Body Info
		std::string getBodyName();
		RigidBodyType getBodyType();
		std::string getBodyTypeStr();

		/* SETTERS */
		void setXPos(float newX);
		void setYPos(float newY);
		void setWidth(float newWidth);
		void setHeight(float newHeight);
		void scaleUpRigidBody(int scaler);
		void setRotateDeg(float newRotateDeg);
		void setCollidable(bool isCollidable);
		void setBodyType(RigidBodyType type);
		void setRigidBodyRect(SDL_FRect rect);
		void setBodyName(std::string name);
		void setActive(bool isActive);

	private:
		bool active;
		bool collidable;
		float rotateDeg;

		SDL_FRect bodyRect;
		SDL_FRect previousBodyRect;
		RigidBodyType bodyType;
		std::string bodyName;
};