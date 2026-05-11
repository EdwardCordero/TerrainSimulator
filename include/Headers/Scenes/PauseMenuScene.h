#pragma once
#include "Graphics\Texture.h"
#include "Graphics\TextObject.h"
#include "Scenes\OptionsScene.h"
#include "Scenes\Scene.h"
#include <SDL.h>

class SceneManager;
class PauseMenuScene : public Scene
{
	public:
		PauseMenuScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		~PauseMenuScene() override;

		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void initialize() override;
		void restart() override;
		void updateRects() override;
		void initializeRects();

	private:
		std::shared_ptr<Scene> optionsScene;

		std::shared_ptr<TextObject> menuTextObject;
		std::shared_ptr<TextObject> optionsTextObject;
		std::shared_ptr<TextObject> gameSelectTextObject;
		std::shared_ptr<TextObject> endGameTextObject;
		std::shared_ptr<TextObject> quitTextObject;
		std::shared_ptr<TextObject> resumeTextObject;

		int menuOptionsMargin;
		SDL_Rect menuRect;
		SDL_Color menuRectColor;
		SDL_Rect menuTitleRect;
		SDL_Color menuTitleRectColor;

		SDL_Rect menuOptionRect;
		SDL_Rect currentScreenSize;

		SDL_Rect optionsRect;
		SDL_Rect gameSelectRect;
		SDL_Rect endGameRect;
		SDL_Rect quitRect;
		SDL_Rect resumeRect;
};