#pragma once
#include "Scenes\Scene.h"
#include "Scenes\SceneManager.h"
#include "Player\Player.h"
#include "Player\PlayerBuilder.h"
#include "Factories\EnemyFactory.h"
#include "UserInterface.h"
#include <SDL.h>
#include <memory>

class AsteroidsScene : public Scene {
	public:
		AsteroidsScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		~AsteroidsScene() override;

		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void initialize() override;
		void restart() override;
		void updateRects() override;

	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<UserInterface> userInterface;
		PlayerBuilder playerBuilder;
		EnemyFactory enemyFactory;
};