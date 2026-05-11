#include "Scenes\Scene.h"
#include "Scenes\SceneManager.h"
#include "Physics\RigidBody.h"
#include "Physics\CollisionManager.h"
#include "Utils\Stopwatch.h"
#include "globalConstants.h"
#include <SDL.h>
#include <string>

Scene::Scene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : sceneManager(sceneManager)
{
	//this->mCollisionManager = CollisionManager();
	this->mActive = false;
	this->mInitialized = false;
	this->mCollisionManager = nullptr;
	this->inputHandler = nullptr;
	this->settingsManager = nullptr;
	this->gameTime = sceneManager->getGameTime();

	this->defaultFont = "heavydata";
	this->defaultTextColor = { 255, 255, 255, 0 };
	this->defaultTextColorName = "white";
	this->hoverTextColor = { 255, 0, 0, 0 };
	this->hoverTextColorName = "red";
	this->margin = 0;
	this->screenWidth = 0;
	this->screenHeight = 0;
	this->sceneName = sceneName;
}
Scene::~Scene() {};

bool Scene::isInitailzed()
{
	return this->mInitialized;
}

void Scene::display()
{
	this->mActive = true;
}

void Scene::hide()
{
	this->mActive = false;
}

bool Scene::isActive()
{
	return this->mActive;
}

void Scene::setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager)
{
	this->graphicsManager = graphicsManager;
}

void Scene::setCollisionManager(std::shared_ptr<CollisionManager> collisionManager)
{
	this->mCollisionManager = collisionManager;
}

void Scene::setInputHandler(std::shared_ptr<InputHandler> inputHandler)
{
	this->inputHandler = inputHandler;
}

void Scene::setSettingsManager(std::shared_ptr<SettingsManager> settingsManager)
{
	this->settingsManager = settingsManager;
}

void Scene::setGameTime(std::shared_ptr<Stopwatch> gameTime)
{
	this->gameTime = gameTime;
}

void Scene::setInitialize(bool isInitialized)
{
	this->mInitialized = isInitialized;
}

std::string Scene::getSceneName()
{
	return this->sceneName;
}
void Scene::setSceneName(std::string sceneName)
{
	this->sceneName = sceneName;
}