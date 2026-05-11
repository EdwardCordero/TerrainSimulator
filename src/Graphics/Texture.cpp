#include "Graphics\Texture.h"
#include "GameLogic\HelperFunctions.h"
#include <format>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

Texture::Texture()
{
	currentTexture = NULL;
	this->textureRect = { 0,0,0,0 };
	this->prevTextureRect = { 0,0,0,0 };
	backgroundColor = { 0, 0, 0, 0 };
	scaler = 1;
	this->texturePath = "";
	this->flicker = false;
	this->fadeDirection = 1;
	this->alpha = 255;
	this->fadeSpeed = 0;
}

Texture::~Texture()
{
	std::cout << "Freeing Texture, with name: " << this->textureName << std::endl;
	free();
}

bool Texture::loadImgTexture(std::string imgPath, SDL_Renderer* renderer)
{
	if (this->currentTexture != nullptr)
	{
		SDL_DestroyTexture(this->currentTexture);
		this->currentTexture = nullptr;
	}

	// The final texture
	SDL_Texture* newTexture = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(imgPath.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << std::format("Unable to load image: {}!\n SDL ERROR: {}", imgPath, SDL_GetError()) << std::endl;
	}
	else
	{
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << std::format("Unable to create texture from {}! \n SDL ERROR: {}", imgPath, SDL_GetError()) << std::endl;
		}
		else
		{
			// Get img dimensions
			this->textureRect.w = loadedSurface->w;
			this->textureRect.h = loadedSurface->h;
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	currentTexture = newTexture;
	SDL_SetTextureBlendMode(this->currentTexture, SDL_BLENDMODE_BLEND);
	this->texturePath = imgPath;
	return currentTexture != NULL;
}

void Texture::free()
{
	if (currentTexture != NULL)
	{
		//std::cout << "Closing sprite sheet" << this->mName << std::endl;
		SDL_DestroyTexture(currentTexture);
		currentTexture = NULL;
		textureName = "";
	}
}

bool Texture::hasChangedDimensions()
{
	if (this->prevTextureRect.h != this->textureRect.h)
	{
		return true;
	}
	if (this->prevTextureRect.w != this->textureRect.w)
	{
		return true;
	}

	return false;
}

void Texture::startFlicker(float fadeSpeed)
{
	this->fadeSpeed = fadeSpeed;
	this->flicker = true;
	this->fadeDirection = 1;
	this->alpha = 255;
}

void Texture::endFlicker()
{
	this->flicker = false;
}

void Texture::fade(float gDeltaTime)
{
	this->alpha += fadeDirection * this->fadeSpeed;

	if (this->alpha <= 0)
	{
		this->alpha = 0;
		this->fadeDirection = 1;
	}
	else if (this->alpha >= 255)
	{
		this->alpha = 255;
		this->fadeDirection = -1;
	}

	SDL_SetTextureAlphaMod(this->currentTexture, alpha);
}

std::string Texture::getTextureName()
{
	return this->textureName;
}

int Texture::getWidth()
{
	return this->textureRect.w;
}
int Texture::getHeight()
{
	return this->textureRect.h;
}
int Texture::getXPos()
{
	return this->textureRect.x;
}
int Texture::getYPos()
{
	return this->textureRect.y;
}

int Texture::getPrevWidth()
{
	return this->prevTextureRect.w;
}
int Texture::getPrevHeight()
{
	return this->prevTextureRect.h;
}
SDL_Texture* Texture::getTexture()
{
	return currentTexture;
}
void Texture::setWidth(int newWidth)
{
	this->textureRect.w = newWidth;
}
void Texture::setHeight(int newHeight)
{
	this->textureRect.h = newHeight;
}

void Texture::setTextureName(std::string name)
{
	this->textureName = name;
}

void Texture::setScale(float scaler)
{
	this->scaler = scaler;
}
std::string Texture::getTexturePath()
{
	return this->texturePath;
}