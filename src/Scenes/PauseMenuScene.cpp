#include "Scenes\PauseMenuScene.h"
#include "Graphics\Texture.h"
#include "Scenes\Scene.h"
#include "Scenes\SceneManager.h"
#include "GameLogic\HelperFunctions.h"
#include <SDL.h>

static SDL_Rect getRectForOption(SDL_Rect parentRect, SDL_Rect rectTemplate, int textWidth, int optionNumber, int margin)
{
	return { parentRect.x, rectTemplate.y + (rectTemplate.h * optionNumber) + margin, rectTemplate.w, rectTemplate.h};
}

PauseMenuScene::PauseMenuScene(std::shared_ptr<SceneManager> sceneManger, std::string sceneName) : Scene(sceneManger, sceneName)
{
	this->menuTextObject = nullptr;
	this->optionsTextObject = nullptr;
	this->gameSelectTextObject = nullptr;
	this->endGameTextObject = nullptr;
	this->quitTextObject = nullptr;
	this->resumeTextObject = nullptr;
	this->optionsScene = nullptr;
	this->menuOptionsMargin = 2;
	this->menuRect = { 0,0,0,0 };
	this->menuTitleRect = { 0,0,0,0 };
	this->menuOptionRect = { 0,0,0,0 };
	this->optionsRect = { 0,0,0,0 };
	this->gameSelectRect = { 0, 0, 0, 0 };
	this->endGameRect = { 0,0,0,0 };
	this->quitRect = { 0,0,0,0 };
	this->resumeRect = { 0,0,0,0 };
	this->currentScreenSize = { 0,0,0,0 };
	this->menuTitleRectColor = {130, 91, 33, 255};
	this->menuRectColor = { 184, 142, 79, 60 };
}

PauseMenuScene::~PauseMenuScene()
{

}

void PauseMenuScene::initialize()
{
	this->mInitialized = true;
	SDL_Renderer* graphicsRenderer = this->graphicsManager->getRenderer();
	TTF_Font* defaultFont = this->graphicsManager->getDefaultFont();
	this->currentScreenSize = this->graphicsManager->getScreenRect();

	this->menuTextObject = std::make_shared<TextObject>();
	this->menuTextObject->loadFontTexture(graphicsRenderer, defaultFont, "Paused");
	this->optionsTextObject = std::make_shared<TextObject>();
	this->optionsTextObject->loadFontTexture(graphicsRenderer, defaultFont, "Options");
	this->gameSelectTextObject = std::make_shared<TextObject>();
	this->gameSelectTextObject->loadFontTexture(graphicsRenderer, defaultFont, "Game Select");
	this->endGameTextObject = std::make_shared<TextObject>();
	this->endGameTextObject->loadFontTexture(graphicsRenderer, defaultFont, "End Game");
	this->quitTextObject = std::make_shared<TextObject>();
	this->quitTextObject->loadFontTexture(graphicsRenderer, defaultFont, "Quit");
	this->resumeTextObject = std::make_shared<TextObject>();
	this->resumeTextObject->loadFontTexture(graphicsRenderer, defaultFont, "Resume");

	this->initializeRects();

	this->optionsScene = std::make_shared<OptionsScene>(this->sceneManager, "OptionsScene");
	this->sceneManager->addScene(this->optionsScene);
	this->optionsScene->initialize();
}

void PauseMenuScene::restart()
{
	this->initializeRects();
}

void PauseMenuScene::initializeRects()
{
	this->currentScreenSize = this->graphicsManager->getScreenRect();
	int menuWidth = this->currentScreenSize.w / 4;
	int menuHeight = this->currentScreenSize.h / 3;
	this->menuRect = { getCenterX(this->currentScreenSize.w, menuWidth), getCenterY(this->currentScreenSize.h, menuHeight), menuWidth, menuHeight };

	this->menuOptionRect = { this->menuRect.x, this->menuRect.y, this->menuRect.w, (this->menuRect.h - this->menuTitleRect.h) / 6 };
	this->menuTitleRect = getRectForOption(this->menuRect, this->menuOptionRect, this->menuTextObject->getTextRect()->w, 0, 0);
	this->menuTextObject->setBoxRect(menuTitleRect);

	this->optionsRect = getRectForOption(this->menuRect, this->menuOptionRect, this->optionsTextObject->getTextRect()->w, 1, this->menuOptionsMargin);
	this->optionsTextObject->setBoxRect(optionsRect);

	this->gameSelectRect = getRectForOption(this->menuRect, this->menuOptionRect, this->gameSelectTextObject->getTextRect()->w, 2, this->menuOptionsMargin);
	this->gameSelectTextObject->setBoxRect(gameSelectRect);

	this->endGameRect = getRectForOption(this->menuRect, this->menuOptionRect, this->endGameTextObject->getTextRect()->w, 3, this->menuOptionsMargin);
	this->endGameTextObject->setBoxRect(endGameRect);

	this->quitRect = getRectForOption(this->menuRect, this->menuOptionRect, this->quitTextObject->getTextRect()->w, 4, this->menuOptionsMargin);
	this->quitTextObject->setBoxRect(this->quitRect);

	this->resumeRect = getRectForOption(this->menuRect, this->menuOptionRect, this->resumeTextObject->getTextRect()->w, 5, this->menuOptionsMargin);
	this->resumeTextObject->setBoxRect(this->resumeRect);
}

void PauseMenuScene::updateRects()
{
	this->initializeRects();
}

void PauseMenuScene::handleEvents(SDL_Event& event)
{

	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (this->optionsTextObject->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			this->sceneManager->disablePauseBtn();
			this->sceneManager->disableScene(this->sceneName);
			this->sceneManager->changeScene(this->optionsScene);
			//this->sceneManager->enableScene(this->optionsScene->getSceneName());
		}
		if (this->gameSelectTextObject->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			//this->sceneManager->disableScene(this->sceneName);
			this->sceneManager->unpause();
			this->sceneManager->changeScene(this->sceneManager->getDefaultScene());
		}
		if (this->endGameTextObject->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "ending game" << std::endl;
			this->sceneManager->changeScene(this->sceneManager->getDefaultScene());
			this->sceneManager->unpause();
		}
		if (this->quitTextObject->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			this->sceneManager->setRunning(false);
		}
		if (this->resumeTextObject->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			this->sceneManager->unpause();
		}
	}
}

void PauseMenuScene::update(const Uint8* keyState)
{

}

void PauseMenuScene::render()
{
	this->graphicsManager->renderFillRect(&this->menuRect, this->menuRectColor);
	this->graphicsManager->renderFillRect(&this->menuTitleRect, this->menuTitleRectColor);

	this->graphicsManager->renderText(this->menuTextObject->getCurrentTexture(), this->menuTextObject->getTextRect());
	this->graphicsManager->renderText(this->optionsTextObject->getCurrentTexture(), this->optionsTextObject->getTextRect());
	this->graphicsManager->renderText(this->gameSelectTextObject->getCurrentTexture(), this->gameSelectTextObject->getTextRect());
	this->graphicsManager->renderText(this->endGameTextObject->getCurrentTexture(), this->endGameTextObject->getTextRect());
	this->graphicsManager->renderText(this->quitTextObject->getCurrentTexture(), this->quitTextObject->getTextRect());
	this->graphicsManager->renderText(this->resumeTextObject->getCurrentTexture(), this->resumeTextObject->getTextRect());

	this->graphicsManager->render();
}

void PauseMenuScene::display()
{
	mActive = true;
}

void PauseMenuScene::hide()
{
	mActive = false;
}