#pragma once
#include "Scene.h"
#include "Graphics\Texture.h"
#include "Graphics\TextObject.h"
#include <SDL.h>
#include <memory>

class SceneManager;
class MainMenu;
class GameSelectScene : public Scene
{
	public:
		GameSelectScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		~GameSelectScene();

		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void initialize() override;
		void restart() override;
		void updateRects() override;

		void calculateButtonRects();

	private:
		std::shared_ptr<MainMenu> mainMenu;

		SDL_Rect spaceInvadersRect;
		std::shared_ptr<TextObject> spaceInvaderTextObj;
		SDL_Rect pacManRect;
		std::shared_ptr<TextObject> pacManTextObj;
		SDL_Rect asteroidsRect;
		std::shared_ptr<TextObject> asteroidsTextObj;
		SDL_Rect tetrisRect;
		std::shared_ptr<TextObject> tetrisTextObj;
		SDL_Rect defenderRect;
		std::shared_ptr<TextObject> defenderTextObj;

		int selectButtonWidth;
		int selectButtonHeight;
};