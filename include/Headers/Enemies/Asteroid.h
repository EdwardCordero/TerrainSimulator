#pragma once
#include "Enemies\Enemy.h"
#include "Physics\RigidBody.h"
#include <SDL.h>
#include <memory>

class Asteroid : public Enemy
{
	public:
		Asteroid();
		~Asteroid() override;
		void onCollision(std::shared_ptr<RigidBody> bodyCollidingInto) override;
		void setSpawnLocation(float x, float y, int size);
		void spawn(SDL_Rect currentScreenRect) override;
		void handleStateSpecificUpdate(float deltaTime) override;
		void updateState() override;
	private:
		bool isDoneAttacking = true;
		int size;
};