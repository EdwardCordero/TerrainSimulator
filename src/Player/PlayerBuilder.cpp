#include "Player\PlayerBuilder.h"
#include "Player\Player.h"
#include "Graphics\Texture.h"
#include "Armory\Weapon.h"
#include "Armory\WeaponTypeEnum.h"
#include <SDL.h>
#include <string>
#include <memory>

PlayerBuilder::PlayerBuilder()
{
	this->player = std::make_shared<Player>();
	this->settings = nullptr;
	this->graphicsManager = nullptr;
}

PlayerBuilder::~PlayerBuilder()
{
	std::cout << "Deleting Player Builder" << std::endl;
}

std::shared_ptr<Player> PlayerBuilder::build()
{
	std::shared_ptr<Player> temp = this->player;
	this->player = nullptr;
	return temp;
}
PlayerBuilder& PlayerBuilder::setGame(std::string gameName)
{
	this->player->setGame(gameName);
	return *this;
}

PlayerBuilder& PlayerBuilder::setSettingsManager(std::shared_ptr<SettingsManager> settingsManager)
{
	this->settings = settingsManager;
	this->player->setSettingsManager(settingsManager);
	return *this;
}

PlayerBuilder& PlayerBuilder::setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager)
{
	this->graphicsManager = graphicsManager;
	this->player->setGraphicsManager(graphicsManager);
	return *this;
}

PlayerBuilder& PlayerBuilder::setSpriteSheet(std::string imgPath, int spriteSheetWidth, int spriteSheetHeight, int totalSpriteCount)
{
	this->player->setSpriteSheet(imgPath, spriteSheetWidth, spriteSheetHeight, totalSpriteCount);
	this->player->getSpriteSheet().initalizeSheetTexture(this->graphicsManager->getRenderer());
	return *this;
}

PlayerBuilder& PlayerBuilder::setHitbox(SDL_FRect hitbox)
{
	this->player->setRigidBodyRect(hitbox);
	return *this;
}

PlayerBuilder& PlayerBuilder::setWeapon(WeaponTypeEnum weaponType)
{
	this->graphicsManager->cacheTexture("Bullet");
	this->player->getWeapon()->setPlayer(this->player);
	this->player->getWeapon()->setWeaponType(weaponType, this->graphicsManager->getCachedTexture("Bullet"));
	return *this;
}
PlayerBuilder& PlayerBuilder::setHealth(int health)
{
	this->player->setHealth(health);
	return *this;
}
PlayerBuilder& PlayerBuilder::setMaxVelocity(float maxVelocity)
{
	this->player->setMaxVelocity(maxVelocity);
	return *this;
}
PlayerBuilder& PlayerBuilder::setAcceleration(float acceleration)
{
	this->player->setAcceleration(acceleration);
	return *this;
}

PlayerBuilder& PlayerBuilder::setStocks(int stocks)
{
	this->player->setStocks(stocks);
	return *this;
}

PlayerBuilder& PlayerBuilder::setAsteroidsConfig()
{
	std::cout << "Iniliaze asteroids player" << std::endl;

	this->setSpriteSheet(this->graphicsManager->getTexturePath("AsteroidPlayer"), 64, 64, 1)
		.setGame("ASTEROIDS")
		.setHitbox({ float(this->settings->getCurrentSettings().display.width / 2), float(this->settings->getCurrentSettings().display.height / 2), 64, 64 })
		.setHealth(1)
		.setStocks(3)
		.setMaxVelocity(300)
		.setAcceleration(150)
		.setWeapon(MINI_GUN);

	return *this;
}
//TODO: fix spritesheet to work with new GraphicsManager
PlayerBuilder& PlayerBuilder::setDefenderConfig()
{

	std::cout << "Iniliaze defender player" << std::endl;

	this->setSpriteSheet(this->graphicsManager->getTexturePath("DefenderPlayerSheet"), 256, 64, 4)
		.setGame("DEFENDER")
		.setHitbox({ float(this->settings->getCurrentSettings().display.width / 2), float(this->settings->getCurrentSettings().display.height / 2), 64, 64 })
		.setHealth(1)
		.setStocks(3)
		.setMaxVelocity(300)
		.setAcceleration(50)
		.setWeapon(MINI_GUN);

	return *this;
}