#include "stdafx.hpp"
#include "GraphicsSettings.hpp"

GraphicsSettings::GraphicsSettings()
{
    title = "DEFAULT";
    resolution = sf::VideoMode::getDesktopMode();
    fullscreen = false;
    verticalSync = false;
    frameRateLimit = 120;
    contextSettings.antiAliasingLevel = 0;
    videoMode = sf::VideoMode::getFullscreenModes();
}

void GraphicsSettings::saveToFile(const std::string path)
{
    std::ofstream ofs(path);

    if (ofs.is_open()) {
        ofs << title;
        ofs << resolution.size.x << " " << resolution.size.y;
        ofs << fullscreen;
        ofs << frameRateLimit;
        ofs << verticalSync;
        ofs << contextSettings.antiAliasingLevel;
    }

    ofs.close();
}
void GraphicsSettings::loadFromFile(const std::string path)
{
    std::ifstream ifs(path);

    if (ifs.is_open()) {
        std::getline(ifs, title);
        ifs >> resolution.size.x >> resolution.size.y;
        ifs >> fullscreen;
        ifs >> frameRateLimit;
        ifs >> verticalSync;
        ifs >> contextSettings.antiAliasingLevel;
    }

    ifs.close();
}
