#include "stdafx.hpp"
#include "SettingsState.hpp"
#include "Model/Example.hpp"
#include "Gui/ListViewAdapter.tpp"
#include "ExampleListViewItem.hpp"
#include "Gui/Slider.hpp"
#include "Gui/Button.hpp"
#include "Gui/ListView.hpp"
#include "Gui/Select.hpp"
#include "Gui/Scroll.hpp"
#include "Gui/TextBox.hpp"

SettingsState::SettingsState(StateData &state_data)
	: State(state_data), soundText(font), soundValue(font), debugLinePositionText(font, "", 16U)
{
	initVariables();
	initKeybinds();
	initGui();
	onResizeWindow();

	background.setFillColor(sf::Color::Cyan);
	background.setSize(sf::Vector2f(state_data.window->getSize()));

	debugLinePositionText.setFillColor(sf::Color::Black);
	debugLinePositionText.setPosition({5.f, 5.f});
}

void SettingsState::updateKeyboardInput(sf::Event &sfEvent)
{
	// Apenas lógica de comando global que NÃO deve ser tratada pelo Canvas
}

void SettingsState::updateEvents(sf::Event &sfEvent)
{
	if (m_guiCanvas)
		m_guiCanvas->handleEvent(sfEvent, mousePosView);
}

void SettingsState::onResizeWindow()
{
	sf::Vector2f window_center = getWindowCenter();
	sf::Vector2u window_size = data.window->getSize();

	// float gap = 50.f;

	// // Background
	// background.setSize(
	// 	sf::Vector2f(
	// 		static_cast<float>(window_size.x),
	// 		static_cast<float>(window_size.y)));

	// // Buttons
	// buttons["BACK"]->setPosition({window_size.x - 150.f - gap, window_size.y - 50.f - gap});
	// gui::Button &firstBtn = *buttons["BACK"];
	// buttons["APPLY"]->setPosition({firstBtn.getLeft() - 150.f - gap, window_size.y - 50.f - gap});

	// // Sound Slider
	// float soundTextX = (window_center.x / 2.f) - (soundText.getGlobalBounds().size.x / 2.f);
	// soundText.setPosition({soundTextX, gap});

	// float soundValueX = (3.f * window_center.x / 2.f);
	// soundValue.setPosition({soundValueX, gap});

	// float sliderXStart = soundText.getPosition().x + soundText.getGlobalBounds().size.x + gap;
	// float sliderY = gap + (soundText.getGlobalBounds().size.y / 2.f);
	// soundSlider->setPosition({sliderXStart, sliderY});

	// float sliderXEnd = soundValue.getPosition().x - gap;
	// float newWidth = sliderXEnd - sliderXStart;
	// if (newWidth > 0)
	// 	soundSlider->setSize({newWidth, 16.f});
}

void SettingsState::update(sf::Time deltaTime)
{
	updateMousePositions();
	updateKeytime(deltaTime);

	if (!this->paused)
	{
		// Atualiza a lógica de tempo do Canvas (cursor piscando, animações)
		if (m_guiCanvas)
		{
			m_guiCanvas->handleContinuousMouseInput(mousePosView);
			m_guiCanvas->update(deltaTime);
		}
	}

	debugLinePositionText.setString(std::to_string(mousePosWindow.x) + " " + std::to_string(mousePosWindow.y));
	// debugLinePositionText.setPosition({mousePosWindow.x + 6.f, mousePosWindow.y - 20.f});
}

void SettingsState::render(sf::RenderTarget &target)
{
	target.draw(background);

	target.draw(soundText);
	target.draw(soundValue);

	if (m_guiCanvas)
		m_guiCanvas->draw(target);

	sf::Vertex verticalLine[2];
	verticalLine[0].position = sf::Vector2f(mousePosView.x, 0.f);
	verticalLine[0].color = sf::Color::Red;
	verticalLine[1].position = sf::Vector2f(mousePosView.x, 800.f);
	verticalLine[1].color = sf::Color::Red;

	sf::Vertex horizontalLine[2];
	horizontalLine[0].position = sf::Vector2f(0.f, mousePosView.y);
	horizontalLine[0].color = sf::Color::Blue;
	horizontalLine[1].position = sf::Vector2f(1200.f, mousePosView.y);
	horizontalLine[1].color = sf::Color::Blue;

	target.draw(verticalLine, 2, sf::PrimitiveType::Lines);
	target.draw(horizontalLine, 2, sf::PrimitiveType::Lines);

	target.draw(debugLinePositionText);
}

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
	m_guiCanvas = std::make_unique<gui::Canvas>();

	auto btnBack = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 100.f),
		sf::Vector2f(150.f, 50.f),
		&font, "Back", 32);

	btnBack->onPressed([this]
					   { endState(); });

	m_guiCanvas->addElement(std::move(btnBack));

	auto btnApply = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 200.f),
		sf::Vector2f(150.f, 50.f),
		&font, "Apply", 32);

	m_guiCanvas->addElement(std::move(btnApply));

	// SOUND ====================================
	soundText.setFillColor(sf::Color::Black);
	soundText.setCharacterSize(32);
	soundText.setString("Sound:");
	soundText.setFont(font);

	float soundTextX = (300.f) - (soundText.getGlobalBounds().size.x / 2.f);
	soundText.setPosition({soundTextX, 10.f});

	float soundValueX = (3.f * 300.f);
	soundValue.setPosition({soundValueX, 10.f});

	auto soundSlider = std::make_unique<gui::Slider>(sf::Vector2f(100.f, 100.f), sf::Vector2f(250.f, 16.f), 0, 100, 50);

	m_soundSlider = soundSlider.get();

	soundSlider->onValueChange(
		[this]
		{ soundValue.setString(std::to_string(m_soundSlider->getValue()) + "%"); });

	m_guiCanvas->addElement(std::move(soundSlider));

	soundValue.setFillColor(sf::Color::Black);
	soundValue.setCharacterSize(32);
	soundValue.setString(std::to_string(m_soundSlider->getValue()) + "%");
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

	auto listView = std::make_unique<gui::ListView>(sf::Vector2f(300.f, 200.f), sf::Vector2f(200.f, 300.f), std::move(produtoAdapter));
	m_guiCanvas->addElement(std::move(listView));

	// SELECT ====================================
	// auto select = std::make_unique<gui::Select>(sf::Vector2f(500.f, 400.f), sf::Vector2f(200.f, 100.f));
	// m_guiCanvas->addElement(std::move(select));

	// SCROLL ====================================
	auto scroll = std::make_unique<gui::Scroll>(sf::Vector2f(800.f, 200.f), sf::Vector2f(20.f, 200.f));
	m_guiCanvas->addElement(std::move(scroll));

	auto textBox = std::make_unique<gui::TextBox>(sf::Vector2f(100.f, 600.f), sf::Vector2f(400.f, 20.f));
	textBox->setAsPrimaryFocusTarget(true, sf::Keyboard::Key::T);
	m_guiCanvas->addElement(std::move(textBox));

	auto textBox2 = std::make_unique<gui::TextBox>(sf::Vector2f(500.f, 150.f), sf::Vector2f(200.f, 20.f));
	m_guiCanvas->addElement(std::move(textBox2));
}
