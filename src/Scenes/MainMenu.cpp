#include "Scenes\MainMenu.h"
#include "GameLogic\HelperFunctions.h"
#include "Scenes\AsteroidsScene.h"
#include "Scenes\DefenderScene.h"
#include "Scenes\SceneManager.h"
#include "Graphics\Texture.h"
#include "Scenes\Scene.h"
#include <string>
#include <memory>

MainMenu::MainMenu(std::shared_ptr<SceneManager> sceneManager, std::string sceneName, std::shared_ptr<Scene> nextScene) : Scene(sceneManager, sceneName)
{
	this->defaultFont = "heavydata";
	this->nextScene = nextScene;

	this->mainTitle = nullptr;
	this->options = nullptr;
	this->start = nullptr;
	this->studioName = nullptr;
	this->copywrite = nullptr;
}

MainMenu::~MainMenu() {}

void MainMenu::setGame(std::string title)
{
	this->gameName = title;
	if (title == "Asteroids")
	{
		this->nextScene = std::make_shared<AsteroidsScene>(this->sceneManager, "AsteroidsScene");
		this->sceneManager->addGameplayScene(this->nextScene);
		//this->sceneManager->addScene(this->nextScene);
	}
	else
	{
		this->nextScene = std::make_shared<DefenderScene>(this->sceneManager, "DefenderScene");
		this->sceneManager->addGameplayScene(this->nextScene);
	}

	this->mainTitle = std::make_shared<TextObject>();
	this->mainTitle->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), gameName + "title");
}

void MainMenu::initialize()
{
	this->start = std::make_shared<TextObject>();
	this->start->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "START GAME");

	this->options = std::make_shared<TextObject>();
	this->options->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "OPTIONS");

	this->studioName = std::make_shared<TextObject>();
	this->studioName->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "2025 UNCANNY STUDIOS");

	this->copywrite = std::make_shared<TextObject>();
	this->copywrite->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "ALL RIGHTS RESERVED");

	//this->updateRects();
}

void MainMenu::restart()
{

}

void MainMenu::handleEvents(SDL_Event& event)
{
	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (this->start->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "Start Game was clicked" << std::endl;
			this->sceneManager->changeScene(this->nextScene);
			this->gameTime->start();
		}
		if (this->options->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "Options was clicked" << std::endl;
		}
	}
}

void MainMenu::update(const Uint8* keyState)
{
	this->updateRects();
}

void MainMenu::render()
{
	this->graphicsManager->preRender();

	this->graphicsManager->renderText(this->mainTitle->getCurrentTexture(), this->mainTitle->getTextRect());
	this->graphicsManager->renderText(this->start->getCurrentTexture(), this->start->getTextRect());
	this->graphicsManager->renderText(this->options->getCurrentTexture(), this->options->getTextRect());
	this->graphicsManager->renderText(this->studioName->getCurrentTexture(), this->studioName->getTextRect());
	this->graphicsManager->renderText(this->copywrite->getCurrentTexture(), this->copywrite->getTextRect());

	this->graphicsManager->render();
}

void MainMenu::display()
{
	this->mActive = true;
}

void MainMenu::hide()
{
	this->mActive = false;
}

void MainMenu::updateRects()
{
	SDL_Rect currentSceneRect = this->graphicsManager->getScreenRect();
	this->screenWidth = currentSceneRect.w;
	this->screenHeight = currentSceneRect.h;

	int mainTitleX = getCenterX(this->screenWidth, this->mainTitle->getBoxRect().w);
	int mainTitleY = this->screenHeight / 10;

	int startX = getCenterX(this->screenWidth, this->start->getBoxRect().w);
	int startY = (this->screenHeight / 2) - this->start->getBoxRect().h;

	int optionsX = getCenterX(this->screenWidth, this->options->getBoxRect().w);
	int optionsY = (this->screenHeight / 2) - this->options->getBoxRect().h + this->start->getBoxRect().h + 5;

	int studioNameX = getCenterX(this->screenWidth, this->studioName->getBoxRect().w);
	int studioNameY = (this->screenHeight - this->studioName->getBoxRect().h) - 40;

	int copywriteX = getCenterX(this->screenWidth, this->copywrite->getBoxRect().w);
	int copywriteY = (this->screenHeight - this->copywrite->getBoxRect().h) - 10;

	this->mainTitle->setPosition(mainTitleX, mainTitleY);
	this->start->setPosition(startX, startY);
	this->options->setPosition(optionsX, optionsY);
	this->studioName->setPosition(studioNameX, studioNameY);
	this->copywrite->setPosition(copywriteX, copywriteY);
}