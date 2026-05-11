#pragma once
#include "Graphics\Texture.h"
#include "Graphics\GraphicsManager.h"
#include "Utils\Stopwatch.h" 
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <memory>

class Player;
class UserInterface
{
	public:
		UserInterface(std::string gameTitle, int stocks, std::shared_ptr<Player> player, std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<SettingsManager> settingsManager);
		~UserInterface();
		void intialize();
		void update();
		void render();
		void setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager);
		void setGameTime(std::shared_ptr<Stopwatch> gameTime);
		void updateScreenSize();
	private:
		std::shared_ptr<Stopwatch> gameTime;
		std::string game;
		int margin;
		std::string font;
		SDL_Color fontColor;
		std::shared_ptr<GraphicsManager> graphicsManager;
		std::shared_ptr<SettingsManager> settingsManager;
		//DisplaySettings currentDisplaySettings;
		SDL_Rect currentScreenSize;
		std::shared_ptr<Player> player;
		int mStocks;

		std::shared_ptr<TextObject> gameTitleTextObj;

		std::shared_ptr<TextObject> scoreTextObj;

		std::shared_ptr<TextObject> timeTextObj;

		SDL_Rect stockRect;
};