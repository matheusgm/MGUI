#include "stdafx.hpp"
#include "Game.hpp"

Game::Game()
{
	initVariables();
	initGraphicsSettings();
	initWindow();
	initKeys();
	initStateData();
	initStates();
}

void Game::updateDt()
{
	dt = dtClock.restart().asSeconds();
}

void Game::updateSFMLEvents()
{
	while (std::optional event = window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window->close();
		}
		else if (event->is<sf::Event::Resized>())
		{
			auto resized = event->getIf<sf::Event::Resized>();
			sf::FloatRect visibleArea({0.f, 0.f}, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
			window->setView(sf::View(visibleArea));
			gfxSettings.resolution.size.x = resized->size.x;
			gfxSettings.resolution.size.y = resized->size.y;
			if (!states.empty())
			{
				states.top()->onResizeWindow();
			}
		}
		if (!states.empty())
			states.top()->updateEvents(*event);
	}
}

void Game::update()
{
	updateSFMLEvents();

	if (!states.empty())
	{
		states.top()->update(dt);

		if (states.top()->getQuit())
		{
			states.top()->endState();
			states.pop();
			if (!states.empty())
			{ // Rever, talvez poderia ser bom uma verificação para saber se precisa atualizar
				states.top()->onResizeWindow();
			}
		}
	}
	else
	{
		endApplication();
		window->close();
	}
}

void Game::render()
{
	window->clear();

	if (!states.empty())
		states.top()->render(*window);

	window->display();
}

void Game::run()
{
	while (window->isOpen())
	{
		updateDt();
		update();
		render();
	}
}

void Game::endApplication()
{
	std::cout << "Ending Application!" << "\n";
}

void Game::initVariables()
{
	window = NULL;
	dt = 0.f;
	gridSize = 50.f;
}

void Game::initGraphicsSettings()
{
	gfxSettings.loadFromFile("src/Config/graphics.ini");
}

void Game::initWindow()
{

	/* Create a SFML window */
	if (gfxSettings.fullscreen)
	{
		window = std::make_unique<sf::RenderWindow>(
			gfxSettings.resolution,
			gfxSettings.title,
			sf::State::Fullscreen,
			gfxSettings.contextSettings);
	}
	else
	{
		window = std::make_unique<sf::RenderWindow>(
			gfxSettings.resolution,
			gfxSettings.title,
			sf::State::Windowed,
			gfxSettings.contextSettings);
	}

	window->setFramerateLimit(gfxSettings.frameRateLimit);
	window->setVerticalSyncEnabled(gfxSettings.verticalSync);
}

void Game::initKeys()
{

	std::ifstream ifs("src/Config/supported_keys.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value)
		{
			supportedKeys[key] = key_value;
		}
	}

	ifs.close();

	// DEBUG REMOVE LATER!
	for (auto i : supportedKeys)
	{
		std::cout << i.first << " " << i.second << "\n";
	}
}

void Game::initStateData()
{
	stateData.window = window.get();
	stateData.gfxSettings = &gfxSettings;
	stateData.supportedKeys = &supportedKeys;
	stateData.states = &states;
	stateData.gridSize = gridSize;
}

void Game::initStates()
{
	states.push(std::make_unique<MainMenuState>(stateData));
}
