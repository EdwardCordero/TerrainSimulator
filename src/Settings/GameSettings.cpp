#include "Settings\GameSettings.h"
#include "Settings\DisplaySettings.h"
#include <nlohmann\json.hpp>

GameSettings::GameSettings()
{
    this->display = DisplaySettings();
}

GameSettings::~GameSettings()
{

}

void to_json(nlohmann::json& j, const GameSettings& g)
{
    j = nlohmann::json{
        {"display", g.display},
    };
}

void from_json(const nlohmann::json& j, GameSettings& g)
{
    j.at("display").get_to(g.display);
}