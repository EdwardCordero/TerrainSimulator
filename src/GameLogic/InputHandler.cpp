#include "GameLogic\InputHandler.h"
#include "Player\Player.h"
#include "GameLogic\HelperFunctions.h"
#include <string>
#include <format>
#include <SDL.h>
#include <memory>

InputHandler::InputHandler()
{
	this->player = nullptr;
	this->inputConfig = DefenderInputConfig;
	this->movingUp = 0;
	this->movingRight = 0;
	this->movingDown = 0;
	this->movingLeft = 0;
	this->shoot = false;
}

InputHandler::~InputHandler()
{

}

void InputHandler::setPlayer(std::shared_ptr<Player> player)
{
	this->player = player;
}

void InputHandler::setInputSchema(InputConfig newConfig)
{
	this->inputConfig = newConfig;
}

void InputHandler::readKeyboardInput(const Uint8* keyState)
{
	// Get directions for magnitude later
	this->movingDown = keyState[SDL_SCANCODE_DOWN] ? 1 : 0;
	this->movingUp = keyState[SDL_SCANCODE_UP] ? 1 : 0;
	this->movingRight = keyState[SDL_SCANCODE_RIGHT] ? 1 : 0;
	this->movingLeft = keyState[SDL_SCANCODE_LEFT] ? 1 : 0;

	this->shoot = keyState[SDL_SCANCODE_SPACE];
}

void InputHandler::handleMovement(float deltaTime)
{
	switch (this->inputConfig)
	{
		case DefenderInputConfig:
			this->useDefenderConfig(deltaTime);
			break;
		case AsteroidsInputConfig:
			this->useAsteroidsConfig(deltaTime);
			break;
		default:
			std::cout << "Invalid Input Config" << std::endl;
			break;
	}
}

void InputHandler::useDefenderConfig(float deltaTime)
{
	// Player constants
	float playerAcceleration = this->player->getAcceleration();

	// Player input vector - Get player input magnitude and direction
	float inputDirX = (movingRight - movingLeft);
	float inputDirY = (movingDown - movingUp);
	float magnitude = sqrt(inputDirX * inputDirX + inputDirY * inputDirY);

	// if any input
	if (magnitude > 0)
	{
		inputDirX /= magnitude;
		inputDirY /= magnitude;
		float newVelocity = this->player->getCurrentVelocity() + (playerAcceleration * magnitude * deltaTime);

		this->player->setCurrentXDirection(inputDirX);
		this->player->setCurrentYDirection(inputDirY);
		this->player->setCurrentVelocity(newVelocity);
		std::cout << std::format("Current Velocity {}, X Direction: {}, Y Direction {}", this->player->getCurrentVelocity(), this->player->getCurrentXDirection(), this->player->getCurrentYDirection()) << std::endl;

	}
	else {
		if (this->player->getCurrentVelocity() > 0)
		{
			float newVelocity = this->player->getCurrentVelocity() - playerAcceleration * deltaTime;
			this->player->setCurrentVelocity(newVelocity);
		}
		else
		{
			this->player->setCurrentXDirection(0);
			this->player->setCurrentYDirection(0);
		}
	}
// increase positions
	float newX = this->player->getXPos() + (this->player->getCurrentVelocity() * this->player->getCurrentXDirection()) * deltaTime;
	float newY = this->player->getYPos() + (this->player->getCurrentVelocity() * this->player->getCurrentYDirection()) * deltaTime;
	this->player->setXPos(newX);
	this->player->setYPos(newY);

	if (this->shoot)
	{
		this->player->getWeapon()->shoot((this->player->getXPos() + (this->player->getWidth() / 2)), (this->player->getYPos() + (this->player->getHeight() / 2)), this->player->getRotateDeg());
	}
}

void InputHandler::useTestingConfig()
{

}


void InputHandler::useAsteroidsConfig(float deltaTime)
{
	// Player input vector - Get player input magnitude and direction
	float inputDirX = (this->movingRight - this->movingLeft);
	float inputDirY = this->movingUp - this->movingDown;
	float magnitude = sqrt(inputDirX * inputDirX + inputDirY * inputDirY);

	if (magnitude > 0)
	{
		// speed quantity, always increases
		float newCurrentVelocity = this->player->getCurrentVelocity() + (this->player->getAcceleration() * magnitude * deltaTime);
		// roatation quantity, this always increases
		int newRotateSpeed = this->player->getRotateSpeed() + (80 * magnitude * deltaTime);

		this->player->setRotateSpeed(newRotateSpeed);
		this->player->setCurrentVelocity(newCurrentVelocity);
	}
	
	float newRotateForce = (inputDirX != 0) ? inputDirX * this->player->getRotateSpeed() : 0;
	this->player->setRotationForce(newRotateForce);
	this->player->setCurrentXDirection(inputDirX);

	if (inputDirY != 0)
	{
		this->player->setCurrentYDirection(inputDirY);
	}
	else
	{
		if (this->player->getCurrentVelocity() > 0)
		{
			float newCurrentVelocity = this->player->getCurrentVelocity() - (this->player->getAcceleration() * deltaTime);
			this->player->setCurrentVelocity(newCurrentVelocity);
		}
		else
		{
			this->player->setCurrentYDirection(0);
		}
	}


	// increase positions
	float newRotationDeg = this->player->getRotateDeg() + (this->player->getRotationForce() * deltaTime);
	this->player->setRotateDeg(newRotationDeg);

	float newX = this->player->getXPos() + (this->player->getCurrentVelocity() * this->player->getCurrentYDirection()) * cos(this->player->getRotateDeg() * M_PI / 180.00) * deltaTime;
	float newY = this->player->getYPos() + (this->player->getCurrentVelocity() * this->player->getCurrentYDirection()) * sin(this->player->getRotateDeg() * M_PI / 180.00) * deltaTime;
	this->player->setXPos(newX);
	this->player->setYPos(newY);

	if (this->shoot)
	{
		this->player->getWeapon()->shoot((this->player->getXPos() + (this->player->getWidth() / 2)), (this->player->getYPos() + (this->player->getHeight() / 2)), this->player->getRotateDeg());
	}
}