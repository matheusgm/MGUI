#include "stdafx.h"
#include "MainMenuState.h"

void MainMenuState::initKeybinds()
{

	std::ifstream ifs("src/Config/mainmenustate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			keybinds[key] = data.supportedKeys->at(key2);
		}
	}

	ifs.close();
}

void MainMenuState::initGui()
{
	// Buttons
	buttons["GAME_STATE"] = std::make_unique<gui::Button>(
		100.f, 100.f,
		250.f, 50.f,
		&font, "New Game", 32);

	buttons["SETTINGS_STATE"] = std::make_unique<gui::Button>(
		100.f, 200.f,
		250.f, 50.f,
		&font, "Settings", 32);

	buttons["DIALOG_BOX_STATE"] = std::make_unique<gui::Button>(
		100.f, 300.f,
		250.f, 50.f,
		&font, "Dialog Box", 32);

	buttons["EXIT_STATE"] = std::make_unique<gui::Button>(
		100.f, 300.f,
		250.f, 50.f,
		&font, "Quit", 32);

	// Settings
	buttons["SETTINGS_STATE"]->onPressed([this]
											   { data.states->push(std::make_unique<SettingsState>(data)); });

	// Dialog Box
	buttons["DIALOG_BOX_STATE"]->onPressed([this]
												 { data.states->push(std::make_unique<DialogBoxState>(data)); });

	// Quit the game
	buttons["EXIT_STATE"]->onPressed([this]
										   { endState(); });
}

MainMenuState::MainMenuState(StateData &state_data)
	: State(state_data)
{
	initKeybinds();
	initGui();
	onResizeWindow();

	background.setFillColor(sf::Color::Magenta);
}

void MainMenuState::updateKeyboardInput(sf::Event &sfEvent)
{
}

void MainMenuState::updateEvents(sf::Event &sfEvent)
{
	updateKeyboardInput(sfEvent);
	for (auto &it : buttons)
		it.second->updateEvents(sfEvent, mousePosView);
}

void MainMenuState::onResizeWindow()
{
	sf::Vector2f window_center = getWindowCenter();
	sf::Vector2u window_size = data.window->getSize();

	float gap = 50.f;

	background.setSize(
		sf::Vector2f(
			static_cast<float>(window_size.x),
			static_cast<float>(window_size.y)));

	buttons["GAME_STATE"]->setPosition(window_center.x - 125.f, 0 + gap);

	gui::Button &firstBtn = *buttons["GAME_STATE"];

	buttons["SETTINGS_STATE"]->setPosition(firstBtn.getLeft(), firstBtn.getBottom() + gap);

	gui::Button &secondBtn = *buttons["SETTINGS_STATE"];

	buttons["DIALOG_BOX_STATE"]->setPosition(firstBtn.getLeft(), secondBtn.getBottom() + gap);
	buttons["EXIT_STATE"]->setPosition(firstBtn.getLeft(), window_size.y - 50.f - gap);
}

void MainMenuState::updateGui() const
{
	/* Updates all the buttons in the state and handles their functionality */
	for (auto &it : buttons)
	{
		it.second->update(mousePosView);
	}
}

void MainMenuState::update(float dt)
{
	updateMousePositions();

	updateGui();
}

void MainMenuState::renderGui(sf::RenderTarget &target) const
{
	for (auto &it : buttons)
		it.second->render(target);
}

void MainMenuState::render(sf::RenderTarget& target)
{
	target.draw(background);

	renderGui(target);
}
