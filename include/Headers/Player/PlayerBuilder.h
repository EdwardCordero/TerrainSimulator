#pragma once
#include "Player\Player.h"
#include "Graphics\Texture.h"
#include "Armory\WeaponTypeEnum.h"
#include "Settings\SettingsManager.h"
#include "Settings\GameSettings.h"
#include "Graphics\GraphicsManager.h"
#include <SDL.h>
#include <string>
#include <memory>

class PlayerBuilder
{
	private:
		std::shared_ptr<Player> player;
	public:
		PlayerBuilder();
		~PlayerBuilder();
		// Remember these are configs, these funcs will only be used to set player values before returning it to actual game. 
		// Player updates will occur in the update function within the base player class
		std::shared_ptr<Player> build();
		std::shared_ptr<SettingsManager> settings;
		std::shared_ptr<GraphicsManager> graphicsManager;

		PlayerBuilder& setGame(std::string gameName);
		PlayerBuilder& setSpriteSheet(std::string imgPath, int spriteSheetWidth, int spriteSheetHeight, int totalSpriteCount);
		PlayerBuilder& setHitbox(SDL_FRect hitbox);
		PlayerBuilder& setWeapon(WeaponTypeEnum weaponType);
		// Stats
		PlayerBuilder& setHealth(int health);
		PlayerBuilder& setMaxVelocity(float maxVelocity);
		PlayerBuilder& setAcceleration(float acceleration);
		PlayerBuilder& setStocks(int stocks);
		// initailzes players for specific games
		PlayerBuilder& setAsteroidsConfig();
		PlayerBuilder& setDefenderConfig();
		// Settings
		PlayerBuilder& setSettingsManager(std::shared_ptr<SettingsManager> settingsManager);
		PlayerBuilder& setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager);
};