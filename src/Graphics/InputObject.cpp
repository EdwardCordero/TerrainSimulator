#include "Graphics\InputObject.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <format>

InputObject::InputObject(Type inputType)
{
	this->inputType = inputType;
	this->inputBoxRect = { 0,0,0,0 };
	this->textRect = { 0,0,0,0 };
	this->value = "";
}

InputObject::~InputObject()
{
	for (auto [key, texture] : textures)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

bool InputObject::loadFontTexture(SDL_Renderer* renderer, TTF_Font* font, std::string text)
{
	SDL_Color defaultColor = { 255, 255, 255, 0 };
	if (this->textures.size() > 0)
	{
		for (auto [key, texture] : this->textures)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	int charIndex = 0;
	for (char textChar : text)
	{
		SDL_Surface* loadedSurface = NULL;
		SDL_Texture* newTexture = NULL;

		loadedSurface = TTF_RenderText_Blended(font, &textChar, defaultColor);

		if (loadedSurface == NULL)
		{
			std::cout << std::format("Unable to render '{}' for text object! SDL_ttf Error: {}", text, TTF_GetError()) << std::endl;
			return false;
		}
		else
		{
			newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
			if (newTexture == NULL)
			{
				std::cout << std::format("Unable to render '{}' surface! SDL_ttf Error: {}", text, TTF_GetError()) << std::endl;
				return false;
			}
			else
			{
				this->textRect.w += loadedSurface->w;
				this->textRect.h = loadedSurface->h;
			}

			SDL_FreeSurface(loadedSurface);
		}

		this->textures[charIndex] = newTexture;
	}

	this->setInputBoxRect(this->textRect);
	this->value = text;
	return true;
}

SDL_Rect InputObject::getInputBoxRect()
{
	return this->inputBoxRect;
}

SDL_Rect* InputObject::getTextRect()
{
	return &this->textRect;
}

void InputObject::setInputBoxRect(SDL_Rect inputBoxRect)
{
	this->inputBoxRect = inputBoxRect;
}
