#pragma once
#include "Scenes\Scene.h"
#include "Graphics\Button.h"
#include <SDL.h>
#include <memory>

class SceneManager;
class AdjustScreenScene : public Scene
{
	public:
		AdjustScreenScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		~AdjustScreenScene() override;

		void initialize() override;
		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void restart() override;
		void updateRects() override;

	private:
		SDL_Rect verticalRect;
		SDL_Rect horizontalRect;

		std::shared_ptr<Button> confirmBtn;
		std::shared_ptr<Button> cancelBtn;
		SDL_Color btnFillColor;
		SDL_Color arrowFillColor;
		SDL_Rect screenSize;
};