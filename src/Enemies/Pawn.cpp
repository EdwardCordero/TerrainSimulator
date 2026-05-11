#include "Enemies\Pawn.h"
#include "GameLogic\HelperFunctions.h"

Pawn::Pawn()
{
	//this->spawn();
}

Pawn::~Pawn()
{
	this->despawn();
}

void Pawn::spawn(SDL_Rect currentScreenRect)
{
	this->setScreenBorders(currentScreenRect);
	this->initializeValues();
}

void Pawn::handleStateSpecificUpdate(float deltaTime)
{
	int screenWidth = this->currentScreenSize.x + this->currentScreenSize.w;
	int screenHeight = this->currentScreenSize.y + this->currentScreenSize.h;

	EnemyState movementState = this->getMovementState();
	std::array<float, 2> newStartPos = this->getStartPos();
	std::array<float, 2> newDestination = this->getDestination();
	float newMovementProgress = this->getMovementProgress();
	std::array<float, 2> newPosition = { this->getXPos(), this->getYPos() };
	std::array<std::array<float, 2>, 4> newTargetCorners = this->getTargetCorners();
	// Vertical Movement, working on moving around player
	if (movementState == Attacking)
	{
		newPosition = MoveRectAroundTarget(deltaTime, this->getRigidBodyRect(), newTargetCorners, newStartPos, newDestination, newMovementProgress, this->getSpeed(), false);
	}
	// Horizontal Movement
	if (movementState == Roaming)
	{
		newPosition = MoveRectBetweenPoints(deltaTime, this->getRigidBodyRect(), newStartPos, newDestination, newMovementProgress, this->getSpeed());
	}

	// check OOB and adjust destination/start position
	if (newMovementProgress && this->getDestination() == newDestination)
	{
		Limit limitHit;
		// Might need to check x & y seperatley or update logic to return all bound hits not just the first border that it hits/checks
		limitHit = isOutOfBound(newPosition[0], newPosition[1], 0, this->getWidth(), this->getHeight(), 0, this->currentScreenSize.w, this->currentScreenSize.h);
		if (limitHit != NO_BORDER)
		{
			newMovementProgress = 1;
			// The function is offseting the x or y that hit the border by 2 to prevent it from shifiting it immediately after 1 frame since it's fast
			int difference = shiftPointsWithinLimit(newPosition, newDestination, newStartPos, limitHit, this->getWidth(), this->getHeight(), screenWidth, screenHeight);
		}
	}

	this->setStartPos(newStartPos[0], newStartPos[1]);
	this->setDestination(newDestination[0], newDestination[1]);
	this->setXPos(newPosition[0]);
	this->setYPos(newPosition[1]);
	this->setMovementProgess(newMovementProgress);
}

void Pawn::updateState()
{
	int destinationCounter = this->getDestinationCounter();
	EnemyState movementState = this->getMovementState();
	if (isEnemyNearPlayer() && this->getMovementState() == Roaming)
	{
		std::cout << "ATTACKING" << std::endl;
		this->setMovementState(Attacking);
		this->isDoneAttacking = false;

		SDL_FRect hitBox = this->getRigidBodyRect();
		int presenceBoxOffset = 80;
		SDL_FRect lineOfSight = { hitBox.x - presenceBoxOffset , hitBox.y - presenceBoxOffset, hitBox.w + (presenceBoxOffset * 2), hitBox.h + (presenceBoxOffset * 2) };
		Limit sideOfContact = whichSideCollided(lineOfSight, hitBox);

		//std::cout << std::format("Player Detected on {} side", sideOfContact) << std::endl;
		std::array<std::array<float, 2>, 4> targetCorners = this->initializeTargetCorners(this->getPlayerRect(), false);
		std::array<float, 2> destination = this->getDestination();

		switch (sideOfContact)
		{
			case TOP_BORDER:
				destination = targetCorners[2];
				//std::cout << std::format("Player Detected on TOP side, startPos {}, destination {}", hitbox.x, destination) << std::endl;
				break;
			case RIGHT_BORDER:
				destination = targetCorners[3];
				//std::cout << std::format("Player Detected on RIGHT side, startPos {}, destination {}", hitbox.y, destination) << std::endl;
				break;
			case BOTTOM_BORDER:
				destination = targetCorners[0];
				//std::cout << std::format("Player Detected on BOTTOM side, startPos {}, destination {}", hitbox.x, destination) << std::endl;
				break;
			case LEFT_BORDER:
				destination = targetCorners[1];
				//std::cout << std::format("Player Detected on LEFT side, startPos {}, destination {}", hitbox.y, destination) << std::endl;
				break;
			default:
				std::cout << "Something went wrong when switching to attack state. No border hit was set" << std::endl;
				break;
		}
		this->setMovementProgess(0.0);
		this->setRotateDeg(0);
		this->setDestination(destination[0], destination[1]);
		this->setStartPos(hitBox.x, hitBox.y);
	}
	/*if (!isPresenceBoxOnPlayer() && this->getMovementState() == Attacking)
	{
		std::cout << "Player moved out of range, going back to ROAMING" << std::endl;
		this->setMovementState(Roaming);
		this->InitalizeSate();
	}*/
}