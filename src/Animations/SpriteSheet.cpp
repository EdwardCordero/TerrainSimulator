#include "Animations\SpriteSheet.h"
#include "Graphics\Texture.h"
#include <format>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

SpriteSheet::SpriteSheet() {
	this->imgPath = "";	
	this->spriteSheetHeight = 0;
	this->spriteSheetWidth = 0;
	this->totalSpriteCount = 0;
	this->spriteWidth = 0;
	this->spriteHeight = 0;
	this->currentSprite = 0;
	this->totalAnimationDuration = 1;
	this->sheetTexture = std::make_shared<Texture>();
	this->spriteRect = { (0 + spriteWidth * currentSprite), 0, spriteWidth, spriteHeight };
	this->baseRect = { 0, 0, spriteWidth, spriteHeight };
}

SpriteSheet::SpriteSheet(std::string imgPath, int spriteSheetWidth, int spriteSheetHeight, int totalSpriteCount)
{
	this->imgPath = imgPath;
	this->currentSprite = 0;
	this->spriteSheetHeight = spriteSheetHeight;
	this->spriteSheetWidth = spriteSheetWidth;
	this->totalSpriteCount = totalSpriteCount;
	this->totalAnimationDuration = 1;
	this->sheetTexture = std::make_shared<Texture>();

	this->spriteWidth = spriteSheetWidth / totalSpriteCount;
	this->spriteHeight = spriteSheetHeight;
	this->spriteRect = { (0 + spriteWidth * currentSprite), 0, spriteWidth, spriteHeight };
	this->baseRect = { 0, 0, spriteWidth, spriteHeight };
}

SpriteSheet::~SpriteSheet()
{
	//std::cout << "Closing sprite sheet" << this->imgPath << std::endl;
}

bool SpriteSheet::initalizeSheetTexture(SDL_Renderer* renderer)
{
	return this->sheetTexture->loadImgTexture(this->imgPath, renderer);
}

SDL_Rect* SpriteSheet::getSpriteRect()
{
	return &this->spriteRect;
}

SDL_Rect* SpriteSheet::getBaseRect()
{
	return &this->baseRect;
}

std::string SpriteSheet::getImgPath()
{
	return this->imgPath;
}
int SpriteSheet::getSpriteWidth()
{
	return this->spriteWidth;
}
int SpriteSheet::getSpriteHeight()
{
	return this->spriteHeight;
}
int SpriteSheet::getCurrentSprite()
{
	return this->currentSprite;
}
int SpriteSheet::getTotalSpriteCount()
{
	return this->totalSpriteCount;
}

std::shared_ptr<Texture> SpriteSheet::getSheetTexture()
{
	return this->sheetTexture;
}

void SpriteSheet::setCurrentSprite(int sprite)
{
	this->currentSprite = sprite;
	this->spriteRect = { (0 + spriteWidth * currentSprite), 0, spriteWidth, spriteHeight };
}

