#include "Enemies\Enemy.h"
#include "Graphics\Texture.h"
#include "GameLogic\HelperFunctions.h"
#include "globalConstants.h"

Enemy::Enemy() : RigidBody()
{
	//this->initializeValues();
	//this->spawn();
	this->setBodyType(EnemyBody);
	this->setBodyName("Asteroid");
	this->anchorX = 0;
	this->anchorY = 0;
	this->destinationCounter = 0;
	this->health = 1;
	this->speed = 100;
	this->points = 0;
	this->dmg = 1;
}

Enemy::~Enemy()
{
	this->despawn();
}

void Enemy::initializeValues()
{
	this->setActive(true);
	this->setCollidable(true);
	this->setBodyType(EnemyBody);
	// Health
	this->health = 1;
	this->speed = 100;
	this->points = 0;
	this->dmg = 1;

	int enemyWidth = enemyTexture->getWidth();
	int enemyHeight = enemyTexture->getHeight();
	int xOffset = (enemyWidth / 2);
	int yOffset = (enemyHeight / 2);
	float initialX = randNumBetween(this->currentScreenSize.x + xOffset, (this->currentScreenSize.x + this->currentScreenSize.w) - xOffset);
	float initialY = randNumBetween(this->currentScreenSize.y + yOffset, (this->currentScreenSize.y + this->currentScreenSize.h) - yOffset);
	SDL_FRect hitBox = { initialX, initialY, enemyWidth, enemyHeight };
	this->setRigidBodyRect(hitBox);

	this->setMovementState(Roaming);
	this->InitalizeSate(); 
}

void Enemy::InitalizeSate()
{
	this->anchorX = this->getXPos();
	this->anchorY = this->getYPos();
	this->startPos[0] = this->getXPos();
	this->startPos[1] = this->getYPos();
	this->movementProgress = 0;
	this->roationDirection = 1;
	this->setRotateDeg(0);
	if (this->movementState == Attacking)
	{
		this->destination = { this->getXPos(), this->getYPos() + 80};
	}
	if (this->movementState == Roaming)
	{
		this->destination = { this->getXPos() + 100, this->getYPos()};
	}
}

void Enemy::spawn(SDL_Rect currentScreenRect)
{
	this->setScreenBorders(currentScreenRect);
	this->initializeValues();
	//SDL_RenderFillRect(gRenderer, &hitBox);
	std::cout << std::format("Enemy spawning at X: {} & Y: {}", this->getXPos(), this->getYPos()) << std::endl;
}

void Enemy::update(SDL_FRect playerCurrentPos, float deltaTime)
{
	this->setPlayerRect(playerRect);
	this->handleStateSpecificUpdate(deltaTime);
	this->updateState();
}

void Enemy::takeDmg(int dmg)
{
	this->health -= dmg;
	if (this->health <= 0)
	{
		despawn();
	}
}

//TODO: Fix this for new Graphics Manager
void Enemy::scaleGameObject(int scaler)
{
	this->scale = scaler;
	this->setWidth(this->enemyTexture->getWidth() * scaler);
	this->setHeight(this->enemyTexture->getHeight() * scaler);
}

void Enemy::renderHitBox()
{
	/*SDL_SetRenderDrawColor(gRenderer, 227, 84, 115, 5);
	SDL_FRect hitBox = this->getRigidBodyRect();
	SDL_RenderDrawRectF(gRenderer, &hitBox);*/
}

void Enemy::despawn()
{
	this->setActive(false);
}

bool Enemy::isEnemyNearPlayer()
{
	int presenceBoxOffset = 80;
	SDL_FRect hitBox = this->getRigidBodyRect();
	SDL_FRect lineOfSight = { hitBox.x - presenceBoxOffset , hitBox.y - presenceBoxOffset, hitBox.w + (presenceBoxOffset * 2), hitBox.h + (presenceBoxOffset * 2) };

	return checkCollision(lineOfSight, this->playerRect);
}

#pragma region Getters & Setters
int Enemy::getHealth()
{
	return this->health;
}
std::array<float, 2> Enemy::getStartPos()
{
	return this->startPos;
}
std::array<float, 2> Enemy::getDestination()
{
	return this->destination;
}
float Enemy::getSpeed()
{
	return this->speed;
}
int Enemy::getPoints()
{
	return this->points;
}
int Enemy::getDmg()
{
	return this->dmg;
}
float Enemy::getMovementProgress()
{
	return this->movementProgress;
}
SDL_Color Enemy::getColor()
{
	return this->color;
}
int Enemy::getAnchorX()
{
	return this->anchorX;
}
int Enemy::getAnchorY()
{
	return this->anchorY;
}
int Enemy::getDestinationCounter()
{
	return this->destinationCounter;
}
EnemyState Enemy::getMovementState()
{
	return this->movementState;
}
std::array<std::array<float, 2>, 4> Enemy::getTargetCorners()
{
	return this->targetCorners;
}
SDL_FRect& Enemy::getPlayerRect()
{
	return this->playerRect;
}

std::shared_ptr<Texture> Enemy::getTexture()
{
	return this->enemyTexture;
}

// Setters
void Enemy::setHealth(int newHealth)
{
	this->health = newHealth;
}
void Enemy::setStartPos(float x, float y)
{
	this->startPos = { x, y };
}
void Enemy::setDestination(float x, float y)
{
	this->destination = { x, y };
}
void Enemy::setSpeed(float newSpeed)
{
	this->speed = newSpeed;
}
void Enemy::setDmg(int newDmg)
{
	this->dmg = newDmg;
}
void Enemy::setMovementProgess(float newMovementProgress)
{
	this->movementProgress = newMovementProgress;
}
void Enemy::setColor(SDL_Color newColor)
{
	this->color = newColor;
}
void Enemy::setTexture(std::shared_ptr<Texture> texture)
{
	this->enemyTexture = texture;
}
void Enemy::setMovementState(EnemyState newState)
{
	this->movementState = newState;
}
void Enemy::setDestinationCounter(int newCounter)
{
	this->destinationCounter = newCounter;
}
void Enemy::setTargetCorners(std::array<std::array<float, 2>, 4> newTargetCorners)
{
	this->targetCorners = newTargetCorners;
}
std::array<std::array<float, 2>, 4> Enemy::initializeTargetCorners(SDL_FRect newTarget, bool offsetByRect)
{
	std::array<std::array<float, 2>, 4> targetCorners = getCornersOfRect(newTarget);
	
	// adjust corners b/c I want the enemy to follow the target from a distance not be directly ontop of it.
	int followingDistance = 20;
	targetCorners[0] = { targetCorners[0][0] - followingDistance, targetCorners[0][1] - followingDistance };
	targetCorners[1] = { targetCorners[1][0] + followingDistance, targetCorners[1][1] - followingDistance };
	targetCorners[2] = { targetCorners[2][0] + followingDistance, targetCorners[2][1] + followingDistance };
	targetCorners[3] = { targetCorners[3][0] - followingDistance, targetCorners[3][1] + followingDistance };
	

	// This is shifting the corners to ensure that the enemy moves around target not within it. ex: target 0 x needs to be less b/c otherwise enemy would move within presence box
	if (offsetByRect)
	{
		int offset = 80;
		SDL_FRect hitBox = this->getRigidBodyRect();
		targetCorners[0][0] -= offset - hitBox.x;
		targetCorners[1][1] -= offset - hitBox.y;
		targetCorners[2][0] += offset - hitBox.x;
		targetCorners[3][1] += offset - hitBox.y;
	}
	// Ensure that target corners are within limits
	for (int i = 0; i < targetCorners.size(); i++)
	{
		if (targetCorners[i][0] > this->currentScreenSize.x + this->currentScreenSize.w)
		{
			targetCorners[i][0] = (this->currentScreenSize.x + this->currentScreenSize.w) - this->getWidth() - 1;
		}
		if (targetCorners[i][0] < this->currentScreenSize.x)
		{
			targetCorners[i][0] = this->currentScreenSize.x + 1;
		}

		if (targetCorners[i][1] > this->currentScreenSize.y + this->currentScreenSize.h)
		{
			targetCorners[i][1] = (this->currentScreenSize.x + this->currentScreenSize.w) - this->getHeight() - 1;
		}
		if (targetCorners[i][1] < this->currentScreenSize.y)
		{
			targetCorners[i][1] = this->currentScreenSize.y + 1;
		}
	}

	this->targetCorners = targetCorners;

	return targetCorners;
}
void Enemy::setPlayerRect(SDL_FRect playerRect)
{
	this->playerRect = playerRect;
}
void Enemy::setScreenBorders(SDL_Rect currentScreenSize)
{
	this->currentScreenSize = currentScreenSize;
}
#pragma endregion