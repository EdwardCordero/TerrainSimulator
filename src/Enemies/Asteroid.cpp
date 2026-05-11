#include "Enemies\Asteroid.h"
#include "Player\Player.h"
#include "GameLogic\HelperFunctions.h"
#include "globalConstants.h"

Asteroid::Asteroid()
{
	this->size = 0;
	this->setActive(false);
	this->setBodyName("Asteroid");
}
Asteroid::~Asteroid()
{

}

void Asteroid::setSpawnLocation(float x, float y, int size)
{
	this->setXPos(x);
	this->setYPos(y);
	this->startPos[0] = this->getXPos();
	this->startPos[1] = this->getYPos();
	int screenWidth = this->currentScreenSize.x + this->currentScreenSize.w;
	int screenHeight = this->currentScreenSize.y + this->currentScreenSize.h;
	// check if starting left/right or top/below of screen
	if (x <= this->currentScreenSize.x || x >= screenWidth)
	{
		this->destination[0] = (x >= screenWidth) ? this->currentScreenSize.x : screenWidth + this->getWidth();
		this->destination[1] = randNumBetween(this->currentScreenSize.y + this->getHeight(), screenHeight - this->getHeight());
	}

	this->size = size;
}

void Asteroid::spawn(SDL_Rect currentScreenRect)
{
	this->setScreenBorders(currentScreenRect);
	int screenWidth = this->currentScreenSize.x + this->currentScreenSize.w;
	int screenHeight = this->currentScreenSize.y + this->currentScreenSize.h;

	//std::vector<int> sizes = { 1, 2, 4, 10 };
	std::vector<int> sizes = { 1, 2, 4, 7 };
	this->initializeValues();
	this->size = randNumInVector(sizes);
	Enemy::points = size;
	this->scaleGameObject(size);

	float width = this->getWidth();// this->getWidth();
	float height = this->getHeight();
	bool spawnAtSides = randNumBetween(0, 1) == 0 ? true : false;

	if (spawnAtSides)
	{
		float randomXPos = randNumInFloatVector({ this->currentScreenSize.x - width, screenWidth + width });
		float randomYPos = randNumBetween(this->currentScreenSize.y + height, screenHeight - height);
		this->setXPos(randomXPos);
		this->setYPos(randomYPos);


		this->destination[0] = randomXPos < 0 ? screenWidth + width : this->currentScreenSize.x - width;
		this->destination[1] = randNumBetween(this->currentScreenSize.y + height, screenHeight - height);
	}
	else
	{
		int randomXPos = randNumBetween(this->currentScreenSize.x + width, screenWidth - width );
		int randomYPos = randNumInFloatVector({ this->currentScreenSize.y - height, screenHeight + height });
		this->setXPos(randomXPos);
		this->setYPos(randomYPos);


		this->destination[0] = randNumBetween(this->currentScreenSize.x + width, screenWidth - width);
		this->destination[1] = randomYPos < 0 ? screenHeight + height : this->currentScreenSize.y - height;
	}
	this->startPos[0] = this->getXPos(); 
	this->startPos[1] = this->getYPos();

	//std::cout << std::format("Asteroid spawning at X: {} & Y: {}", this->getXPos(), this->getYPos()) << std::endl;
	//std::cout << std::format("Asteroid Destination is X: {} & Y: {}", this->destination[0], this->destination[1]) << std::endl;

	// Get random roation direction
	this->roationDirection = randNumBetween(0, 1) == 0 ? -1 : 1;

	this->speed = 500;
	this->rotationSpeed = 800 / size;
}

void Asteroid::handleStateSpecificUpdate(float deltaTime)
{
	float distanceLeft = calculateDistance(this->startPos[0], this->destination[0], this->startPos[1], this->destination[1]);
	float distanceToTravel = distanceLeft / this->speed;

	if (this->movementProgress < 1.0f)
	{
		this->movementProgress += deltaTime / distanceToTravel;
		if (this->movementProgress > 1.0f) { this->movementProgress = 1.0f; }
		float newX = lerp(this->startPos[0], this->destination[0], movementProgress);
		float newY = lerp(this->startPos[1], this->destination[1], movementProgress);

		this->setXPos(newX);
		this->setYPos(newY);
		float newRotationDeg = this->getRotateDeg() + (rotationSpeed) * deltaTime * roationDirection;
		this->setRotateDeg(newRotationDeg);
	}
	else
	{
		this->despawn();
	}
}

void Asteroid::updateState()
{

}

void Asteroid::onCollision(std::shared_ptr<RigidBody> bodyCollidingInto)
{
	if (bodyCollidingInto->getBodyType() == PlayerBody)
	{
		// Downcast to shared_ptr<Player> if it's actually a Player
		std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(bodyCollidingInto);
		if (p) {
			//std::cout << "Asteroid collided with player, health BEFORE: " << p->getHealth() << std::endl;
			int newHealth = p->getHealth() - this->getDmg();
			p->setHealth(newHealth); // Calls Player-specific function
			//std::cout << "Asteroid hit player, health AFTER: " << p->getHealth() << std::endl;
		}

	}
}