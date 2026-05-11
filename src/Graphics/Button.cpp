#include "Graphics\Button.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

Button::Button()
{
	this->textObj = std::make_shared<TextObject>();
}

Button::~Button()
{
	this->textObj->~TextObject();
}

void Button::setOnClick(ClickHandler handler)
{
	this->onClick = handler;
}

void Button::click()
{
	if (onClick)
	{
		onClick();
	}
	else
	{
		std::cout << "No onClick behavior set" << std::endl;
	}
}