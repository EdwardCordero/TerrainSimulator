#pragma once
#include "Settings\GameSettings.h"
#include <iostream>

class SettingsManager
{
	public:
		SettingsManager();
		~SettingsManager();
		GameSettings loadSettings();
		GameSettings getCurrentSettings();
		void updateScreenSize(int width, int height);
		std::string getOSPath();
	private:
		std::string osPath;
		std::string settingsPath;
		GameSettings gameSettings;
};