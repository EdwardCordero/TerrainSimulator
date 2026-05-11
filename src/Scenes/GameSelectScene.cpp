#include "Scenes\GameSelectScene.h"
#include "Scenes\SceneManager.h"
#include "Scenes\DefenderScene.h"
#include "Scenes\MainMenu.h"
#include "Scenes\AsteroidsScene.h"
#include <memory>

GameSelectScene::GameSelectScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : Scene(sceneManager, sceneName)
{
	this->defaultFont = "heavydata";
	this->defaultTextColorName = "white";
	this->defaultTextColor = { 255, 255, 255, 255 };
	this->hoverTextColor = { 255, 0, 0, 255 };
	this->hoverTextColorName = "red";
	this->selectButtonHeight = 0;
	this->selectButtonWidth = 0;

	this->mainMenu = nullptr;

	this->spaceInvadersRect = { 0, 0, 0, 0 };
	this->pacManRect = { 0, 0, 0, 0 };
	this->asteroidsRect = { 0, 0, 0, 0 };
	this->tetrisRect = { 0, 0, 0, 0 };
	this->defenderRect = { 0, 0, 0, 0 };
}

GameSelectScene::~GameSelectScene() {};

void GameSelectScene::initialize()
{
	this->mInitialized = true;
	this->screenWidth = this->settingsManager->getCurrentSettings().display.width;
	this->screenHeight = this->settingsManager->getCurrentSettings().display.height;

	int margin = this->screenWidth / 20;
	SDL_Color buttonColor = { 255, 0, 0, 200 };
	this->selectButtonWidth = (this->screenWidth - (margin * 2)) / 3;
	this->selectButtonHeight = (this->screenHeight - (margin * 2)) / 3;
	this->mainMenu = std::make_shared<MainMenu>(this->sceneManager, "MainMenu", nullptr);
	this->sceneManager->addScene(this->mainMenu);

	this->spaceInvaderTextObj = std::make_shared<TextObject>();
	this->spaceInvaderTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Space Invaders");
	this->spaceInvaderTextObj->setFillColor(buttonColor).setFill(true);

	this->pacManTextObj = std::make_shared<TextObject>();
	this->pacManTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Pac-Man");
	this->pacManTextObj->setFillColor(buttonColor).setFill(true);

	this->asteroidsTextObj = std::make_shared<TextObject>();
	this->asteroidsTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Asteroids");
	this->asteroidsTextObj->setFillColor(buttonColor).setFill(true);

	this->tetrisTextObj = std::make_shared<TextObject>();
	this->tetrisTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Tetris");
	this->tetrisTextObj->setFillColor(buttonColor).setFill(true);

	this->defenderTextObj = std::make_shared<TextObject>();
	this->defenderTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Defender");
	this->defenderTextObj->setFillColor(buttonColor).setFill(true);

	this->calculateButtonRects();
	this->mCollisionManager->clearGrid();
}

void GameSelectScene::restart()
{
	this->screenWidth = this->settingsManager->getCurrentSettings().display.width;
	this->screenHeight = this->settingsManager->getCurrentSettings().display.height;

	//this->calculateButtonRects();
	this->mCollisionManager->clearGrid();
}

void GameSelectScene::handleEvents(SDL_Event& event)
{
	int margin = this->screenWidth / 20;
	int selectButtonWidth = (this->screenWidth - (margin * 2)) / 3;

	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool gameSelected = false;
		if (this->spaceInvaderTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "Space Invaders was selected" << std::endl;
			this->mainMenu->setGame("Space Invaders");
			gameSelected = true;
		}
		if (this->pacManTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "PacMan was selected" << std::endl;
			this->mainMenu->setGame("Pac-Man");
			gameSelected = true;
		}
		if (this->asteroidsTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "Asteroids was selected" << std::endl;
			this->mainMenu->setGame("Asteroids");
			gameSelected = true;
		}
		if (this->tetrisTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "Tetris was selected" << std::endl;
			this->mainMenu->setGame("Tetris");
			gameSelected = true;
		}
		if (this->defenderTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONUP)
		{
			std::cout << "Defender was selected" << std::endl;
			this->mainMenu->setGame("Defender");
			gameSelected = true;
		}

		if (gameSelected)
		{
			this->sceneManager->changeScene(this->mainMenu);
		}
	}
}

void GameSelectScene::update(const Uint8* keyState)
{
//	calculateButtonRects();
}

void GameSelectScene::updateRects()
{
	//	this->screenWidth = this->graphicsManager->getScreenRect().w;
	//	this->screenHeight = this->graphicsManager->getScreenRect().h;
	//
	//	//SDL_Rect currentRect = this->spaceInvadersRect;
	//	this->graphicsManager->scaleRect(spaceInvadersRect);
	//	this->spaceInvaderTextObj->setBoxRect(spaceInvadersRect);
	//
	//	this->graphicsManager->scaleRect(pacManRect);
	//	this->pacManTextObj->setBoxRect(pacManRect);
	//
	//	this->graphicsManager->scaleRect(asteroidsRect);
	//	this->asteroidsTextObj->setBoxRect(asteroidsRect);
	//
	//	this->graphicsManager->scaleRect(tetrisRect);
	//	this->tetrisTextObj->setBoxRect(tetrisRect);
	//
	//	this->graphicsManager->scaleRect(defenderRect);
	//	this->defenderTextObj->setBoxRect(defenderRect);
	//}
	this->calculateButtonRects();
}

void GameSelectScene::calculateButtonRects()
{
	this->screenWidth = this->graphicsManager->getScreenRect().w;
	this->screenHeight = this->graphicsManager->getScreenRect().h;

	int margin = this->screenWidth / 20;
	SDL_Color buttonColor = { 255, 0, 0, 200 };
	this->selectButtonWidth = (this->screenWidth - (margin * 2)) / 3;
	this->selectButtonHeight = (this->screenHeight - (margin * 2)) / 3;

	int spaceInvadersX = margin;
	int spaceInvadersY = margin;

	int pacmanX = this->screenWidth - selectButtonWidth - margin;
	int pacmanY = margin;

	int asteroidX = margin;
	int asteroidY = this->screenHeight - margin - selectButtonHeight;

	int tetrisX = (2 * selectButtonWidth) + margin;
	int tetrisY = this->screenHeight - margin - selectButtonHeight;

	int defenderX = margin + selectButtonWidth;
	int defenderY = margin + selectButtonHeight;

	this->spaceInvadersRect = { spaceInvadersX, spaceInvadersY, selectButtonWidth, selectButtonHeight };

	this->pacManRect = { pacmanX, pacmanY, selectButtonWidth, selectButtonHeight };

	this->asteroidsRect = { asteroidX, asteroidY, selectButtonWidth, selectButtonHeight };

	this->tetrisRect = { tetrisX, tetrisY, selectButtonWidth, selectButtonHeight };

	this->defenderRect = { defenderX, defenderY, selectButtonWidth, selectButtonHeight };

	this->spaceInvaderTextObj->setBoxRect(spaceInvadersRect);
	this->pacManTextObj->setBoxRect(pacManRect);
	this->asteroidsTextObj->setBoxRect(asteroidsRect);
	this->tetrisTextObj->setBoxRect(tetrisRect);
	this->defenderTextObj->setBoxRect(defenderRect);
}

void GameSelectScene::render()
{
	this->graphicsManager->preRender();

	this->graphicsManager->renderFillRect(&this->spaceInvadersRect, this->spaceInvaderTextObj->getBoxColor());
	this->graphicsManager->renderFillRect(&this->pacManRect, this->pacManTextObj->getBoxColor());
	this->graphicsManager->renderFillRect(&this->asteroidsRect, this->asteroidsTextObj->getBoxColor());
	this->graphicsManager->renderFillRect(&this->tetrisRect, this->tetrisTextObj->getBoxColor());
	this->graphicsManager->renderFillRect(&this->defenderRect, this->defenderTextObj->getBoxColor());

	this->graphicsManager->renderText(this->spaceInvaderTextObj->getCurrentTexture(), this->spaceInvaderTextObj->getTextRect());
	this->graphicsManager->renderText(this->pacManTextObj->getCurrentTexture(), this->pacManTextObj->getTextRect());
	this->graphicsManager->renderText(this->asteroidsTextObj->getCurrentTexture(), this->asteroidsTextObj->getTextRect());
	this->graphicsManager->renderText(this->tetrisTextObj->getCurrentTexture(), this->tetrisTextObj->getTextRect());
	this->graphicsManager->renderText(this->defenderTextObj->getCurrentTexture(), this->defenderTextObj->getTextRect());

	this->graphicsManager->render();
}

void GameSelectScene::display()
{
	this->mActive = true;
}

void GameSelectScene::hide()
{
	this->mActive = false;
}
