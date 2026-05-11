#pragma once
#include <SDL.h>
#include "Graphics\Texture.h"
#include "Graphics\TextObject.h"
#include "Scenes\Scene.h"
#include "Scenes\SceneManager.h"
#include <string>
#include <memory>

class MainMenu : public Scene
{
	public:
		MainMenu(std::shared_ptr<SceneManager> sceneManager, std::string sceneName, std::shared_ptr<Scene> nextScene);
		~MainMenu() override;

		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void initialize() override;
		void restart() override;
		void setGame(std::string game);
		void updateRects() override;

	private:
		std::shared_ptr<Scene> nextScene;
		std::string gameName;

		std::shared_ptr<TextObject> mainTitle;
		std::shared_ptr<TextObject> options;
		std::shared_ptr<TextObject> start;
		std::shared_ptr<TextObject> studioName;
		std::shared_ptr<TextObject> copywrite;
};