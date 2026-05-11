#pragma once
#include <SDL.h>
#include "Player\Player.h"
#include "UserInterface.h"
#include "Scenes\Scene.h"
#include "Factories\EnemyFactory.h"
#include "Player\PlayerBuilder.h"
#include "GameLogic\InputHandler.h"
#include <memory>

class DefenderScene : public Scene {
public:
	DefenderScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
	~DefenderScene() override;

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