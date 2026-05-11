#include "Graphics\TextObject.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <format>

TextObject::TextObject()
{
	this->initalizeValues();
}

TextObject::~TextObject()
{

	for (auto [styleName, texture] : this->textures)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	SDL_DestroyTexture(this->currentTexture);
	this->currentTexture = nullptr;
}

void TextObject::initalizeValues()
{
	this->boxRect = { 0,0,0,0 };
	this->textRect = { 0,0,0,0 };
	this->text = "";
	this->currentTexture = nullptr;

	this->styles["Default"] = { 255, 255, 255, 0 };
	this->styles["Hover"] = { 255, 0, 0, 0 };
	this->aligned = Alignment::Center;
	this->wrap = false;
	this->wrapLength = 0;
	this->hovered = false;
	this->fillColor = {0, 0, 0, 0};
	this->filled = false;
}

bool TextObject::loadFontTexture(SDL_Renderer* renderer, TTF_Font* font, std::string text, bool wrap, int wrapLength)
{
	if (currentTexture != nullptr)
	{
		SDL_DestroyTexture(this->currentTexture);
		this->currentTexture = nullptr;

		for (auto [styleName, texture] : this->textures)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	for (const auto& [styleName, color] : this->styles)
	{
		SDL_Surface* loadedSurface = NULL;
		SDL_Texture* newTexture = NULL;

		if (!wrap)
		{
			loadedSurface = TTF_RenderText_Blended(font, text.c_str(), color);
		}
		else
		{
			loadedSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrapLength);
		}

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
				if (!wrap)
				{
					this->textRect.w = loadedSurface->w;
				}
				else
				{
					if (loadedSurface->w > wrapLength)
					{
						this->textRect.w = wrapLength;
					}
					else
					{
						this->textRect.w = loadedSurface->w;
					}
				}
				this->textRect.h = loadedSurface->h;
			}

			SDL_FreeSurface(loadedSurface);
		}

		this->textures[styleName] = newTexture;
		//SDL_DestroyTexture(newTexture);
	}

	// TODO: get rid of string here, should set to 0?
	this->currentTexture = textures["Default"];
	this->setBoxRect(this->textRect);
	this->text = text;
	return true;
}

SDL_Texture* TextObject::getCurrentTexture()
{
	return this->currentTexture;
}

SDL_Rect TextObject::getBoxRect()
{
	return this->boxRect;
}

SDL_Rect* TextObject::getTextRect()
{
	
	switch (this->aligned)
	{
	case Alignment::Left:
		this->textRect.x = this->boxRect.x;
		break;
	case Alignment::Center:
		this->textRect.x = (this->boxRect.x + this->boxRect.w/ 2) - (this->textRect.w / 2);
		break;
	case Alignment::Right:
		this->textRect.x = this->boxRect.w - this->textRect.w;
		break;
	}

	this->textRect.y = (this->boxRect.y + this->boxRect.h / 2) - (this->textRect.h / 2);

	return &this->textRect;
}

SDL_Color TextObject::getBoxColor()
{
	return this->fillColor;
}

void TextObject::setBoxRect(SDL_Rect newRect)
{
	this->boxRect = newRect;
	if (newRect.w < this->textRect.w)
	{
		this->textRect.w = newRect.w;
	}
	if (newRect.h < this->textRect.h)
	{
		this->textRect.h = newRect.h;
	}

	switch (this->aligned)
	{
		case Alignment::Left:
			this->textRect.x = this->boxRect.x;
			break;
		case Alignment::Center:
			this->textRect.x = (this->boxRect.x / 2) - (this->textRect.w / 2);
			break;
		case Alignment::Right:
			this->textRect.x = this->boxRect.w - this->textRect.w;
			break;
	}

	this->textRect.y = (this->boxRect.y / 2) - (this->textRect.h / 2);
}


TextObject& TextObject::setFillColor(SDL_Color fillColor)
{
	this->fillColor = fillColor;
	return *this;
}


void TextObject::setStyleColor(std::string styleName, SDL_Color styleColor)
{
	this->styles[styleName] = styleColor;
}

void TextObject::setFill(bool fill)
{
	this->filled = fill;
}

void TextObject::setPosition(int x, int y)
{
	//this->textRect = { x, y, this->textRect.w, this->textRect.h };
	SDL_Rect newRect = { x, y, this->boxRect.w, this->boxRect.h };
	// using function to allign
	this->setBoxRect(newRect);
}

bool TextObject::isHoveredOver(int mouseX, int mouseY, SDL_Rect hoverHitBox)
{
	if (SDL_RectEmpty(&hoverHitBox))
	{
		hoverHitBox = this->boxRect;
	}
	bool isHoveredOver = false;
	if ((mouseX >= hoverHitBox.x && mouseX <= hoverHitBox.x + hoverHitBox.w) && (mouseY >= hoverHitBox.y && mouseY <= hoverHitBox.y + hoverHitBox.h))
	{
		isHoveredOver = true;
		this->currentTexture = textures["Hover"];
	}
	else
	{
		isHoveredOver = false;
		this->currentTexture = textures["Default"];
	}

	return isHoveredOver;
}

bool TextObject::isFilled()
{
	return this->filled;
}

std::string TextObject::getText()
{
	return this->text;
}