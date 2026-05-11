#pragma once
#include "Player\Player.h"
#include <SDL.h>
#include <memory>

enum InputConfig
{
	DefenderInputConfig, AsteroidsInputConfig
};

class InputHandler
{
	public:
		InputHandler();
		~InputHandler();
		void setPlayer(std::shared_ptr<Player> player);
		void setInputSchema(InputConfig newConfig);
		void readKeyboardInput(const Uint8* keySate);
		void handleMovement(float deltaTime);
		void useDefenderConfig(float deltaTime);
		void useAsteroidsConfig(float deltaTime);
		void useTestingConfig();

		/* Input Variables */
		// Movement
		int movingUp;
		int movingRight;
		int movingDown;
		int movingLeft;
	private:
		std::shared_ptr<Player> player;
		InputConfig inputConfig;
		// Weapon
		bool shoot;
};
