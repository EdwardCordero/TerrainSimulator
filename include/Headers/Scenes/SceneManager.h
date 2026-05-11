#pragma once
#include <SDL.h>
#include "Scenes\Scene.h"
#include "Scenes\LeaderboardScene.h"
#include "Scenes\OptionsScene.h"
#include "Physics\CollisionManager.h"
#include "GameLogic\InputHandler.h"
#include "Settings\SettingsManager.h"
#include "Graphics\GraphicsManager.h"
#include "Scenes\GameSelectScene.h"
#include "Scenes\PauseMenuScene.h"
#include "Utils\Stopwatch.h"
#include <memory>
#include <map>
#include <vector>

class SceneManager {
	public:
		SceneManager();
		~SceneManager();
		void initalize(std::shared_ptr<SceneManager> self, std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<SettingsManager> settingsManager);
		void changeScene(std::shared_ptr<Scene> newScene);
		void pause();
		void unpause();
		void handleEvents(SDL_Event& event);
		void update(const Uint8* keyState);
		void render();
		bool isRunning();
		void setRunning(bool isRunning);
		std::shared_ptr<Stopwatch> getGameTime();
		void changeToGameSelectScene();
		void resumePrevScene();

		std::vector<std::shared_ptr<Scene>> getActiveScenes();
		bool addScene(std::shared_ptr<Scene> newScene);
		bool addGameplayScene(std::shared_ptr<Scene> gameplayScene);
		void disableScene(std::string sceneName);
		void enableScene(std::string sceneName);
		void startGameTime();
		void disablePauseBtn();
		void enablePauseBtn();
		void updateRects();

		std::shared_ptr<Scene> getPrevScene();
		std::shared_ptr<Scene> getCurrentScene();
		std::shared_ptr<GameSelectScene> getGameSelectScene();
		void initalizeScene(bool isActive, std::string sceneName);
	private:
		std::shared_ptr<CollisionManager> collisionManager;
		std::shared_ptr<InputHandler> inputHandler;
		std::shared_ptr<SettingsManager> settingsManager;
		std::shared_ptr<GraphicsManager> graphicsManager;

		std::map<bool, std::map<std::string, std::shared_ptr<Scene>>> sceneDictionary;
		std::shared_ptr<Scene> currentScene;
		std::shared_ptr<Scene> prevScene;
		std::shared_ptr<Scene> gameplayScene;
		std::shared_ptr<GameSelectScene> gameSelectScene;
		std::shared_ptr<PauseMenuScene> pauseMenuScene;

		bool isPaused;
		bool running;
		bool disablePause;
		std::shared_ptr<Stopwatch> gameTime;
};