#include "stdafx.hpp"
#include "MainMenuState.hpp"
#include "Gui/Button.hpp"

MainMenuState::MainMenuState(StateData &state_data)
	: State(state_data)
{
	initKeybinds();
	initGui();
	onResizeWindow();

	background.setFillColor(sf::Color::Magenta);
	background.setSize(sf::Vector2f(state_data.window->getSize()));
}

void MainMenuState::updateKeyboardInput(sf::Event &sfEvent)
{
}

void MainMenuState::updateEvents(sf::Event &sfEvent)
{
	if (m_guiCanvas)
		m_guiCanvas->handleEvent(sfEvent, mousePosView);
}

void MainMenuState::onResizeWindow()
{
	sf::Vector2f window_center = getWindowCenter();
	sf::Vector2u window_size = data.window->getSize();

	// float gap = 50.f;

	// background.setSize(
	// 	sf::Vector2f(
	// 		static_cast<float>(window_size.x),
	// 		static_cast<float>(window_size.y)));

	// buttons["GAME_STATE"]->setPosition({window_center.x - 125.f, 0 + gap});

	// gui::Button &firstBtn = *buttons["GAME_STATE"];

	// buttons["SETTINGS_STATE"]->setPosition({firstBtn.getLeft(), firstBtn.getBottom() + gap});

	// gui::Button &secondBtn = *buttons["SETTINGS_STATE"];

	// buttons["DIALOG_BOX_STATE"]->setPosition({firstBtn.getLeft(), secondBtn.getBottom() + gap});
	// buttons["EXIT_STATE"]->setPosition({firstBtn.getLeft(), window_size.y - 50.f - gap});
}

void MainMenuState::update(sf::Time deltaTime)
{
	updateMousePositions();

	if (!this->paused)
	{
		// Atualiza a lógica de tempo do Canvas (cursor piscando, animações)
		if (m_guiCanvas)
		{
			m_guiCanvas->handleContinuousMouseInput(mousePosView);
			m_guiCanvas->update(deltaTime);
		}
	}
}

void MainMenuState::render(sf::RenderTarget &target)
{
	target.draw(background);

	if (m_guiCanvas)
		m_guiCanvas->draw(target);
}

void MainMenuState::initKeybinds()
{

	std::ifstream ifs("src/Config/mainmenustate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
			keybinds[key] = data.supportedKeys->at(key2);
	}

	ifs.close();
}

void MainMenuState::initGui()
{
	m_guiCanvas = std::make_unique<gui::Canvas>();

	// Buttons
	auto btnGameState = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 100.f),
		sf::Vector2f(250.f, 50.f),
		&font, "New Game", 32);

	m_guiCanvas->addElement(std::move(btnGameState));

	auto btnSettingState = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 200.f),
		sf::Vector2f(250.f, 50.f),
		&font, "Settings", 32);

	btnSettingState->onPressed([this]
							   { data.states->push(std::make_unique<SettingsState>(data)); });

	m_guiCanvas->addElement(std::move(btnSettingState));

	auto btnDialogBoxState = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 300.f),
		sf::Vector2f(250.f, 50.f),
		&font, "Dialog Box", 32);

	btnDialogBoxState->onPressed([this]
								 { data.states->push(std::make_unique<DialogBoxState>(data)); });

	m_guiCanvas->addElement(std::move(btnDialogBoxState));

	auto btnExitState = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 400.f),
		sf::Vector2f(250.f, 50.f),
		&font, "Quit", 32);

	btnExitState->onPressed([this]
							{ endState(); });

	m_guiCanvas->addElement(std::move(btnExitState));
}
