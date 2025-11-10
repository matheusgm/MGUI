#include "stdafx.h"
#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings()
{
    this->title = "DEFAULT";
    this->resolution = sf::VideoMode::getDesktopMode();
    this->fullscreen = false;
    this->verticalSync = false;
    this->frameRateLimit = 120;
    this->contextSettings.antiAliasingLevel = 0;
    this->videoMode = sf::VideoMode::getFullscreenModes();
}

void GraphicsSettings::saveToFile(const std::string path)
{
    std::ofstream ofs(path);

    if (ofs.is_open()) {
        ofs << this->title;
        ofs << this->resolution.size.x << " " << this->resolution.size.y;
        ofs << this->fullscreen;
        ofs << this->frameRateLimit;
        ofs << this->verticalSync;
        ofs << this->contextSettings.antiAliasingLevel;
    }

    ofs.close();
}
void GraphicsSettings::loadFromFile(const std::string path)
{
    std::ifstream ifs(path);

    if (ifs.is_open()) {
        std::getline(ifs, this->title);
        ifs >> this->resolution.size.x >> this->resolution.size.y;
        ifs >> this->fullscreen;
        ifs >> this->frameRateLimit;
        ifs >> this->verticalSync;
        ifs >> this->contextSettings.antiAliasingLevel;
    }

    ifs.close();
}
