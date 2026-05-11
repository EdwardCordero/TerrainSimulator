#include <Settings\DisplaySettings.h>

DisplaySettings::DisplaySettings()
{
    this->fullScreen = true;
    this->width = 2560;
    this->height = 1440;
    this->baseWidth = 2560;
    this->baseHeight = 1440;
}

DisplaySettings::~DisplaySettings()
{

};

void to_json(nlohmann::json& j, const DisplaySettings& d) {
    j = nlohmann::json{
        {"fullScreen", d.fullScreen},
        {"width", d.width},
        {"height", d.height}
    };
}

void from_json(const nlohmann::json& j, DisplaySettings& d) {
    j.at("fullScreen").get_to(d.fullScreen);
    j.at("width").get_to(d.width);
    j.at("height").get_to(d.height);
}
