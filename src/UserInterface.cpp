#include "UserInterface.h"
#include "Graphics\Texture.h"
#include "GameLogic\HelperFunctions.h"
#include "Utils\Stopwatch.h"
#include "Player\Player.h"
#include <iostream>
#include <string>

UserInterface::UserInterface(std::string gameTitle, int stocks, std::shared_ptr<Player> player, std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<SettingsManager> settingsManager)
{
	this->player = player;
	this->mStocks = stocks;
	this->graphicsManager = graphicsManager;
	this->settingsManager = settingsManager;
	//this->currentDisplaySettings = //this->settingsManager->getCurrentSettings().display;
	this->currentScreenSize = { 0,0,0,0 };
	this->font = "heavydata";
	this->margin = 20;
	this->game = gameTitle;
	this->fontColor = { 255, 255, 255, 0 };
	this->stockRect = { 0,0,0,0 };
	this->gameTitleTextObj = std::make_shared<TextObject>();
	this->scoreTextObj = std::make_shared<TextObject>();
	this->timeTextObj = std::make_shared<TextObject>();
}

UserInterface::~UserInterface()
{
	//this->mGameTitleText.free();
}

void UserInterface::intialize()
{
	std::string formatedTime = FormatUint32(this->gameTime->getTicks());
	this->currentScreenSize = this->graphicsManager->getScreenRect();
	this->gameTitleTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), this->game);
	this->gameTitleTextObj->setPosition((this->currentScreenSize.w / 2) - (this->gameTitleTextObj->getTextRect()->w / 2), margin);

	this->stockRect = { margin, this->currentScreenSize.h - this->player->getSpriteSheet().getSpriteHeight() - (margin + 50), this->player->getSpriteSheet().getSpriteWidth(), this->player->getSpriteSheet().getSpriteHeight() };
}

void UserInterface::update()
{
	// Timer
	std::string formatedTime = FormatUint32(this->gameTime->getTicks());
	std::string scoreText = std::format("Score: {}", player->getScore());
	std::string timeText = std::format("Time: {}", formatedTime);

	this->scoreTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), scoreText);
	this->timeTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), timeText);

	this->scoreTextObj->setPosition(margin, margin);
	this->timeTextObj->setPosition(this->currentScreenSize.w - this->timeTextObj->getBoxRect().w - margin, margin);

	this->mStocks = this->player->getStocks();
}

void UserInterface::render()
{
	this->graphicsManager->renderText(this->gameTitleTextObj->getCurrentTexture(), this->gameTitleTextObj->getTextRect());
	this->graphicsManager->renderText(this->scoreTextObj->getCurrentTexture(), this->scoreTextObj->getTextRect());
	this->graphicsManager->renderText(this->timeTextObj->getCurrentTexture(), this->timeTextObj->getTextRect());


	for (int i = 0; i < mStocks; i++)
	{
		int xPos = this->stockRect.x + (this->stockRect.w * i);

		SDL_FRect currentstockRect = { xPos, this->stockRect.y, this->stockRect.w, this->stockRect.h };
		this->graphicsManager->renderTextureEx(this->player->getSpriteSheet().getSheetTexture(), this->player->getSpriteSheet().getBaseRect(), &currentstockRect, 0, SDL_FLIP_NONE);
	}
}

void UserInterface::setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager)
{
	this->graphicsManager = graphicsManager;
}

void UserInterface::setGameTime(std::shared_ptr<Stopwatch> gameTime)
{
	this->gameTime = gameTime;
}


void UserInterface::updateScreenSize()
{
	this->currentScreenSize = this->graphicsManager->getScreenRect();
	SDL_Rect currentBoxRect;

	currentBoxRect = this->gameTitleTextObj->getBoxRect();
	this->graphicsManager->scaleRect(currentBoxRect);
	this->gameTitleTextObj->setBoxRect(currentBoxRect);

	currentBoxRect = this->scoreTextObj->getBoxRect();
	this->graphicsManager->scaleRect(currentBoxRect);
	this->scoreTextObj->setBoxRect(currentBoxRect);

	currentBoxRect = this->timeTextObj->getBoxRect();
	this->graphicsManager->scaleRect(currentBoxRect);
	this->timeTextObj->setBoxRect(currentBoxRect);

	SDL_Rect tempRect = stockRect;
	this->graphicsManager->scaleRect(tempRect);
	this->stockRect = tempRect;
}