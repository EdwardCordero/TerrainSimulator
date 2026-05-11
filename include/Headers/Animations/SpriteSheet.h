#pragma once
#include "Graphics\Texture.h"
#include <string>
#include <SDL.h>
#include <iostream>

class SpriteSheet {
	public:
		SpriteSheet();
		SpriteSheet(std::string imgPath, int spriteSheetWidth, int spriteSheetHeight, int totalSpriteCount);
		~SpriteSheet();

		bool initalizeSheetTexture(SDL_Renderer* renderer);

		std::shared_ptr<Texture> getSheetTexture();
		int getCurrentSprite();
		std::string getImgPath();
		int getSpriteWidth();
		int getSpriteHeight();
		int getTotalSpriteCount();
		SDL_Rect* getSpriteRect();
		SDL_Rect* getBaseRect();

		void setCurrentSprite(int sprite);
	private:
		std::shared_ptr<Texture> sheetTexture;
		std::string imgPath;
		int spriteSheetWidth;
		int spriteSheetHeight;
		int spriteWidth;
		int spriteHeight;
		int totalSpriteCount;
		int totalAnimationDuration;
		int currentSprite;
		SDL_Rect spriteRect;
		SDL_Rect baseRect;
};