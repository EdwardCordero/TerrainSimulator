#pragma once
#include "Scenes\Scene.h"
#include "Graphics\ButtonList.h"
#include "Graphics\Texture.h"
#include "Graphics\Button.h"
#include "Utils\Stopwatch.h"
#include "Scenes\AdjustScreenScene.h"
#include "Physics\GameObject.h"
#include <SDL.h>
#include <string>
#include <memory>
#include <map>

class SceneManager;
class OptionsScene : public Scene
{
	public:
		OptionsScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		~OptionsScene() override;

		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void initialize() override;
		void restart() override;
		void updateRects() override;
	private:
		std::shared_ptr<AdjustScreenScene> adjustScreenScene;

		SDL_Rect optionsMenuRect;
		SDL_Color optionsMenuRectColor;

		std::shared_ptr<TextObject> optionsTitleTexObj;

		std::string selectedSetting;
		SDL_Rect settingsRect;
		SDL_Rect tabRect;
		SDL_Color tabColor;
		std::map<std::string, std::shared_ptr<TextObject>> settingTabTextures;
		// 1st str key : 
		std::map<std::string, std::map<std::string, std::shared_ptr<TextObject>>> settingTextures;

		// for settings menu
		std::shared_ptr<TextObject> cancelSettingsTextObj;
		std::shared_ptr<TextObject> applySettingsTextObj;
		std::shared_ptr<TextObject> resetSettingsTextObj;
		std::shared_ptr<Button> adjustScreenBtn;
		std::shared_ptr<ButtonList> screenSizesList;

		SDL_Rect buttonRect;
		int buttonMargin;
		int buttonWidth;
		int buttonHeight;

		// use this as parent screen, I'm using this for menu but can be used for any screen size just change this
		SDL_Rect currentScreenSize;
};