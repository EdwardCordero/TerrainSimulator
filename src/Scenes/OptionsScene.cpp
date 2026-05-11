#include "Scenes\OptionsScene.h"
#include "Graphics\Texture.h"
#include "Scenes\SceneManager.h"
#include "GameLogic\HelperFunctions.h"
#include "GameLogic\GameSystem.h"
#include "Scenes\GameSelectScene.h"
#include "Utils\Stopwatch.h"
#include "globalConstants.h"
#include <SDL.h>
#include <string>
#include <memory>

static SDL_Rect getRectForOption(SDL_Rect parentRect, SDL_Rect rectTemplate, int textWidth, int optionNumber, int margin)
{
	return { parentRect.x + (rectTemplate.w * optionNumber) + margin, rectTemplate.y, rectTemplate.w, rectTemplate.h};
}

OptionsScene::OptionsScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : Scene(sceneManager, sceneName)
{
	this->mActive = false;
	this->margin = 20;

	this->optionsMenuRect = { 0,0,0,0 };
	this->optionsMenuRectColor = { 0,0,0,0 };
	this->currentScreenSize = { 0,0,0,0 };
	this->settingsRect = { 0,0,0,0 };

	this->buttonMargin = 20;
	this->buttonWidth = 0;
	this->buttonHeight = 0;
	this->buttonRect = { 0,0,0,0 };
	this->tabRect = { 0,0,0,0 };
	this->tabColor = { 34, 35, 36, 200 };
	
	this->optionsTitleTexObj = nullptr;
	this->applySettingsTextObj = nullptr;
	this->cancelSettingsTextObj = nullptr;
	this->resetSettingsTextObj = nullptr;
	this->adjustScreenBtn = nullptr;
}

OptionsScene::~OptionsScene()
{

}

void OptionsScene::initialize()
{
	this->mInitialized = true;
	this->selectedSetting = "display";
	this->optionsTitleTexObj = std::make_shared<TextObject>();
	this->optionsTitleTexObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Options");
	this->applySettingsTextObj = std::make_shared<TextObject>();
	this->applySettingsTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Apply Settings");
	this->cancelSettingsTextObj = std::make_shared<TextObject>();
	this->cancelSettingsTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Cancel");
	this->resetSettingsTextObj = std::make_shared<TextObject>();
	this->resetSettingsTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Reset Settings");

	// Read json setting options such as display, audio, etc... and load their fonts
	json jsonSettings = this->settingsManager->getCurrentSettings();
	int tabIndex = 0;
	for (const auto& [tabName, tabSettings] : jsonSettings.items())
	{
		std::shared_ptr<TextObject> newTextObj = std::make_shared<TextObject>();
		newTextObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), tabName);
		newTextObj->setFillColor(tabColor).setFill(true);
		this->settingTabTextures[tabName] = newTextObj;
		tabIndex++;
		int settingIndex = 0;
		for (const auto& [setting, value] : tabSettings.items())
		{
			std::shared_ptr<TextObject> newSettingText = std::make_shared<TextObject>();
			newSettingText->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), setting + ": ");

			std::shared_ptr<TextObject> settingValueText = std::make_shared<TextObject>();
			settingValueText->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), jsonToString(value));
			settingValueText->setFillColor({ tabColor.r, tabColor.g, tabColor.b, 255 }).setFill(true);

			this->settingTextures[tabName][setting] = newSettingText;
			this->settingTextures[tabName][setting + "_value"] = settingValueText;
			settingIndex++;
		}
	}

	// Buttons
	this->adjustScreenBtn = std::make_shared<Button>();
	this->adjustScreenBtn->textObj->loadFontTexture(this->graphicsManager->getRenderer(), this->graphicsManager->getDefaultFont(), "Adjust Screen Size");
	this->adjustScreenBtn->textObj->setFillColor({ 0, 255, 150, 100 }).setFill(true);

	this->adjustScreenScene = std::make_shared<AdjustScreenScene>(this->sceneManager, "AdjustScreenSize");
	this->sceneManager->addScene(this->adjustScreenScene);

	auto sm = this->sceneManager;
	auto scene = this->adjustScreenScene;
	this->adjustScreenBtn->setOnClick([sm, scene]() {
		if (!scene->isInitailzed())
		{
			scene->initialize();
		}
		else {
			scene->restart();
		}
		sm->enableScene(scene->getSceneName());
	});

	std::vector<std::string> screenSizes = { "2560x1440", "1920x1080" };
	int dropdownWidth = this->adjustScreenBtn->textObj->getBoxRect().w;
	SDL_Rect dropdownRect = { this->adjustScreenBtn->textObj->getBoxRect().x + this->adjustScreenBtn->textObj->getBoxRect().w + 10, this->adjustScreenBtn->textObj->getBoxRect().y, dropdownWidth, this->adjustScreenBtn->textObj->getBoxRect().h};
	this->screenSizesList = std::make_shared<ButtonList>(this->graphicsManager, dropdownRect, screenSizes);

	this->updateRects();
}

void OptionsScene::restart()
{
	this->selectedSetting = "display";
	this->updateRects();

}

void OptionsScene::updateRects()
{
	this->currentScreenSize = this->graphicsManager->getScreenRect();
	this->optionsMenuRect = this->currentScreenSize;

	// 3 bcs I only have 3 btns, might make this a class variable we'll see
	int numOfBtns = 3;
	this->buttonWidth = (this->currentScreenSize.w - ((numOfBtns * 2) * buttonMargin)) / numOfBtns;
	this->buttonHeight = (this->currentScreenSize.h - ((numOfBtns * 2) * buttonMargin)) / 8;
	// this is almost like a sprite sheet, maybe use that class instead?
	this->buttonRect = { this->currentScreenSize.x + buttonMargin, this->currentScreenSize.h - buttonMargin - buttonHeight, buttonWidth, buttonHeight };

	int optionsX = getCenterX(this->optionsMenuRect.w, this->optionsTitleTexObj->getBoxRect().w);
	int optionsY = margin;
	this->optionsTitleTexObj->setPosition(optionsX, optionsY);
	// button height to take into account tab height since they are going to be the same
	int tabHeight = this->optionsTitleTexObj->getTextRect()->h;
	int settingsRectY = (this->optionsTitleTexObj->getBoxRect().y + this->optionsTitleTexObj->getBoxRect().h) + tabHeight;
	this->settingsRect = { this->buttonRect.x, settingsRectY, this->optionsMenuRect.w - this->buttonRect.x, this->buttonRect.y - settingsRectY };
	int tabWidth = this->settingsRect.w / 10;
	this->tabRect = { this->settingsRect.x, this->settingsRect.y - tabHeight, tabWidth, tabHeight };

	int resetBtnRectX = getCenterX(this->optionsMenuRect.w, this->resetSettingsTextObj->getBoxRect().w);
	int applyBtnRectX = this->optionsMenuRect.w - (this->buttonMargin * 2) - this->applySettingsTextObj->getBoxRect().w;
	this->cancelSettingsTextObj->setPosition(this->buttonRect.x, this->buttonRect.y);
	this->applySettingsTextObj->setPosition(applyBtnRectX, this->buttonRect.y);
	this->resetSettingsTextObj->setPosition(resetBtnRectX, this->buttonRect.y);

	SDL_Rect settingTextRect = { this->settingsRect.x + margin, this->settingsRect.y + margin, 0, 0 };
	int tabIndex = 0;
	for (auto& [tabName, tabTexture] : this->settingTabTextures)
	{
		SDL_Rect currentTabRect = { tabRect.x + (tabRect.w * tabIndex), tabRect.y, tabRect.w, tabRect.y };
		tabTexture->setBoxRect(currentTabRect);
		tabIndex++;
	}

	int settingIndex = 0;
	
	for (auto settingTextIT = this->settingTextures[this->selectedSetting].begin(); settingTextIT != this->settingTextures[this->selectedSetting].end(); settingTextIT++)
	{
		SDL_Rect newSettingTextRect = settingTextIT->second->getBoxRect();
		settingTextRect.y += (newSettingTextRect.h + margin);
		settingTextRect.h = newSettingTextRect.h;
		settingTextIT->second->setPosition(settingTextRect.x, settingTextRect.y);
		int adjustedXForText = newSettingTextRect.x + newSettingTextRect.w + margin;
		// set rect for value of setting
		settingTextIT++;
		settingTextIT->second->setPosition(adjustedXForText, settingTextRect.y);
	}

	// Buttons
	settingTextRect.y += settingTextRect.h;
	this->adjustScreenBtn->textObj->setPosition(settingTextRect.x, settingTextRect.y);

	//std::vector<std::string> screenSizes = { "2560x1440", "1920x1080" };
	int dropdownWidth = this->adjustScreenBtn->textObj->getBoxRect().w;
	SDL_Rect dropdownRect = { this->adjustScreenBtn->textObj->getBoxRect().x + this->adjustScreenBtn->textObj->getBoxRect().w + 10, this->adjustScreenBtn->textObj->getBoxRect().y, dropdownWidth, this->adjustScreenBtn->textObj->getBoxRect().h };
	this->screenSizesList->updateRect(dropdownRect);
}

void OptionsScene::handleEvents(SDL_Event& event)
{
	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (this->cancelSettingsTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
		{
			this->sceneManager->resumePrevScene();
			this->sceneManager->pause();
			this->sceneManager->enablePauseBtn();
		}
		if (this->applySettingsTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
		{

		}
		if (this->resetSettingsTextObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
		{

		}
		if (this->adjustScreenBtn->textObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
		{
			//this->sceneManager->disableScene(this->sceneName);
			//this->adjustScreenBtn->click();
			SDL_Rect btn = this->adjustScreenBtn->textObj->getBoxRect();

			this->adjustScreenBtn->textObj->setBoxRect({ btn.x, btn.y, btn.w - (16 * (btn.w/btn.h)), btn.h - (9 * (btn.h / btn.w)) });
			std::cout << "CLICKED" << std::endl;
		}
	}

	this->screenSizesList->handleEvents(event);
}

void OptionsScene::update(const Uint8* keyState)
{
	if (this->screenSizesList->changedValue)
	{
		std::cout << "Screen size has changed" << std::endl;
		std::string selectedSize = this->screenSizesList->getSelectedOptionText();
		std::vector<std::string> size = splitString(selectedSize, 'x');
		int width = std::stoi(size[0]);
		int height = std::stoi(size[1]);
		this->graphicsManager->setScreenRect(width, height);
		this->sceneManager->updateRects();
		this->screenSizesList->changedValue = false;
		//this->updateRects();
	}
}

void OptionsScene::render()
{
	this->graphicsManager->preRender();

	// Title
	//this->graphicsManager->renderFillRect(&this->optionsTitleTexObj->getBoxRect(), this->optionsTitleTexObj->getBoxColor());
	this->graphicsManager->renderText(this->optionsTitleTexObj->getCurrentTexture(), this->optionsTitleTexObj->getTextRect());
	// Low button row
	this->graphicsManager->renderText(this->cancelSettingsTextObj->getCurrentTexture(), this->cancelSettingsTextObj->getTextRect());
	this->graphicsManager->renderText(this->resetSettingsTextObj->getCurrentTexture(), this->resetSettingsTextObj->getTextRect());
	this->graphicsManager->renderText(this->applySettingsTextObj->getCurrentTexture(), this->applySettingsTextObj->getTextRect());
	auto t = this->adjustScreenBtn->textObj->getBoxRect();
	this->graphicsManager->renderFillRect(this->adjustScreenBtn->textObj->getTextRect(), this->adjustScreenBtn->textObj->getBoxColor());

	// tabs for current setting
	int tabIndex = 0;
	for (auto& [tabName, tabTexture] : this->settingTabTextures)
	{
		SDL_Rect currentTabRect = getRectForOption(this->settingsRect, this->tabRect, tabTexture->getBoxRect().w, tabIndex, 0);
		this->graphicsManager->renderFillRect(&currentTabRect, tabColor);
		this->graphicsManager->renderText(tabTexture->getCurrentTexture(), tabTexture->getTextRect());
	}

	this->settingsRect.w = (this->applySettingsTextObj->getBoxRect().x + this->applySettingsTextObj->getBoxRect().w) - this->buttonRect.x;
	this->graphicsManager->renderFillRect(&this->settingsRect, { tabColor.r, tabColor.g, tabColor.b, 90 });

	// render settings keys and values
	for (auto& [settingName, settingTexture] : this->settingTextures[this->selectedSetting])
	{
		if (settingTexture->isFilled())
		{
			SDL_Rect boxRect = settingTexture->getBoxRect();
			this->graphicsManager->renderFillRect(&boxRect, settingTexture->getBoxColor());
		}
		this->graphicsManager->renderText(settingTexture->getCurrentTexture(), settingTexture->getTextRect());
		//this->adjustScreenBtn->textObj->setBoxRect({ settingTexture->getBoxRect().x, settingTexture->getBoxRect().y + settingTexture->getBoxRect().h, adjustScreenBtn->textObj->getTextRect()->w, adjustScreenBtn->textObj->getTextRect()->h });
	}

	if (this->selectedSetting == "display")
	{
		SDL_Rect adjustBtnRect = adjustScreenBtn->textObj->getBoxRect();
		this->graphicsManager->renderText(adjustScreenBtn->textObj->getCurrentTexture(), adjustScreenBtn->textObj->getTextRect());
		this->screenSizesList->render();
	}

	this->graphicsManager->render();
}

void OptionsScene::display()
{
	mActive = true;
	this->gameTime->pause();
}

void OptionsScene::hide()
{
	mActive = false;
	this->gameTime->unpause();
}