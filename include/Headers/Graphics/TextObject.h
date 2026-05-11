#pragma once
#include "Graphics\Texture.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class TextObject
{
	public:
		enum class Alignment { Left, Center, Right };
		TextObject();
		~TextObject();
		void initalizeValues();
		bool loadFontTexture(SDL_Renderer* renderer, TTF_Font* font, std::string text, bool wrap = false, int wrapLength = 0);
		
		SDL_Texture* getCurrentTexture();
		SDL_Rect getBoxRect();
		SDL_Rect* getTextRect();
		SDL_Color getBoxColor();

		void setBoxRect(SDL_Rect newRect);
		TextObject& setFillColor(SDL_Color fillColor);
		void setStyleColor(std::string styleName, SDL_Color styleColor);
		void setFill(bool fill);
		void setPosition(int x, int y);
		bool isHoveredOver(int mouseX, int mouseY, SDL_Rect hoverHitBox = { 0,0,0,0 });
		bool isFilled();
		std::string getText();
	private:
		SDL_Texture* currentTexture;
		std::map<const std::string, SDL_Texture*> textures;
		SDL_Rect boxRect;
		SDL_Rect textRect;
		std::string text;
		SDL_Color fillColor;
		std::map<std::string, SDL_Color> styles;
		Alignment aligned;
		bool wrap;
		int wrapLength;
		bool hovered;
		bool filled;
};