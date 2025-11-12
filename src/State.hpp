#pragma once

#include "GraphicsSettings.hpp"

class Player;
class GraphicsSettings;
class State;

struct StateData
{
	float gridSize = 0.f;
    sf::RenderWindow* window = nullptr;
    GraphicsSettings* gfxSettings = nullptr;
    std::map<std::string, int>* supportedKeys = nullptr;
    std::stack<std::unique_ptr<State>>* states = nullptr;
};

class State
{
protected:
	StateData& data;
	sf::Font font;
	std::map<std::string, int> keybinds;
	bool quit = false;
    bool paused = false;
    float keytime = 0.f;
    float keytimeMax = 10.f;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Resources
	std::map<std::string, sf::Texture> textures;

	// Functions
	virtual void initKeybinds() = 0;

public:
	State(StateData& data);
	virtual ~State() = default;

	bool getQuit() const { return quit; };
	bool getKeytime();
	sf::Vector2f getWindowCenter() const;

	void endState() { quit = true; };
	void pauseState() { paused = true; };
	void unpauseState() { paused = false; };

	virtual void updateMousePositions();
    virtual void updateKeytime(float dt);
    virtual void updateKeyboardInput(sf::Event& event) = 0;
    virtual void updateEvents(sf::Event& event) = 0;
    virtual void onResizeWindow() = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
};
