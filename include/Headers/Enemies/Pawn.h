#pragma once
#include "Enemies/Enemy.h"
#include <SDL.h>

class Pawn : public Enemy
{
	public:
		Pawn();
		~Pawn() override;
		void spawn(SDL_Rect currentScreenRect) override;
		void handleStateSpecificUpdate(float deltaTime) override;
		void updateState() override;
	private:
		bool isDoneAttacking = true;
};