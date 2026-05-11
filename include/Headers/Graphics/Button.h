#pragma once
#include "Graphics\TextObject.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

struct Button
{
	using ClickHandler = std::function<void()>;

	Button();
	~Button();
	void setOnClick(ClickHandler handler);
	void click();

	std::shared_ptr<TextObject> textObj;

	ClickHandler onClick;
};