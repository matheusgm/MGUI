#pragma once

#include "MainMenuState.hpp"

class Game
{
public:
    Game();
    virtual ~Game() = default;

    void updateDt();
    void updateSFMLEvents();
    void update();

    void render();

    void run();

    void endApplication();

private:
    GraphicsSettings gfxSettings;
    StateData stateData;

    sf::Clock dtClock;

    std::unique_ptr<sf::RenderWindow> window;
    std::stack<std::unique_ptr<State>> states;

    std::map<std::string, int> supportedKeys;

    float dt = 0.f;
    float gridSize = 100.f;

    void initVariables();
    void initGraphicsSettings();
    void initWindow();
    void initKeys();
    void initStateData();
    void initStates();
};
