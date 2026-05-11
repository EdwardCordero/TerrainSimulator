#include "globalConstants.h"
#include "Scenes\Scene.h"
#include "Scenes\DefenderScene.h"
#include "Scenes\SceneManager.h"
#include "Physics\RigidBody.h"
#include "Player\PlayerBuilder.h"
#include "GameLogic\InputHandler.h"
#include "Player\Player.h"
#include <memory>

DefenderScene::DefenderScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName): Scene(sceneManager, sceneName)
{
	this->player = nullptr;
	this->userInterface = nullptr;
}

DefenderScene::~DefenderScene()
{
}

void DefenderScene::initialize()
{
	if (!this->mInitialized)
	{
		this->mInitialized = true;
		this->player = this->playerBuilder.setSettingsManager(this->settingsManager).setGraphicsManager(this->graphicsManager).setDefenderConfig().build();
		this->player->spawnPlayer();
		this->player->getWeapon()->setCollisionManager(this->mCollisionManager);

		this->userInterface = std::make_shared<UserInterface>("Defender", 3, this->player, this->graphicsManager, this->settingsManager);
		this->userInterface->setGameTime(this->gameTime);
		this->userInterface->intialize();

		this->mCollisionManager->addRigidBody(this->player);

		this->inputHandler->setPlayer(this->player);
		this->inputHandler->setInputSchema(DefenderInputConfig);

		this->enemyFactory = EnemyFactory(this->mCollisionManager, this->settingsManager, this->graphicsManager);
		this->enemyFactory.setPlayer(this->player);
	}
}

void DefenderScene::restart()
{
	this->player->spawnPlayer();
	this->userInterface->setGameTime(this->gameTime);
	this->enemyFactory.refresh();
}

void DefenderScene::handleEvents(SDL_Event& event)
{
	
}

void DefenderScene::update(const Uint8* keyState)
{
	//this->inputHandler->readKeyboardInput(keyState);
	this->inputHandler->handleMovement(this->graphicsManager->getDeltaTime());

	// Player update
	player->update(keyState, this->graphicsManager->getDeltaTime(), this->graphicsManager->getScreenRect());

	// enemy spawn
	enemyFactory.createPawn(player->getXPos() + 100, player->getYPos());

	// Move pawns
	enemyFactory.updatePawns();

	this->userInterface->update();
}

void DefenderScene::render()
{
	this->graphicsManager->preRender();

	SDL_FRect playerRect = this->player->getRigidBodyRect();
	this->graphicsManager->renderTextureEx(this->player->getSpriteSheet().getSheetTexture(), this->player->getSpriteSheet().getSpriteRect(), &playerRect, this->player->getRotateDeg(), SDL_FLIP_NONE);
	this->userInterface->render();
	this->enemyFactory.renderEnemies();

	std::vector<std::shared_ptr<Bullet>> bullets = this->player->getWeapon()->getBullets();
	for (int i = 0; i < bullets.size(); i++)
	{
		SDL_FRect bulletRect = bullets[i]->getRigidBodyRect();
		this->graphicsManager->renderTexture(this->player->getWeapon()->getAmmoTexture(), &bulletRect);
	}

	this->graphicsManager->render();
}

void DefenderScene::updateRects()
{
	SDL_FRect playerRect = this->player->getRigidBodyRect();
	this->graphicsManager->scaleRect(playerRect);
	this->player->setRigidBodyRect(playerRect);

	this->userInterface->updateScreenSize();
	this->enemyFactory.updateScreenSize();
}

void DefenderScene::display()
{
	this->mActive = true;
}

void DefenderScene::hide()
{
	this->mActive = false;
}