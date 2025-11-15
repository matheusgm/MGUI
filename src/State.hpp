#pragma once

#include "GraphicsSettings.hpp"
#include "Model/StateData.hpp"

class State
{
public:
	State(StateData &data);
	virtual ~State() = default;

	virtual void updateMousePositions();
	virtual void updateKeytime(float dt);
	virtual void updateKeyboardInput(sf::Event &event) = 0;
	virtual void updateEvents(sf::Event &event) = 0;
	virtual void onResizeWindow() = 0;
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderTarget &target) = 0;

	bool getQuit() const { return quit; };
	bool getKeytime();
	sf::Vector2f getWindowCenter() const;

	void endState() { quit = true; };
	void pauseState() { paused = true; };
	void unpauseState() { paused = false; };

protected:
	StateData &data;
	sf::Font font;
	std::map<std::string, int> keybinds;
	bool quit = false;
	bool paused = false;
	float keytime = 0.f;
	float keytimeMax = 10.f;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	std::map<std::string, sf::Texture> textures;

	virtual void initKeybinds() = 0;
};
