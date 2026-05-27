#include "Graphics\GraphicsManager.h"
#include "Physics\RigidBody.h"
#include "GameLogic\HelperFunctions.h"
#include <iostream>
#include <fstream>
#include <format>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <string>
#include <nlohmann\json.hpp>

using json = nlohmann::json;

GraphicsManager::GraphicsManager()
{
	this->gTimeDeltaSeconds = 0;
	this->gWindow = nullptr;
	this->gRenderer = nullptr;
	this->gScreenSurface = nullptr;
	this->gCurrentSurface = nullptr;
	this->settingsManager = nullptr;
	this->targetFrameRate = 60;
	this->frameDelay = 1000 / targetFrameRate;
	this->currentScreenRect = { 0,0,0,0 };
	this->defaultFontName = "heavydata";
	this->xScaler = 1;
	this->yScaler = 1;
	this->xScalerOld = 1;
	this->yScalerOld = 1;
}

GraphicsManager::~GraphicsManager()
{
	if (gScreenSurface != NULL)
	{
		SDL_FreeSurface(gScreenSurface);
		gScreenSurface = NULL;
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
	SDL_Quit();
}

bool GraphicsManager::initialize(std::shared_ptr<SettingsManager> settingsManager)
{
	this->settingsManager = settingsManager;
	this->setScreenRect(this->settingsManager->getCurrentSettings().display.width, this->settingsManager->getCurrentSettings().display.height);
	this->initializeWindow();
	this->initializeRenderer();

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);
	SDL_RenderPresent(gRenderer);
	int imgFlags = IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << std::format("SDL_image could not initialize! SDL_image ERROR: {}", IMG_GetError()) << std::endl;
		return false;
	}
	else
	{
		gScreenSurface = SDL_GetWindowSurface(gWindow);
	}
	if (TTF_Init() == -1)
	{
		std::cout << std::format("SDL_ttf could not initialize!, SDL_ttf Error: {}", TTF_GetError()) << std::endl;
		return false;
	}	

	//SDL_SetWindowFullscreen(this->gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	this->loadTexturePaths();
	return true;
}

bool GraphicsManager::initializeWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << std::format("SDL could not initialize! SDL_Error: {}", SDL_GetError()) << std::endl;
		return false;
	}
	SDL_Rect usableBounds;
	SDL_GetDisplayUsableBounds(0, &usableBounds);
	int screenWidth = usableBounds.w;
	int screenHeight = usableBounds.h;
	
	this->gWindow = SDL_CreateWindow("Paradox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		std::cout << std::format("Window could not be created! SDL_Erorr: {}", SDL_GetError()) << std::endl;
		return false;
	}

	return true;
}

bool GraphicsManager::initializeRenderer()
{
	this->gRenderer = SDL_CreateRenderer(this->gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		std::cout << std::format("Renderer could not be created! SDL_Error: {}", SDL_GetError());
		return false;
	}
	return true;
}

bool GraphicsManager::loadTexturePaths()
{
	this->textureResourcePath = std::string(DATA_PATH) + "textures.json"; //this->settingsManager->getOSPath() + "textures.json";
	json data;

	std::ifstream file(textureResourcePath);
	if (!file.is_open())
	{
		std::cout << "ERROR: Could not open the JSON file: " << textureResourcePath << std::endl;
		return false;
	}

	try {
		file >> data;
	}
	catch (const json::parse_error& e)
	{
		std::cout << "ERROR parsing JSON: " << e.what() << std::endl;
		return false;
	}

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		const std::string& categoryName = it.key();
		const json& categoryArray = it.value();

		if (categoryArray.is_array())
		{
			for (const auto& entry : categoryArray)
			{
				if (entry.is_object())
				{
					for (auto const& [name, path] : entry.items())
					{
						this->texturePaths[name] = std::string(RESOURCES_PATH) + path.get<std::string>();
						if (categoryName == "Fonts")
						{
							initializeFont(name, std::string(RESOURCES_PATH) + path.get<std::string>(), 50);
						}
					}
				}
				else
				{
					std::cout << "Warning: Array entry in category '" << categoryName << "' is not an object." << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Warning: Top-level item '" << categoryName << "' is not an array" << std::endl;
		}
	}

	return true;
}

// Should be able to set fontsize to big number and then scale down the texture
bool GraphicsManager::initializeFont(std::string fontName, std::string path, int fontSize)
{
	TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
	if (font == nullptr) {
		std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
		return false;
	}

	loadedFonts[fontName] = font;
	return true;
}

void GraphicsManager::setWindowFullScreen()
{
	SDL_SetWindowFullscreen(this->gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
}
void GraphicsManager::setWindowBordered(SDL_bool bordered)
{
	SDL_SetWindowBordered(this->gWindow, bordered);
}

void GraphicsManager::parseJSONArray(const json& data, std::string arrayName)
{
	if (data.contains(arrayName) && data[arrayName].is_array())
	{
		for (const auto& imagePath : data[arrayName])
		{
			if (imagePath.is_object())
			{
				for (auto const& [name, path] : imagePath.items())
				{
					texturePaths[name] = std::string(RESOURCES_PATH) + path.get<std::string>();
				}
			}
		}
	}
}

void GraphicsManager::setSettingsManager(std::shared_ptr<SettingsManager> settingsManager)
{
	this->settingsManager = settingsManager;
	this->setScreenRect(this->settingsManager->getCurrentSettings().display.width, this->settingsManager->getCurrentSettings().display.height);
}

void GraphicsManager::setScreenRect(int newWidth, int newHeight)
{
	SDL_SetWindowSize(this->gWindow, newWidth, newHeight);
	this->xScalerOld = this->xScaler;
	this->yScalerOld = this->yScaler;

	this->currentScreenRect.w = newWidth;
	this->currentScreenRect.h = newHeight;

	this->xScaler = (float)this->currentScreenRect.w / this->settingsManager->getCurrentSettings().display.baseWidth;
	this->yScaler = (float)this->currentScreenRect.h / this->settingsManager->getCurrentSettings().display.baseHeight;
}


void GraphicsManager::cacheTexture(std::string textureName)
{
	std::string path = this->texturePaths[textureName];

	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
	newTexture->loadImgTexture(path, this->gRenderer);

	this->cachedTextures[textureName] = newTexture;
}

SDL_Rect GraphicsManager::getScreenRect()
{
	return this->currentScreenRect;
}

SDL_Rect GraphicsManager::getWindowRect()
{
	int width;
	int height;
	SDL_GetWindowSize(gWindow, &width, &height);

	return {0,0,width, height};
}

std::shared_ptr<Texture> GraphicsManager::getCachedTexture(std::string textureName)
{
	return this->cachedTextures[textureName];
}

void GraphicsManager::preRender()
{
	SDL_SetRenderDrawColor(this->gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(this->gRenderer);

	SDL_RenderSetScale(this->gRenderer, 1.0f, 1.0f);
}

void GraphicsManager::render()
{
	SDL_RenderPresent(this->gRenderer);
}

void GraphicsManager::renderTexture(std::shared_ptr<Texture> texture, const SDL_Rect* renderQuad)
{
	SDL_RenderCopy(gRenderer, texture->getTexture(), NULL, renderQuad);
	return;
}

void GraphicsManager::renderTexture(std::shared_ptr<Texture> texture, const SDL_FRect* renderQuad)
{
	SDL_RenderCopyF(gRenderer, texture->getTexture(), NULL, renderQuad);
	return;
}

void GraphicsManager::renderText(SDL_Texture* texture, const SDL_Rect* renderQuad)
{
	SDL_RenderCopy(gRenderer, texture, NULL, renderQuad);
}

void GraphicsManager::renderTextureEx(std::shared_ptr<Texture> texture, const SDL_Rect* srcQuad, const SDL_FRect* renderQuad, float rotateDeg, SDL_RendererFlip flip)
{
	SDL_RenderCopyExF(gRenderer, texture->getTexture(), srcQuad, renderQuad, rotateDeg, NULL, flip);
	return;
}

void GraphicsManager::renderDrawPoint(int x, int y, SDL_Color color)
{
	setDrawColor(color);
	SDL_RenderDrawPoint(gRenderer, x, y);
	return;
}

void GraphicsManager::renderFillRect(const SDL_Rect* rect, SDL_Color fillColor)
{
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(this->gRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	SDL_RenderFillRect(this->gRenderer, rect);
	return;
}

void GraphicsManager::renderFillCircle(std::shared_ptr<Circle> circle)
{
	int radius = circle->radius;
	int centerX = circle->centerX;
	int centerY = circle->centerY;
	std::vector<SDL_Point> points;
	for(int y = radius * -1; y <= radius; y++)
	{
		for(int x = radius * -1; x <= radius; x++)
		{
			if(x*x + y*y <= radius*radius)
			{
				// SDL_RenderDrawPoint(gRenderer,
				// 					centerX + x,
				// 					centerY + y);
				SDL_Point point = {centerX + x, centerY + y};
				points.push_back(point);
			}
		}
	}
	SDL_RenderDrawPoints(gRenderer, points.data(), points.size());
}

void GraphicsManager::renderDrawPoints(SDL_Point* points, int size)
{
	SDL_RenderDrawPoints(this->gRenderer, points, size);
}

void GraphicsManager::scaleRect(SDL_FRect& renderQuad)
{
	// reset to original before scaling
	renderQuad.x /= this->xScalerOld;
	renderQuad.w /= this->xScalerOld;

	renderQuad.y /= this->yScalerOld;
	renderQuad.h /= this->yScalerOld;


	renderQuad.x *= this->xScaler;
	renderQuad.w *= this->xScaler;

	renderQuad.y *= this->yScaler;
	renderQuad.h *= this->yScaler;
}
void GraphicsManager::scaleRect(SDL_Rect& renderQuad)
{
	renderQuad.x /= this->xScalerOld;
	renderQuad.w /= this->xScalerOld;

	renderQuad.y /= this->yScalerOld;
	renderQuad.h /= this->yScalerOld;

	renderQuad.x *= this->xScaler;
	renderQuad.w *= this->xScaler;

	renderQuad.y *= this->yScaler;
	renderQuad.h *= this->yScaler;
}

void GraphicsManager::setDrawColor(SDL_Color drawColor)
{
	SDL_SetRenderDrawColor(this->gRenderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	return;
}

SDL_Renderer* GraphicsManager::getRenderer()
{
	return this->gRenderer;
}

float GraphicsManager::getDeltaTime()
{
	return this->gTimeDeltaSeconds;
}

int GraphicsManager::getFrameRate()
{
	return this->targetFrameRate;
}

int GraphicsManager::getFrameDelay()
{
	return this->frameDelay;
}

void GraphicsManager::setDeltaTime(float deltaTime)
{
	this->gTimeDeltaSeconds = deltaTime;
}

TTF_Font* GraphicsManager::getDefaultFont()
{
	return this->loadedFonts[this->defaultFontName];
}

std::string GraphicsManager::getTexturePath(std::string textureName)
{
	return this->texturePaths[textureName];
};

TTF_Font* GraphicsManager::getFont(std::string fontName)
{
	return this->loadedFonts[fontName];
}