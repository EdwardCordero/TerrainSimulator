#pragma once
#include "Graphics\Texture.h"
#include "Armory\Bullet.h"
#include "Armory\Weapon.h"
#include "Animations\SpriteSheet.h"
#include "Physics\RigidBody.h"
#include "Settings\SettingsManager.h"
#include <string>
#include <vector>
#include <memory>

class Player : public RigidBody
{
	public:
		Player();
		~Player();
		/* Intializing */
		void spawnPlayer();
		void despawnPlayer();
		/* During Update */
		void update(const Uint8* keyState, float deltaTime, SDL_Rect screenSize);
		// TOneverDo: Merge both "handle functions" keyboard sets direction vars and playershomovement updates positions
		//void handleKeyboardInputNew(const Uint8* keyState);
		void onCollision(std::shared_ptr<RigidBody> bodyCollidingInto);

		/* Render Logic */
		void renderHitBox();

		/* Animation */
		void updateAnimation();
		void updateDefenderAnimation();

		/* Getters */
		// Stats & Info
		std::string getGame();
		int getScore();
		int getHealth();
		int getMaxHealth();
		int getStocks();
		// Movement
		float getMaxVelocity();
		float getAcceleration();
		float getCurrentVelocity();
		/*float getYVelocity();
		float getXVelocity();*/
		float getCurrentXDirection();
		float getCurrentYDirection();
		float getPreviousXDirection();
		float getPreviousYDirection();
		// Rotation
		float getRotationForce();
		int getRotateSpeed();
		int getMaxRotationSpeed();
		// Visuals
		SpriteSheet getSpriteSheet();
		// Weapons
		Weapon* getWeapon();

		/* Setters */
		// Stats & Info
		void setGame(std::string gameName);
		void setScore(int newScore);
		void setHealth(int newHealth);
		void setStocks(int stocks);
		void setSettingsManager(std::shared_ptr<SettingsManager> settingsManager);
		void setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager);
		// Movement
		void setMaxVelocity(float newMaxVelocity);
		void setAcceleration(float newAcceleration);
		void setCurrentVelocity(float newVelocity);
		/*void updateYVelocity(int newYDirection);
		void updateXVelocity(int newXDirection);*/
		void setCurrentXDirection(float newXDirection);
		void setCurrentYDirection(float newYDirection);
		// Rotation
		void setRotationForce(float newRotationForce);
		void setRotateSpeed(int newRotationSpeed);
		void setMaxRotation(int newMaxRotation);
		// Visuals
		void setSpriteSheet(std::string imgPath, int spriteSheetWidth, int spriteSheetHeight, int totalSpriteCount);
		void invincible();
	protected:
		/* Player Visuals And Position */
		SpriteSheet playerSpriteSheet;
		std::string game;
		int score;

		/* PlayerStats */
		int health;
		int maxHealth;
		float maxVelocity;
		float acceleration;
		int stocks;
		int iFrames;
		bool isInvin;
		/* Weapons */
		Weapon weapon;

		/* Movement */
		float currentVelocity;

		/* Rotation */
		float rotateForce;
		int rotationSpeed;
		int maxRotationSpeed;

		/* Input */
		int movingUp;
		int movingLeft;
		int movingRight;
		int movingDown;
		float currentXDirection;
		float currentYDirection;
		float previousXDirection;
		float previousYDirection;
		bool hMoving = false;
		bool vMoving = false;

		std::shared_ptr<SettingsManager> settingsManager;
		std::shared_ptr<GraphicsManager> graphicsManager;
};