#include "stdafx.h"
#include "Game.h"

// Static functions

// Initializer functions

void Game::initVariables()
{
	this->window = NULL;
	this->dt = 0.f;
	this->gridSize = 50.f;
}

void Game::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("src/Config/graphics.ini");
}

void Game::initWindow(){

	/* Create a SFML window */
	if (this->gfxSettings.fullscreen)
	{
		this->window = std::make_unique<sf::RenderWindow>(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::State::Fullscreen,
			this->gfxSettings.contextSettings
		);
	}
	else
	{
		this->window = std::make_unique<sf::RenderWindow>(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::State::Windowed,
			this->gfxSettings.contextSettings
		);
	}

	this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
}


void Game::initKeys()
{

	std::ifstream ifs("src/Config/supported_keys.ini");

	if (ifs.is_open()) {
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value) {
			this->supportedKeys[key] = key_value;
		}
	}

	ifs.close();

	// DEBUG REMOVE LATER!
	for (auto i : this->supportedKeys) {
		std::cout << i.first << " " << i.second << "\n";
	}
	
}

void Game::initStateData()
{
	this->stateData.window = this->window.get();
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.gridSize = this->gridSize;
}

void Game::initStates()
{
	this->states.push(std::make_unique<MainMenuState>(this->stateData));
}


// Constructors / Destructors
Game::Game(){
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initStateData();
	this->initStates();

}

void Game::endApplication()
{
	std::cout << "Ending Application!" << "\n";
}


void Game::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();

}

void Game::updateSFMLEvents()
{
	while (std::optional event = this->window->pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			this->window->close();
		}else if (event->is<sf::Event::Resized>()) {
			// resize my view
			auto resized = event->getIf<sf::Event::Resized>();
			sf::FloatRect visibleArea({0.f, 0.f}, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
			this->window->setView(sf::View(visibleArea));
			this->gfxSettings.resolution.size.x = resized->size.x;
			this->gfxSettings.resolution.size.y = resized->size.y;
			if (!this->states.empty()) {
				this->states.top()->onResizeWindow();
			}
		}
		if (!this->states.empty()) {
			this->states.top()->updateEvents(*event);
		}
	}
}

void Game::update()
{
	this->updateSFMLEvents();

	if (!this->states.empty()) {
		this->states.top()->update(this->dt);
		
		if (this->states.top()->getQuit()) {
			this->states.top()->endState();
			this->states.pop();
			if (!this->states.empty()) { // Rever, talvez poderia ser bom uma verifica��o para saber se precisa atualizar
				this->states.top()->onResizeWindow();
			}
		}
	}
	// Application end
	else {
		this->endApplication();
		this->window->close();
	}

	

}

void Game::render()
{
	this->window->clear();

	// Render items
	if (!this->states.empty()) {
		this->states.top()->render(*this->window);
	}

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen()) {
		this->updateDt();
		this->update();
		this->render();
	}
}


