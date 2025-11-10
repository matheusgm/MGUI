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
			this->keybinds[key] = this->data.supportedKeys->at(key2);
		}
	}

	ifs.close();
}

void MainMenuState::initGui()
{
	// Buttons
	this->buttons["GAME_STATE"] = std::make_unique<gui::Button>(
		100.f, 100.f,
		250.f, 50.f,
		&this->font, "New Game", 32);

	this->buttons["SETTINGS_STATE"] = std::make_unique<gui::Button>(
		100.f, 200.f,
		250.f, 50.f,
		&this->font, "Settings", 32);

	this->buttons["DIALOG_BOX_STATE"] = std::make_unique<gui::Button>(
		100.f, 300.f,
		250.f, 50.f,
		&this->font, "Dialog Box", 32);

	this->buttons["EXIT_STATE"] = std::make_unique<gui::Button>(
		100.f, 300.f,
		250.f, 50.f,
		&this->font, "Quit", 32);

	// Settings
	this->buttons["SETTINGS_STATE"]->onPressed([this]
											   { this->data.states->push(std::make_unique<SettingsState>(this->data)); });

	// Dialog Box
	this->buttons["DIALOG_BOX_STATE"]->onPressed([this]
												 { this->data.states->push(std::make_unique<DialogBoxState>(this->data)); });

	// Quit the game
	this->buttons["EXIT_STATE"]->onPressed([this]
										   { this->endState(); });
}

MainMenuState::MainMenuState(StateData &state_data)
	: State(state_data)
{
	this->initKeybinds();
	this->initGui();
	this->onResizeWindow();

	this->background.setFillColor(sf::Color::Magenta);
}

void MainMenuState::updateKeyboardInput(sf::Event &sfEvent)
{
}

void MainMenuState::updateEvents(sf::Event &sfEvent)
{
	this->updateKeyboardInput(sfEvent);
	for (auto &it : this->buttons)
		it.second->updateEvents(sfEvent, this->mousePosView);
}

void MainMenuState::onResizeWindow()
{
	sf::Vector2f window_center = this->getWindowCenter();
	sf::Vector2u window_size = this->data.window->getSize();

	float gap = 50.f;

	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(window_size.x),
			static_cast<float>(window_size.y)));

	this->buttons["GAME_STATE"]->setPosition(window_center.x - 125.f, 0 + gap);

	gui::Button &firstBtn = *this->buttons["GAME_STATE"];

	this->buttons["SETTINGS_STATE"]->setPosition(firstBtn.getLeft(), firstBtn.getBottom() + gap);

	gui::Button &secondBtn = *this->buttons["SETTINGS_STATE"];

	this->buttons["DIALOG_BOX_STATE"]->setPosition(firstBtn.getLeft(), secondBtn.getBottom() + gap);
	this->buttons["EXIT_STATE"]->setPosition(firstBtn.getLeft(), window_size.y - 50.f - gap);
}

void MainMenuState::updateGui() const
{
	/* Updates all the buttons in the state and handles their functionality */
	for (auto &it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}
}

void MainMenuState::update(float dt)
{
	this->updateMousePositions();

	this->updateGui();
}

void MainMenuState::renderGui(sf::RenderTarget &target) const
{
	for (auto &it : this->buttons)
		it.second->render(target);
}

void MainMenuState::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	this->renderGui(target);
}
