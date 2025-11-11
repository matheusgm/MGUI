#include "stdafx.h"
#include "SettingsState.h"
#include "Model/Example.hpp"
#include "Gui/ListViewAdapter.tpp"
#include "ExampleListViewItem.hpp"

void SettingsState::initVariables()
{
	// modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initKeybinds()
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

void SettingsState::initGui()
{
	buttons["BACK"] = std::make_unique<gui::Button>(
		100.f, 100.f,
		150.f, 50.f,
		&font, "Back", 32);

	buttons["APPLY"] = std::make_unique<gui::Button>(
		100.f, 200.f,
		150.f, 50.f,
		&font, "Apply", 32);

	// Button functionality
	// Quit the game
	buttons["BACK"]->onPressed([this]
									 { endState(); });

	/*std::vector<std::string> modes_str;
	for (auto& i : modes) {
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}*/

	// SOUND ====================================
	soundText.setFillColor(sf::Color::Black);
	soundText.setCharacterSize(32);
	soundText.setString("Sound:");
	soundText.setFont(font);

	soundSlider = std::make_unique<gui::Slider>(100.f, 100.f, 250.f, 16.f, 0, 100, 50);
	soundSlider->onValueChange(
		[this]
		{ soundValue.setString(std::to_string(soundSlider->getValue()) + "%"); });

	soundValue.setFillColor(sf::Color::Black);
	soundValue.setCharacterSize(32);
	soundValue.setString(std::to_string(soundSlider->getValue()) + "%");
	soundValue.setFont(font);

	// LIST VIEW ====================================

	std::vector<Example> todosOsProdutos;
	for (int i = 0; i < 20; ++i)
	{
		todosOsProdutos.emplace_back(Example("#" + std::to_string(i),
											 100.0f + i * 0.5f,
											 (i % 100 == 0) ? 0 : 5));
	}

	float itemHeight = 60.0f;
	auto produtoAdapter = std::make_unique<gui::ListViewAdapter<Example, ExampleListViewItem>>(std::move(todosOsProdutos), font, itemHeight);

	listView = std::make_unique<gui::ListView>(sf::Vector2f({200.f, 200.f}), sf::Vector2f({200.f, 300.f}), std::move(produtoAdapter));

	// SELECT ====================================
	select = std::make_unique<gui::Select>(500.f, 200.f, 200.f, 100.f);

	// SCROLL ====================================
	scroll = std::make_unique<gui::Scroll>(1000.f, 200.f, 20.f, 200.f);
}

SettingsState::SettingsState(StateData &state_data)
	: State(state_data), soundText(font), soundValue(font)
{
	initVariables();
	initKeybinds();
	initGui();
	onResizeWindow();

	background.setFillColor(sf::Color::Cyan);
}

void SettingsState::updateKeyboardInput(sf::Event &sfEvent)
{
}

void SettingsState::updateEvents(sf::Event &sfEvent)
{
	updateKeyboardInput(sfEvent);
	for (auto &it : buttons)
		it.second->updateEvents(sfEvent, mousePosView);

	soundSlider->updateEvents(sfEvent, mousePosView);

	listView->updateEvents(sfEvent, mousePosView);

	select->updateEvents(sfEvent, mousePosView);

	scroll->updateEvents(sfEvent, mousePosView);
}

void SettingsState::onResizeWindow()
{
	sf::Vector2f window_center = getWindowCenter();
	sf::Vector2u window_size = data.window->getSize();

	float gap = 50.f;

	// Background
	background.setSize(
		sf::Vector2f(
			static_cast<float>(window_size.x),
			static_cast<float>(window_size.y)));

	// Buttons
	buttons["BACK"]->setPosition(window_size.x - 150.f - gap, window_size.y - 50.f - gap);
	gui::Button &firstBtn = *buttons["BACK"];
	buttons["APPLY"]->setPosition(firstBtn.getLeft() - 150.f - gap, window_size.y - 50.f - gap);

	// Sound Slider
	soundText.setPosition(sf::Vector2f((window_center.x / 2.f) - (soundText.getGlobalBounds().size.x / 2.f), gap));
	soundValue.setPosition(sf::Vector2f((3.f * window_center.x / 2.f), gap));
	soundSlider->setPosition(soundText.getPosition().x + soundText.getGlobalBounds().size.x + gap, 50.f + (soundText.getGlobalBounds().size.y / 2.f));
	soundSlider->setSize(soundValue.getPosition().x - soundText.getPosition().x - soundText.getGlobalBounds().size.x - 2 * gap, 16.f);
}

void SettingsState::updateGui(float dt) const
{
	/* Updates all the gui in the state and handles their functionality */
	for (auto &it : buttons)
		it.second->update(mousePosView);

	soundSlider->update(mousePosView);
	listView->update(mousePosView);
	select->update(mousePosView);
	scroll->update(mousePosView);
}

void SettingsState::update(float dt)
{
	updateMousePositions();

	updateGui(dt);
}

void SettingsState::renderGui(sf::RenderTarget &target) const
{
	for (auto &it : buttons)
		it.second->render(target);

	target.draw(soundText);
	target.draw(soundValue);
	soundSlider->render(target);

	listView->render(target);
	select->render(target);
	scroll->render(target);
}

void SettingsState::render(sf::RenderTarget &target)
{
	target.draw(background);

	renderGui(target);
}
