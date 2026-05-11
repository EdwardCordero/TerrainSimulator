#include "GameLogic\GameSystem.h"
#include "Graphics\Texture.h"
#include "Graphics\GraphicsManager.h"
#include "GlobalConstants.h"
#include "Factories\EnemyFactory.h"
#include "Scenes\SceneManager.h"
#include "Scenes\DefenderScene.h"
#include "Scenes\GameSelectScene.h"
#include <iostream>
#include <vector>
#include <memory>
#include <SDL.h>
#include <Settings\SettingsManager.h>

using namespace std;
int main(int argc, char* args[]) {
	SDL_Event e;
	std::shared_ptr<SettingsManager> settingsManager = std::make_shared<SettingsManager>();
	std::shared_ptr<GraphicsManager> graphicsManager = std::make_shared<GraphicsManager>();
	if(!graphicsManager->initialize(settingsManager))
	{
		cout << "Failed to initialize Graphics Manager! " << endl;
	}
	else
	{
		Uint32 lastTime = SDL_GetTicks();
		// player input
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		// Sets scene to main menu, main menu changes scene to game in handleEvents
		std::shared_ptr<SceneManager> sceneManager = std::make_shared<SceneManager>();
		int s = 12;
		sceneManager->initalize(sceneManager, graphicsManager, settingsManager);
		while (sceneManager->isRunning())
		{
			Uint32 frameStart = SDL_GetTicks();
			// EVENTS
			while (SDL_PollEvent(&e) != 0)
			{
				sceneManager->handleEvents(e);
			}

			// player input
			keyState = SDL_GetKeyboardState(nullptr);

			// UPDATE
			sceneManager->update(keyState);

			// Get time for frame
			Uint32 currentTime = SDL_GetTicks();
			graphicsManager->setDeltaTime((currentTime - lastTime) / 1000.0f);
			lastTime = currentTime;

			// RENDER
			sceneManager->render();

			// Anchor FPS
			Uint32 frameTime = SDL_GetTicks() - frameStart;
			if (frameTime < graphicsManager->getFrameDelay())
			{
				SDL_Delay(graphicsManager->getFrameDelay() - frameTime);
			}
		}
	}
	graphicsManager->~GraphicsManager();
	return 0;
}