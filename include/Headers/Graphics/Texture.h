#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL.h>
#include <SDL_ttf.h>

class Texture
{
	public:
		// Initializes variables
		Texture();
		// Deallocates memory
		~Texture();
		// Loads image at specified path
		bool loadImgTexture(std::string imgPath, SDL_Renderer* renderer);
		// Deallocates texture
		void free();

		// Getters
		std::string getTextureName();
		std::string getTexturePath();
		SDL_Texture* getTexture();
		// current position
		int getWidth();
		int getHeight();
		int getXPos();
		int getYPos();
		// prev position
		int getPrevWidth();
		int getPrevHeight();

		// Setters
		void setWidth(int newWidth);
		void setHeight(int newHeight);
		void setScale(float scaler);
		void setTextureName(std::string name);
		// helper functions
		bool hasChangedDimensions();
		void startFlicker(float fadeSpeed);
		void endFlicker();
		void fade(float gDeltaTime);
	private:
		std::string textureName;
		std::string texturePath;
		SDL_Texture* currentTexture;
		SDL_Color backgroundColor;
		SDL_Rect textureRect;
		SDL_Rect prevTextureRect;
		float scaler;
		bool flicker;
		Uint8 alpha;
		int fadeDirection;
		float fadeSpeed;
};