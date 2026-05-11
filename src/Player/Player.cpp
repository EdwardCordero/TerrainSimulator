#include "Player\Player.h"
#include "Animations\SpriteSheet.h"
#include "Armory\Weapon.h"
#include "Physics\RigidBody.h"
#include "Settings\SettingsManager.h"
#include <iostream>
#include <memory>

Player::Player() : RigidBody()
{
	/* Stats */
	this->stocks = 0;
	this->iFrames = 5;
	this->game = "default";
	this->score = 0;
	this->health = 0;
	this->maxHealth = 1;
	this->isInvin = false;
	/* Movement Stats */
	this->maxVelocity = 0;
	this->acceleration = 0;
	this->currentVelocity = 0;

	/* Direction Input */
	this->movingDown = 0;
	this->movingUp = 0;
	this->movingRight = 0;
	this->movingLeft = 0;
	this->currentXDirection = 0;
	this->currentYDirection = 0;
	this->previousXDirection = 0;
	this->previousYDirection = 0;

	/* For Rotation */
	this->rotateForce = 0;
	this->rotationSpeed = 1;
	this->maxRotationSpeed = 360;

	// Classes
	this->playerSpriteSheet = SpriteSheet();
	this->weapon = Weapon(nullptr);
}

Player::~Player()
{
	std::cout << "Player deleted" << std::endl;
}

void Player::onCollision(std::shared_ptr<RigidBody> bodyCollidingInto)
{
	if (bodyCollidingInto->getBodyType() == EnemyBody)
	{
		std::cout << "Player hit" << std::endl;
		this->health -= 1;
		this->invincible();
	}
}

void Player::update(const Uint8* keyState, float deltaTime, SDL_Rect screenSize)
{
	// player movement
	this->updateAnimation();

	if (this->health <= 0)
	{
		// DEAD
		this->despawnPlayer();
	}

	this->weapon.update(deltaTime, screenSize);
	if (isInvin)
	{
		this->iFrames -= this->graphicsManager->getFrameRate();
		if (this->iFrames > 0)
		{
			this->playerSpriteSheet.getSheetTexture()->fade(this->graphicsManager->getDeltaTime());
		}
		else
		{
			this->setCollidable(true);
			this->isInvin = false;
			this->playerSpriteSheet.getSheetTexture()->endFlicker();
		}
	}
}

void Player::updateAnimation()
{
	if (game == "DEFENDER")
	{
		updateDefenderAnimation();
	}
	if (game == "ASTEROIDS")
	{
		this->playerSpriteSheet.setCurrentSprite(0);
	}
}

void Player::updateDefenderAnimation()
{
	if (this->currentVelocity <= 0)
	{
		this->playerSpriteSheet.setCurrentSprite(0);
	}
	if (this->currentVelocity > 0 && this->currentVelocity < maxVelocity / 2)
	{
		this->playerSpriteSheet.setCurrentSprite(1);
	}
	if (this->currentVelocity > maxVelocity / 2 && this->currentVelocity < maxVelocity)
	{
		this->playerSpriteSheet.setCurrentSprite(2);
	}
	if (this->currentVelocity >= maxVelocity)
	{
		this->playerSpriteSheet.setCurrentSprite(3);
	}
}

void Player::spawnPlayer()
{
	this->health = maxHealth;
	float initalHeight = this->playerSpriteSheet.getSpriteHeight();
	float initalWidth = this->playerSpriteSheet.getSpriteWidth();
	float initialX = this->graphicsManager->getScreenRect().w / 2;
	float initialY = this->graphicsManager->getScreenRect().h / 2;
	this->setRigidBodyRect({ initialX, initialY, initalWidth, initalHeight });
	this->setCollidable(true);
	this->setBodyType(PlayerBody);
	this->setActive(true);
	this->invincible();
	std::cout << std::format("PLAYER SPAWNING AT X: {} Y: {}", this->getXPos(), this->getYPos()) << std::endl;
}

void Player::invincible()
{
	this->iFrames = 5;
	this->iFrames *= this->graphicsManager->getFrameRate();
	this->isInvin = true;
	this->setCollidable(false);
}

void Player::despawnPlayer()
{
	this->stocks--;
	this->setActive(false);
	this->setCollidable(false);
	this->playerSpriteSheet.getSheetTexture()->startFlicker(50);
}

void Player::renderHitBox()
{
	//int presenceBoxOffset = 80;
	//SDL_FRect hitBox = this->getRigidBodyRect();
	//SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
	//SDL_FRect presenceBox = { hitBox.x - presenceBoxOffset , hitBox.y - presenceBoxOffset, hitBox.w + (presenceBoxOffset * 2), hitBox.h + (presenceBoxOffset * 2)};;
	//SDL_RenderDrawRectF(gRenderer, &presenceBox);
}


// Stats & Info
std::string Player::getGame()
{
	return this->game;
}
int Player::getScore()
{
	return this->score;
}
int Player::getHealth()
{
	return this->health;
}
int Player::getMaxHealth()
{
	return this->maxHealth;
}
int Player::getStocks()
{
	return this->stocks;
}
// Movement
float Player::getMaxVelocity()
{
	return this->maxVelocity;
}
float Player::getAcceleration()
{
	return this->acceleration;
}
float Player::getCurrentVelocity()
{
	return this->currentVelocity;
}
float Player::getCurrentXDirection()
{
	return this->currentXDirection;
}
float Player::getCurrentYDirection()
{
	return this->currentYDirection;
}
float Player::getPreviousXDirection()
{
	return this->previousXDirection;
}
float Player::getPreviousYDirection()
{
	return this->previousYDirection;
}
// Rotation
float Player::getRotationForce()
{
	return this->rotateForce;
}
int Player::getRotateSpeed()
{
	return this->rotationSpeed;
}
int Player::getMaxRotationSpeed()
{
	return this->maxRotationSpeed;
}
SpriteSheet Player::getSpriteSheet()
{
	return this->playerSpriteSheet;
}
Weapon* Player::getWeapon()
{
	return &weapon;
}

void Player::setGame(std::string gameName)
{
	this->game = gameName;
}
void Player::setScore(int newScore)
{
	this->score = newScore;
}
void Player::setHealth(int newHealth)
{
	if (newHealth > this->maxHealth)
	{
		newHealth = maxHealth;
	}
	else
	{
		this->health = newHealth;
	}
}

void Player::setStocks(int stocks)
{
	this->stocks = stocks;
}

void Player::setSettingsManager(std::shared_ptr<SettingsManager> settingsManager)
{
	this->settingsManager = settingsManager;
}

void Player::setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager)
{
	this->graphicsManager = graphicsManager;
}

void Player::setMaxVelocity(float newMaxVelocity)
{
	this->maxVelocity = newMaxVelocity;
}
void Player::setAcceleration(float newAcceleration)
{
	this->acceleration = newAcceleration;
}
void Player::setCurrentVelocity(float newVelocity)
{
	if (newVelocity >= 0 && newVelocity < maxVelocity)
	{
		this->currentVelocity = newVelocity;
		//std::cout << std::format("New Velocity within range {}", newVelocity) << std::endl;
	}
	else if (newVelocity < 0)
	{
		this->currentVelocity = 0;
		//std::cout << std::format("New Velocity less than 0 {}", newVelocity) << std::endl;
	}
	else
	{
		this->currentVelocity = maxVelocity;
		//std::cout << std::format("New Velocity over max {}", newVelocity) << std::endl;
	}
}
void Player::setCurrentXDirection(float newXDirection)
{
	this->previousXDirection = this->currentXDirection;
	this->currentXDirection = newXDirection;
}
void Player::setCurrentYDirection(float newYDirection)
{
	this->previousYDirection = this->currentYDirection;
	this->currentYDirection = newYDirection;
}
void Player::setRotationForce(float newRotationForce)
{
	this->rotateForce = newRotationForce;
}
void Player::setRotateSpeed(int newRotationSpeed)
{
	if (newRotationSpeed > this->maxRotationSpeed)
	{
		this->rotationSpeed = this->maxRotationSpeed;
	}
	else
	{
		this->rotationSpeed = newRotationSpeed;
	}
}
void Player::setMaxRotation(int newMaxRotation)
{
	this->maxRotationSpeed = newMaxRotation;
}
void Player::setSpriteSheet(std::string imgPath, int spriteSheetWidth, int spriteSheetHeight, int totalSpriteCount)
{
	//SpriteSheet spriteSheet = SpriteSheet(imgPath, spriteSheetWidth, spriteSheetHeight, totalSpriteCount);
	this->playerSpriteSheet = SpriteSheet(imgPath, spriteSheetWidth, spriteSheetHeight, totalSpriteCount);
}