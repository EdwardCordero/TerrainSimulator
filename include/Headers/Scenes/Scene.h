#pragma once
#include "Graphics\Texture.h"
#include "Physics\RigidBody.h"
#include "Physics\CollisionManager.h"
#include "GameLogic\InputHandler.h"
#include "Settings\SettingsManager.h"
#include "Graphics\GraphicsManager.h"
#include "Utils\Stopwatch.h"
#include <SDL.h>
#include <vector>

class SceneManager;
class Scene {
	protected:
		std::shared_ptr<SceneManager> sceneManager;
		std::shared_ptr<CollisionManager> mCollisionManager;
		std::shared_ptr<InputHandler> inputHandler;
		std::shared_ptr<SettingsManager> settingsManager;
		std::shared_ptr<GraphicsManager> graphicsManager;
		std::shared_ptr<Stopwatch> gameTime;
		bool mActive;
		bool mInitialized;
		std::vector<std::shared_ptr<Texture>> textures;
		int screenWidth;
		int screenHeight;
		int margin;
		std::string sceneName;

		std::string defaultFont;
		std::string defaultTextColorName;
		SDL_Color defaultTextColor;
		std::string hoverTextColorName;
		SDL_Color hoverTextColor;

	public:
		Scene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		virtual ~Scene() = 0;
		virtual void handleEvents(SDL_Event& event) = 0;
		virtual void update(const Uint8* keyState) = 0;
		virtual void render() = 0;
		virtual void display() = 0;
		virtual void hide() = 0;
		virtual void initialize() = 0;
		virtual void restart() = 0;
		virtual void updateRects() = 0;
		bool isInitailzed();
		void setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager);
		void setCollisionManager(std::shared_ptr<CollisionManager> collisionManager);
		void setInputHandler(std::shared_ptr<InputHandler> inputHandler);
		void setSettingsManager(std::shared_ptr<SettingsManager> settingsManager);
		void setGameTime(std::shared_ptr<Stopwatch> gameTime);
		void setInitialize(bool isInitialized);
		bool isActive();
		std::string getSceneName();
		void setSceneName(std::string sceneName);
};