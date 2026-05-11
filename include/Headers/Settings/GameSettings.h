#pragma once
#include <nlohmann\json.hpp>
#include <Settings\DisplaySettings.h>

struct GameSettings
{
	GameSettings();
	~GameSettings();
	DisplaySettings display;
};

void to_json(nlohmann::json& j, const GameSettings& g);
void from_json(const nlohmann::json& j, GameSettings& g);