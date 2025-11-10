#pragma once

#include "MainMenuState.h"

class Game{
private:
    // Variables
    GraphicsSettings gfxSettings;
    StateData stateData;
    
    sf::Clock dtClock;
    float dt = 0.f;
    
    std::unique_ptr<sf::RenderWindow> window;
    std::stack<std::unique_ptr<State>> states;

    std::map<std::string, int> supportedKeys;

    float gridSize = 100.f;

    // Initialization
    void initVariables();
    void initGraphicsSettings();
    void initWindow();
    void initKeys();
    void initStateData();
    void initStates();
    

public: 
    // Constructors / Destructors
    Game();
    virtual ~Game() = default;;

    // Regular
    void endApplication();

    // Update
    void updateDt();
    void updateSFMLEvents();
    void update();

    // Render
    void render();

    // Core
    void run();
};
