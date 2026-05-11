#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class InputObject
{
	public:
		enum class Type {Number, Text};
		InputObject(Type inputType);
		~InputObject(); 
		bool loadFontTexture(SDL_Renderer* renderer, TTF_Font* font, std::string text);

		SDL_Rect getInputBoxRect();
		SDL_Rect* getTextRect();
		void setInputBoxRect(SDL_Rect inputBoxRect);

	private:
		Type inputType;
		std::map<int, SDL_Texture*> textures;
		SDL_Rect inputBoxRect;
		SDL_Rect textRect;
		std::string value;
};