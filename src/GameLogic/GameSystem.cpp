#include "GameLogic\GameSystem.h"
#include "GlobalConstants.h"
#include <iostream>
#include <format>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

bool init()
{
	bool success = true;

	//if (SDL_Init(SDL_INIT_VIDEO) < 0)
	//{
	//	std::cout << std::format("SDL could not initialize! SDL_Error: {}", SDL_GetError()) << std::endl;
	//	success = false;
	//}
	//else
	//{
	//	gWindow = SDL_CreateWindow("Paradox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, SDL_WINDOW_SHOWN);
	//	if (gWindow == NULL)
	//	{
	//		std::cout << std::format("Window could not be created! SDL_Erorr: {}", SDL_GetError()) << std::endl;
	//		success = false;
	//	}
	//	else
	//	{
	//		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	//		if (gRenderer == NULL)
	//		{
	//			std::cout << std::format("Renderer could not be created! SDL_Error: {}", SDL_GetError());

	//			success = false;
	//		}
	//		else
	//		{
	//			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	//			SDL_RenderClear(gRenderer);
	//			SDL_RenderPresent(gRenderer);
	//			int imgFlags = IMG_INIT_JPG;
	//			if (!(IMG_Init(imgFlags) & imgFlags))
	//			{
	//				std::cout << std::format("SDL_image could not initialize! SDL_image ERROR: {}", IMG_GetError()) << std::endl;
	//				success = false;
	//			}
	//			else
	//			{
	//				gScreenSurface = SDL_GetWindowSurface(gWindow);
	//			}
	//			if (TTF_Init() == -1)
	//			{
	//				std::cout << std::format("SDL_ttf could not initialize!, SDL_ttf Error: {}", TTF_GetError()) << std::endl;
	//				success = false;
	//			}

	//			// Get the display mode
	//			SDL_DisplayMode displayMode;
	//			if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
	//				std::cerr << "SDL_GetCurrentDisplayMode Error: " << SDL_GetError() << std::endl;
	//				SDL_Quit();
	//				return 1;
	//			}

	//			// Get the width and height of the screen
	//			int screenWidth = displayMode.w;
	//			int screenHeight = displayMode.h;

	//			std::cout << "Screen width: " << screenWidth << std::endl;
	//			std::cout << "Screen height: " << screenHeight << std::endl;
	//		}
	//	}
	//}

	return success;
}

void close()
{
	/*if (gScreenSurface != NULL)
	{
		SDL_FreeSurface(gScreenSurface);
		gScreenSurface = NULL;
	}
	if (gTexture != NULL)
	{
		SDL_DestroyTexture(gTexture);
		gTexture = NULL;
	}
	if (gWindow != NULL)
	{
		SDL_DestroyWindow(gWindow);
		SDL_DestroyRenderer(gRenderer);
		gWindow = NULL;
		gRenderer = NULL;
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();*/
}