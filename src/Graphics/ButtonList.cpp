#include "Graphics\ButtonList.h"
#include "Graphics\Button.h"
#include "Graphics\GraphicsManager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <map>

ButtonList::ButtonList(std::shared_ptr<GraphicsManager> gm, SDL_Rect btnRect, std::vector<std::string> choices)
{
	this->currentIndex = 0;
	this->horizontal = false;
	this->listBtnRect = btnRect;
	this->optionRectColor = { 184, 142, 79, 255 };
	this->gm = gm;
	this->borderOn = true;;
	this->borderColor = { 255, 255, 255, 255 };
	this->borderThickness = 2;
	this->showOptions = false;
	this->changedValue = false;
	for (int i = 0; i < choices.size(); i++)
	{
		this->addOption(choices[i]);
	}
}

ButtonList::~ButtonList()
{

}

void ButtonList::addOption(std::string optionText)
{
	int index = buttons.size();

	std::shared_ptr<Button> newButton = std::make_shared<Button>();
	newButton->textObj->loadFontTexture(this->gm->getRenderer(), this->gm->getDefaultFont(), optionText);
	newButton->textObj->setFillColor(this->optionRectColor).setFill(true);
	newButton->setOnClick([this, index]() {
		this->updateSelectedOption(index);
	});
	newButton->textObj->setBoxRect({ this->listBtnRect.x, this->listBtnRect.y + (this->listBtnRect.h * (index + 1)), this->listBtnRect.w, this->listBtnRect.h});

	buttons[index] = newButton;
}

void ButtonList::removeOption(int choiceID)
{

}

void ButtonList::render()
{
	SDL_Rect currentBtnRect = { this->buttons[this->currentIndex]->textObj->getTextRect()->x, ((this->listBtnRect.y + this->listBtnRect.h) - (this->listBtnRect.h / 2)) - (this->buttons[this->currentIndex]->textObj->getTextRect()->h / 2), this->buttons[this->currentIndex]->textObj->getTextRect()->w, this->buttons[this->currentIndex]->textObj->getTextRect()->h};

	gm->renderFillRect(&this->listBtnRect, this->optionRectColor);
	gm->renderText(this->buttons[this->currentIndex]->textObj->getCurrentTexture(), &currentBtnRect);

	if (showOptions)
	{
		for (auto& [index, btn] : buttons)
		{
			SDL_Color btnColor = this->optionRectColor;
			if (index == this->currentIndex)
			{
				btnColor = { 130, 91, 33, 255 };
			}
			SDL_Rect btnRect = btn->textObj->getBoxRect();
			gm->renderFillRect(&btnRect, btnColor);
			gm->renderText(btn->textObj->getCurrentTexture(), btn->textObj->getTextRect());
			SDL_Rect currentOptionBottomBorder = { this->listBtnRect.x, this->listBtnRect.y + (this->listBtnRect.h * (index + 1)), this->listBtnRect.w, this->borderThickness };
			gm->renderFillRect(&currentOptionBottomBorder, this->borderColor);
		}
	}
	int borderHeight = this->showOptions ? this->listBtnRect.h * (this->buttons.size() + 1) : this->listBtnRect.h;
	int bottomBorderY = this->showOptions ? this->listBtnRect.y + (this->listBtnRect.h * (this->buttons.size() + 1)) : this->listBtnRect.y + this->listBtnRect.h;
	SDL_Rect topBorder = { this->listBtnRect.x, this->listBtnRect.y, this->listBtnRect.w, this->borderThickness };
	gm->renderFillRect(&topBorder, this->borderColor);
	SDL_Rect leftBorder = { this->listBtnRect.x, this->listBtnRect.y, this->borderThickness, borderHeight };
	gm->renderFillRect(&leftBorder, this->borderColor);
	SDL_Rect rightBorder = { this->listBtnRect.x + this->listBtnRect.w, this->listBtnRect.y, this->borderThickness, borderHeight };
	gm->renderFillRect(&rightBorder, this->borderColor);
	SDL_Rect bottomBorder = { this->listBtnRect.x, bottomBorderY, this->listBtnRect.w, this->borderThickness};
	gm->renderFillRect(&bottomBorder, this->borderColor);
}

// This will keep current btn at  index so that I can use 
void ButtonList::updateSelectedOption(int id)
{
	this->changedValue = this->currentIndex != id;
	this->currentIndex = id;
}

void ButtonList::handleEvents(SDL_Event& event)
{
	//this->changedValue = false;
	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (!this->showOptions)
		{
			if (this->buttons[currentIndex]->textObj->isHoveredOver(x, y, this->listBtnRect) && event.type == SDL_MOUSEBUTTONDOWN)
			{
				this->showOptions = true;
			}
		}
		else
		{
			for (auto& [index, btn] : buttons)
			{
				if (btn->textObj->isHoveredOver(x, y) && event.type == SDL_MOUSEBUTTONDOWN)
				{
					btn->click();
					this->showOptions = false;
				}
			}
		}
	}
}

std::string ButtonList::getSelectedOptionText()
{
	return this->buttons[this->currentIndex]->textObj->getText();
}

void ButtonList::updateRect(SDL_Rect newRect)
{
	this->listBtnRect = newRect;
	for (auto& [index, btn] : buttons)
	{
		btn->textObj->setBoxRect({ this->listBtnRect.x, this->listBtnRect.y + (this->listBtnRect.h * (index + 1)), this->listBtnRect.w, this->listBtnRect.h });
	}
}