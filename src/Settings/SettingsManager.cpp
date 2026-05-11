#include "Settings\SettingsManager.h"
#include <nlohmann\json.hpp>
#include <fstream>
#include <filesystem> 
#include <iostream>
#include <SDL_filesystem.h>

using json = nlohmann::json;

SettingsManager::SettingsManager()
{
	char* osPath = SDL_GetPrefPath("UncannyGS", "Pardox");
	this->osPath = std::string(osPath);
	this->settingsPath = std::string(DATA_PATH) + "settings.json"; //std::string(osPath) + "settings.json";

	if (!std::filesystem::exists(this->settingsPath))
	{
		json j = this->gameSettings;
		std::ofstream out(this->settingsPath);
		out << j.dump(4);
		out.close();
	}

	this->gameSettings = this->loadSettings();
	SDL_free(osPath);
}

SettingsManager::~SettingsManager()
{

}

std::string SettingsManager::getOSPath()
{
	return this->osPath;
}

GameSettings SettingsManager::getCurrentSettings()
{
	return this->gameSettings;
}

GameSettings SettingsManager::loadSettings()
{
	std::ifstream in(this->settingsPath);
	json j;
	in >> j;
	return j.get<GameSettings>();
}

void SettingsManager::updateScreenSize(int width, int height)
{
	this->gameSettings.display.height = height;
	this->gameSettings.display.width = width;
}