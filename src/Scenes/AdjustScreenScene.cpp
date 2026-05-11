#include "Scenes\Scene.h"
#include "Graphics\Button.h"
#include "Scenes\AdjustScreenScene.h"
#include "GameLogic\HelperFunctions.h"
#include "Scenes\SceneManager.h"
#include <SDL.h>
#include <memory>

AdjustScreenScene::AdjustScreenScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : Scene(sceneManager, sceneName)
{
	this->verticalRect = { 0, 0, 20, 40 };
	this->horizontalRect = { 0, 0, 40, 20 };
	this->btnFillColor = { 184, 142, 79, 60 };
	this->arrowFillColor = { 255,255,255,255 };
	this->screenSize = { 0,0,0,0 };
	this->confirmBtn = nullptr;
	this->cancelBtn = nullptr;
}

void AdjustScreenScene::initialize()
{
	this->confirmBtn = std::make_shared<Button>();
	this->cancelBtn = std::make_shared<Button>();

	this->confirmBtn->textObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Confirm Scree Size");
	this->cancelBtn->textObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Cancel");
	this->confirmBtn->textObj->setPosition(getCenterX(this->settingsManager->getCurrentSettings().display.width, this->confirmBtn->textObj->getTextRect()->w), getCenterY(this->settingsManager->getCurrentSettings().display.height, this->confirmBtn->textObj->getTextRect()->h));
	this->cancelBtn->textObj->setPosition(getCenterX(this->settingsManager->getCurrentSettings().display.width, this->cancelBtn->textObj->getTextRect()->w), this->confirmBtn->textObj->getBoxRect().y + this->confirmBtn->textObj->getBoxRect().h );

	this->confirmBtn->textObj->setFillColor(this->btnFillColor).setFill(true);
	this->cancelBtn->textObj->setFillColor(this->btnFillColor).setFill(true);

	auto sm = this->sceneManager;
	this->cancelBtn->setOnClick([sm]() {
		sm->enableScene(sm->getCurrentScene()->getSceneName());
	});
	mInitialized = true;


}

void AdjustScreenScene::restart()
{
	this->confirmBtn->textObj->setPosition(getCenterX(this->settingsManager->getCurrentSettings().display.width, this->confirmBtn->textObj->getTextRect()->w), getCenterY(this->settingsManager->getCurrentSettings().display.height, this->confirmBtn->textObj->getTextRect()->h));
	this->cancelBtn->textObj->setPosition(getCenterX(this->settingsManager->getCurrentSettings().display.width, this->cancelBtn->textObj->getTextRect()->w), this->confirmBtn->textObj->getBoxRect().y + this->confirmBtn->textObj->getBoxRect().h);

	this->confirmBtn->textObj->setFillColor(this->btnFillColor).setFill(true);
	this->cancelBtn->textObj->setFillColor(this->btnFillColor).setFill(true);
}

void AdjustScreenScene::updateRects()
{

}

AdjustScreenScene::~AdjustScreenScene()
{

}

void AdjustScreenScene::handleEvents(SDL_Event& event)
{
	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (this->confirmBtn->textObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
		{
			this->confirmBtn->click();
		}
		if (this->cancelBtn->textObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
		{
			this->sceneManager->disableScene(this->sceneName);
			this->cancelBtn->click();
		}
	}
}

void AdjustScreenScene::update(const Uint8* keyState)
{
	
}

void AdjustScreenScene::render()
{
	this->graphicsManager->preRender();

	this->graphicsManager->renderText(this->confirmBtn->textObj->getCurrentTexture(), this->confirmBtn->textObj->getTextRect());
	SDL_Rect confirmRect = this->confirmBtn->textObj->getBoxRect();
	this->graphicsManager->renderFillRect(&confirmRect, this->confirmBtn->textObj->getBoxColor());

	this->graphicsManager->renderText(this->cancelBtn->textObj->getCurrentTexture(), this->cancelBtn->textObj->getTextRect());
	SDL_Rect cancelRect = this->cancelBtn->textObj->getBoxRect();
	this->graphicsManager->renderFillRect(&cancelRect, this->cancelBtn->textObj->getBoxColor());

	SDL_Rect tempVRect = this->verticalRect;
	SDL_Rect tempHRect = this->horizontalRect;
	// Top Left
	tempVRect.x = 0;
	tempVRect.y = 0;
	tempHRect.x = 0;
	tempHRect.y = 0;
	this->graphicsManager->renderFillRect(&tempVRect, this->arrowFillColor);
	this->graphicsManager->renderFillRect(&tempHRect, this->arrowFillColor);

	// Top Right
	tempVRect.x = this->settingsManager->getCurrentSettings().display.width - tempVRect.w;
	tempVRect.y = 0;
	tempHRect.x = this->settingsManager->getCurrentSettings().display.width - tempHRect.w;;
	tempHRect.y = 0;
	this->graphicsManager->renderFillRect(&tempVRect, this->arrowFillColor);
	this->graphicsManager->renderFillRect(&tempHRect, this->arrowFillColor);

	// Bottom Left
	tempVRect.x = 0;
	tempVRect.y = this->settingsManager->getCurrentSettings().display.height - tempVRect.h;
	tempHRect.x = 0;
	tempHRect.y = this->settingsManager->getCurrentSettings().display.height - tempHRect.h;
	this->graphicsManager->renderFillRect(&tempVRect, this->arrowFillColor);
	this->graphicsManager->renderFillRect(&tempHRect, this->arrowFillColor);

	// Bottom Right
	tempVRect.x = this->settingsManager->getCurrentSettings().display.width - tempVRect.w;
	tempVRect.y = this->settingsManager->getCurrentSettings().display.height - tempVRect.h;
	tempHRect.x = this->settingsManager->getCurrentSettings().display.width - tempHRect.w;;
	tempHRect.y = this->settingsManager->getCurrentSettings().display.height - tempHRect.h;
	this->graphicsManager->renderFillRect(&tempVRect, this->arrowFillColor);
	this->graphicsManager->renderFillRect(&tempHRect, this->arrowFillColor);

	this->graphicsManager->render();
}

void AdjustScreenScene::display()
{
	this->mActive = true;
}

void AdjustScreenScene::hide()
{
	this->mActive = false;
}