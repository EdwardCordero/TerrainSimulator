#include "Scenes\Scene.h"
#include "Scenes\OptionsScene.h"
#include "Scenes\LeaderboardScene.h"
#include "Scenes\SceneManager.h"
#include "Physics\CollisionManager.h"
#include "Factories\EnemyFactory.h"
#include "Utils\Stopwatch.h"
#include <memory>

SceneManager::SceneManager() : currentScene(nullptr) {
	this->collisionManager = nullptr;
	this->inputHandler = nullptr;
	this->currentScene = nullptr;
	this->graphicsManager = nullptr;
	this->gameSelectScene = nullptr;
	this->prevScene = nullptr;
	this->gameTime = std::make_shared<Stopwatch>();
	this->isPaused = false;
	this->running = false;
	this->disablePause = false;
	this->gameplayScene = nullptr;
}

SceneManager::~SceneManager()
{

}

void SceneManager::initalize(std::shared_ptr<SceneManager> self, std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<SettingsManager> settingsManager)
{
	this->running = true;
	this->graphicsManager = graphicsManager;
	this->settingsManager = settingsManager;

	this->collisionManager = std::make_shared<CollisionManager>();
	this->collisionManager->setDebug(true);
	this->collisionManager->setGraphicsManager(this->graphicsManager);
	this->collisionManager->intialize();

	this->inputHandler = std::make_shared<InputHandler>();


	this->pauseMenuScene = std::make_shared<PauseMenuScene>(self, "PauseMenu");
	this->addScene(this->pauseMenuScene);
	this->pauseMenuScene->initialize();

	this->gameSelectScene = make_shared<GameSelectScene>(self, "GameSelectScene");
	this->addScene(this->gameSelectScene);
	this->changeScene(this->gameSelectScene);
	this->prevScene = this->currentScene;
}

void SceneManager::changeScene(std::shared_ptr<Scene> newScene)
{
	if (this->currentScene != nullptr)
	{
		this->prevScene = this->currentScene;
		std::cout << std::format("Prev scene is: {}, Current Scene is: {}", this->prevScene->getSceneName(), newScene->getSceneName()) << std::endl;
		this->disableScene(this->currentScene->getSceneName());
	}

	if (!newScene->isInitailzed())
	{
		newScene->initialize();
		newScene->setInitialize(true);
	}
	else
	{
		newScene->restart();
	}

	this->currentScene = newScene;
	this->enableScene(newScene->getSceneName());
}

bool SceneManager::addScene(std::shared_ptr<Scene> newScene)
{
	auto it = this->sceneDictionary[false].find(newScene->getSceneName());
	if (it != this->sceneDictionary[false].end())
	{
		this->sceneDictionary[false].erase(it);
	}
	newScene->setCollisionManager(this->collisionManager);
	newScene->setInputHandler(this->inputHandler);
	newScene->setSettingsManager(this->settingsManager);
	newScene->setGraphicsManager(this->graphicsManager);
	newScene->hide();

	this->sceneDictionary[false][newScene->getSceneName()] = newScene;
	return true;
}

void SceneManager::updateRects()
{
	for (auto& [active, currentScene] : this->sceneDictionary)
	{
		for (auto& [sceneName, sceneObj] : currentScene)
		{
			if (sceneObj->isInitailzed())
			{
				sceneObj->updateRects();
			}
		}
	}
}

bool SceneManager::addGameplayScene(std::shared_ptr<Scene> gameplayScene)
{
	this->gameplayScene = gameplayScene;
	return this->addScene(gameplayScene);
}

void SceneManager::disableScene(std::string sceneName)
{
	auto it = this->sceneDictionary[true].find(sceneName);
	if (it != this->sceneDictionary[true].end())
	{
		this->sceneDictionary[true][sceneName]->hide();
		this->sceneDictionary[false][sceneName] = std::move(it->second);
		this->sceneDictionary[true].erase(it);
	}
}

void SceneManager::enableScene(std::string sceneName)
{
	auto it = this->sceneDictionary[false].find(sceneName);
	if (it != this->sceneDictionary[false].end())
	{
		this->sceneDictionary[false][sceneName]->display();
		this->sceneDictionary[true][sceneName] = std::move(it->second);
		this->sceneDictionary[false].erase(it);
	}
}

void SceneManager::startGameTime()
{
	this->gameTime->start();
}

std::vector<std::shared_ptr<Scene>> SceneManager::getActiveScenes()
{
	std::vector<std::shared_ptr<Scene>> activeScenes;
	for (auto& [sceneName, scene] : this->sceneDictionary[true])
	{
		activeScenes.push_back(scene);
	}

	return activeScenes;
}

void SceneManager::handleEvents(SDL_Event& event)
{
	if (event.type == SDL_QUIT)
	{
		running = false;
	}

	if (!disablePause)
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			if (this->pauseMenuScene->isActive())
			{
				this->unpause();
			}
			else
			{
				this->pause();
			}
		}
	}

	// need this because updating the ptr like the function's for loop will cause it to error out. Didn't document this right away so I forgot why and I don't want to look into it rn. It just works.
	for (std::shared_ptr<Scene> scene : this->getActiveScenes())
	{
		scene->handleEvents(event);
	}
}

void SceneManager::pause()
{
	this->enableScene(this->pauseMenuScene->getSceneName());
	this->disableScene(this->currentScene->getSceneName());
	/*if (this->gameplayScene != nullptr)
	{
		this->disableScene(this->gameplayScene->getSceneName());
	}*/
	this->gameTime->pause();
	this->isPaused = true;
}

void SceneManager::unpause()
{
	this->disableScene(this->pauseMenuScene->getSceneName());
	this->enableScene(this->currentScene->getSceneName());
	/*if (this->gameplayScene != nullptr)
	{
		this->enableScene(this->gameplayScene->getSceneName());
	}*/
	this->gameTime->unpause();
	this->isPaused = false;
}

void SceneManager::update(const Uint8* keyState)
{
	this->inputHandler->readKeyboardInput(keyState);
	for (auto& [sceneName, scene] : this->sceneDictionary[true])
	{
		scene->update(keyState);
	}
}

void SceneManager::render()
{
	if (this->gameplayScene != nullptr && this->gameplayScene->isInitailzed() && this->gameplayScene->isActive())
	{
		this->gameplayScene->render();
	}

	for (auto& [sceneName, scene] : this->sceneDictionary[true])
	{
		scene->render();
	}
}

bool SceneManager::isRunning()
{
	return this->running;
}

void SceneManager::setRunning(bool isRunning)
{
	this->running = isRunning;
}

std::shared_ptr<Stopwatch> SceneManager::getGameTime()
{
	return this->gameTime;
}

void SceneManager::changeToGameSelectScene()
{
	this->changeScene(this->gameSelectScene);
	if (this->pauseMenuScene->isActive())
	{
		this->pauseMenuScene->hide();
	}
	if (this->gameTime->isPaused())
	{
		this->gameTime->unpause();
	}
}

void SceneManager::resumePrevScene()
{
	std::shared_ptr<Scene> tempScene = this->prevScene;
	if (this->currentScene != nullptr)
	{
		this->prevScene = this->currentScene;
		this->disableScene(this->currentScene->getSceneName());
	}

	this->currentScene = tempScene;
	this->enableScene(tempScene->getSceneName());

	this->currentScene->render();
}

std::shared_ptr<GameSelectScene> SceneManager::getGameSelectScene()
{
	return this->gameSelectScene;
}

std::shared_ptr<Scene> SceneManager::getCurrentScene()
{
	return this->currentScene;
}

std::shared_ptr<Scene> SceneManager::getPrevScene()
{
	return this->prevScene;
}

void SceneManager::initalizeScene(bool isActive, std::string sceneName)
{
	this->sceneDictionary[isActive][sceneName]->initialize();
}

void SceneManager::disablePauseBtn()
{
	this->disablePause = true;
}
void SceneManager::enablePauseBtn()
{
	this->disablePause = false;
}