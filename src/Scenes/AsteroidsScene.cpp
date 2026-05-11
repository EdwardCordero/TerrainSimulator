#include "Scenes\AsteroidsScene.h"
#include "Scenes\Scene.h"
#include "Scenes\SceneManager.h"
#include "Factories\EnemyFactory.h"
#include "Player\PlayerBuilder.h"
#include "Physics\RigidBody.h"
#include "Animations\SpriteSheet.h"
#include "UserInterface.h"
#include "GameLogic\InputHandler.h"
#include "Graphics\GraphicsManager.h"
#include <string>
#include <SDL.h>
#include <memory>

AsteroidsScene::AsteroidsScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : Scene(sceneManager, sceneName)
{
	this->player = nullptr;
	this->userInterface = nullptr;
	this->inputHandler = nullptr;
}

AsteroidsScene::~AsteroidsScene()
{
	std::cout << "Deconstructing asteroids scene" << std::endl;
}

void AsteroidsScene::initialize()
{
	if (!this->mInitialized)
	{
		this->mInitialized = true;
		this->player = this->playerBuilder.setSettingsManager(this->settingsManager).setGraphicsManager(this->graphicsManager).setAsteroidsConfig().build();
		this->player->spawnPlayer();
		this->player->getWeapon()->setCollisionManager(this->mCollisionManager);

		/* User Interface */
		this->userInterface = std::make_shared<UserInterface>("Asteroids", 3, player, this->graphicsManager, this->settingsManager);
		this->userInterface->setGameTime(this->gameTime);
		this->userInterface->intialize();

		/* Collision Manager */
		this->mCollisionManager->addRigidBody(this->player);

		/* Input Handler */
		this->inputHandler->setPlayer(this->player);
		this->inputHandler->setInputSchema(AsteroidsInputConfig);

		/* Enemy Initalization */
		enemyFactory = EnemyFactory(this->mCollisionManager, this->settingsManager, this->graphicsManager);
		enemyFactory.setPlayer(this->player);
		// Initailize enemy pool
		for (int i = 0; i < 5; i++)
		{
			enemyFactory.createAsteroid();
		}
		for (int i = 0; i < this->enemyFactory.getEnemyPoolSize(); i++)
		{
			this->mCollisionManager->addRigidBody(this->enemyFactory.getEnemy(i));
		}
	}
}

void AsteroidsScene::restart()
{
	this->mCollisionManager->clearGrid();
	this->player->spawnPlayer();
	this->mCollisionManager->addRigidBody(this->player);
	this->enemyFactory.refresh();
	for (int i = 0; i < 5; i++)
	{
		enemyFactory.createAsteroid();
	}
	for (int i = 0; i < this->enemyFactory.getEnemyPoolSize(); i++)
	{
		this->mCollisionManager->addRigidBody(this->enemyFactory.getEnemy(i));
	}

}

void AsteroidsScene::handleEvents(SDL_Event& event)
{

}

void AsteroidsScene::update(const Uint8* keyState)
{
	//this->inputHandler->readKeyboardInput(keyState);
	this->inputHandler->handleMovement(this->graphicsManager->getDeltaTime());

	this->player->update(keyState, this->graphicsManager->getDeltaTime(), this->graphicsManager->getScreenRect());

	this->enemyFactory.update();

	this->userInterface->update();

	this->mCollisionManager->update();
}

void AsteroidsScene::render()
{
	this->graphicsManager->preRender();

	SDL_FRect playerRect = this->player->getRigidBodyRect();

	this->graphicsManager->renderTextureEx(this->player->getSpriteSheet().getSheetTexture(), this->player->getSpriteSheet().getSpriteRect(), &playerRect, this->player->getRotateDeg(), SDL_FLIP_NONE);
	this->userInterface->render();
	enemyFactory.renderEnemies();

	std::vector<std::shared_ptr<Bullet>> bullets = this->player->getWeapon()->getBullets();
	for (int i = 0; i < bullets.size(); i++)
	{
		SDL_FRect bulletRect = bullets[i]->getRigidBodyRect();
		this->graphicsManager->renderTexture(this->player->getWeapon()->getAmmoTexture(), &bulletRect);
	}

	this->graphicsManager->render();
}

void AsteroidsScene::updateRects()
{
	SDL_FRect playerRect = this->player->getRigidBodyRect();
	this->graphicsManager->scaleRect(playerRect);
	this->player->setRigidBodyRect(playerRect);
	this->userInterface->updateScreenSize();
	this->enemyFactory.updateScreenSize();
}

void AsteroidsScene::display()
{
	this->mActive = true;
}

void AsteroidsScene::hide()
{
	this->mActive = false;
}