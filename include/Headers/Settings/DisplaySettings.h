#pragma once
#include <nlohmann\json.hpp>
#include <array>

struct DisplaySettings
{
	DisplaySettings();
	~DisplaySettings();
	bool fullScreen;
	int width;
	int height;
	int baseWidth;
	int baseHeight;
	std::array<int, 2> aspectRatio = { 16, 9 };
};

void to_json(nlohmann::json& j, const DisplaySettings& d);
void from_json(const nlohmann::json& j, DisplaySettings& d);