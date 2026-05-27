#pragma once
#include "Scenes\Scene.h"
#include "Graphics\Shapes.h"
#include "Scenes\SceneManager.h"
#include <SDL.h>
#include <memory>

class SimulatorScene : public Scene {
	public:
        SimulatorScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
        ~SimulatorScene() override;
        void handleEvents(SDL_Event& event) override;
        void update(const Uint8* keyState) override;
        void render() override;
        void display() override;
        void hide() override;
        void initialize() override;
        void restart() override;
        void updateRects() override;
    private:
        bool isActive;
        SDL_Renderer* renderer;
        std::vector<std::shared_ptr<Circle>> nodes;
};