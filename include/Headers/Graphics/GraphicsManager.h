#pragma once
#include "Settings\SettingsManager.h"
#include "Physics\RigidBody.h"
#include "Graphics\Texture.h"
#include "Graphics\TextObject.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <nlohmann\json.hpp>

using json = nlohmann::json;

class GraphicsManager
{
	public:
		GraphicsManager();
		~GraphicsManager();
		SDL_Renderer* getRenderer();
		float getDeltaTime();
		int getFrameRate();
		int getFrameDelay();
		void setDeltaTime(float deltaTime);

		bool initialize(std::shared_ptr<SettingsManager> settingsManager);
		bool initializeFont(std::string fontName, std::string path, int fontSize);
		bool loadTexturePaths();
		void setWindowFullScreen();
		void setWindowBordered(SDL_bool bordered);

		void preRender();
		void render();
		void renderTexture(std::shared_ptr<Texture> texture, const SDL_Rect* renderQuad);
		void renderTexture(std::shared_ptr<Texture> texture, const SDL_FRect* renderQuad);
		void renderText(SDL_Texture* texture, const SDL_Rect* renderQuad);
		void renderTextureEx(std::shared_ptr<Texture> texture, const SDL_Rect* srcQuad, const SDL_FRect* renderQuad, float rotateDeg, SDL_RendererFlip flip);
		void renderDrawPoint(int x, int y, SDL_Color color);
		void renderFillRect(const SDL_Rect* rect, SDL_Color fillColor);
		void scaleRect(SDL_FRect& renderQuad);
		void scaleRect(SDL_Rect& renderQuad);

		void setDrawColor(SDL_Color drawColor);
		void parseJSONArray(const json& data, std::string arrayName);

		void setSettingsManager(std::shared_ptr<SettingsManager> settingsManager);
		void setScreenRect(int newWidth, int newHeight);
		void cacheTexture(std::string textureName);

		SDL_Rect getScreenRect();
		TTF_Font* getFont(std::string fontName);
		TTF_Font* getDefaultFont();
		std::string getTexturePath(std::string textureName);
		std::shared_ptr<Texture> getCachedTexture(std::string textureName);

	private:
		bool initializeWindow();
		bool initializeRenderer();

		SDL_Window* gWindow;
		SDL_Renderer* gRenderer;
		SDL_Surface* gScreenSurface;
		SDL_Surface* gCurrentSurface;

		int targetFrameRate;
		int frameDelay;
		float gTimeDeltaSeconds;
		float xScaler;
		float yScaler;
		float xScalerOld;
		float yScalerOld;

		std::string textureResourcePath;
		std::string defaultFontName;

		std::shared_ptr<SettingsManager> settingsManager;
		std::map<std::string, std::shared_ptr<Texture>> cachedTextures;
		std::map<std::string, TTF_Font*> loadedFonts;
		std::map<std::string, std::string> texturePaths;
		SDL_Rect currentScreenRect;
};