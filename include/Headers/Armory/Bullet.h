#pragma once
#include "Physics\RigidBody.h"
#include "Graphics\Texture.h"
#include <memory>

class Bullet : public RigidBody
{
	public:
		Bullet(SDL_FRect bulletRect, float degree);
		~Bullet();
		void onCollision(std::shared_ptr<RigidBody> bodyCollidingInto) override;
		float getVelocity();
		float getDmg();
		void setVelocity(float newVelocity);
	private:
		float velocity;
		float dmg;
};