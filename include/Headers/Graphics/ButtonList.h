#pragma once
#include "Graphics\GraphicsManager.h"
#include <Graphics\Button.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

struct ButtonList
{
	ButtonList(std::shared_ptr<GraphicsManager> gm, SDL_Rect btnRect, std::vector<std::string> choices);
	~ButtonList();

	void addOption(std::string optionText);
	void removeOption(int choiceID);
	void updateSelectedOption(int id);
	void handleEvents(SDL_Event& event);
	void render();
	void updateRect(SDL_Rect newRect);

	std::string getSelectedOptionText();

	SDL_Rect listBtnRect;
	SDL_Color optionRectColor;
	std::shared_ptr<GraphicsManager> gm;
	std::map<int, std::shared_ptr<Button>> buttons;
	int currentIndex;
	bool borderOn;
	SDL_Color borderColor;
	int borderThickness;
	bool showOptions;
	bool horizontal;
	bool changedValue;
};