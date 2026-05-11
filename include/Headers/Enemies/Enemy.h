#pragma once
#include "Graphics\Texture.h"
#include "Physics\RigidBody.h"
#include <SDL.h>
#include <vector>
#include <array>
#include <string>

enum EnemyState {
	Attacking,
	Roaming
};

class Enemy : public RigidBody
{
	public:
		/* Initalization */
		Enemy();
		void initializeValues();
		void InitalizeSate();

		/* Frame Functions */
		virtual ~Enemy() = 0;
		void update(SDL_FRect playerRect, float deltaTime);
		virtual void spawn(SDL_Rect currentScreenRect) = 0;
		virtual void handleStateSpecificUpdate(float deltaTime) = 0;
		virtual void updateState() = 0;

		/* Render */
		void renderHitBox();
		void despawn();

		/* Helper Functions I Guess...*/
		void takeDmg(int dmg); 
		bool isEnemyNearPlayer();
		void scaleGameObject(int scaler);

		/* Getters */
		int getHealth();
		float getSpeed();
		int getPoints();
		int getDmg();
		SDL_Color getColor();
		SDL_FRect& getPlayerRect();
		std::shared_ptr<Texture> getTexture();

		/* For Movement Between Points */
		std::array<float,2> getStartPos();
		std::array<float,2> getDestination();
		std::array<std::array<float, 2>, 4> getTargetCorners();

		/* Movement Variables */
		float getMovementProgress();
		int getAnchorX();
		int getAnchorY();
		int getDestinationCounter();
		EnemyState getMovementState();

		/* Setters */
		void setHealth(int newHealth);
		void setSpeed(float newSpeed);
		void setDmg(int newDmg);
		void setPlayerRect(SDL_FRect playerRect);
		void setScreenBorders(SDL_Rect currentScreenSize);

		/* Visuals */
		void setColor(SDL_Color newColor);
		void setTexture(std::shared_ptr<Texture> texture);

		/* For Movement Between Points*/
		void setStartPos(float x, float y);
		void setDestination(float x, float y);
		void setTargetCorners(std::array<std::array<float, 2>, 4> newTargetCorners);
		std::array<std::array<float, 2>, 4> initializeTargetCorners(SDL_FRect newTarget, bool offsetByRect);

		/* Movement Variables */
		void setMovementProgess(float newMovementProgress);
		void setDestinationCounter(int newCounter);
		void setMovementState(EnemyState newState);

	protected:
		/* Stats */
		int health;
		float speed;
		int points;
		int dmg;
		int scale;

		std::shared_ptr<Texture> enemyTexture;
		EnemyState movementState = Attacking;
		SDL_Color color{ 255, 0, 0, 255 };

		/* For movement */
		std::array<float, 2> startPos;
		std::array<float, 2> destination;
		std::array<std::array<float, 2>, 4> targetCorners;
		float movementProgress;
		int destinationCounter;
		/* used for trig movement*/
		float anchorX;
		float anchorY;
		// For rotation
		int rotationSpeed;
		int roationDirection;
		int maxRotationSpeed;

		// For player info
		SDL_FRect playerRect;

		// For screen info to use on spawn
		SDL_Rect currentScreenSize;
};