#include "stdafx.hpp"
#include "SettingsState.hpp"
#include "Model/Example.hpp"
#include "Gui/ListViewAdapter.tpp"
#include "ExampleListViewItem.hpp"
#include "Gui/FocusElementManager.hpp"

SettingsState::SettingsState(StateData &state_data)
	: State(state_data), soundText(font), soundValue(font), debugLinePositionText(font, "", 16U)
{
	initVariables();
	initKeybinds();
	initGui();
	onResizeWindow();

	background.setFillColor(sf::Color::Cyan);

	debugLinePositionText.setFillColor(sf::Color::Black);
	debugLinePositionText.setPosition({5.f, 5.f});
}

void SettingsState::updateKeyboardInput(sf::Event &sfEvent)
{
	if (auto mousePressed = sfEvent.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			// Se o clique foi processado por um elemento (selecionando-o),
			// o FocusManager já atualizou o estado.

			// Se o clique foi em um TextBox, ele chamou setFocusedElement(this).
			// Se o clique foi fora de TODOS os elementos interativos:

			// 🛑 CRUCIAL: Se o elemento focado atual for diferente do elemento
			// onde o clique aconteceu, ou se ele for nullptr, limpe o foco.

			// Simplificação (idealmente, cada elemento cuida de si ou você verifica colisões):
			// Assumimos que o TextBox.updateEvents só chama setFocusedElement(this) se houve hit.
			// Se o FocusManager ainda está com foco em 'X', mas o clique foi no fundo (miss),
			// e X não se desfocou, vamos forçar o desfoco.

			gui::GuiElement *focusedElement = gui::FocusElementManager::getInstance().getFocusedElement();
			if (focusedElement != nullptr)
			{
				bool clickHitElement = false;
				// Verifique se o clique colidiu com o elemento focado ou qualquer outro elemento
				// (Para simplificar, usaremos o getLocalBounds mapeado para o global)

				// Exemplo Simplificado de verificação de acerto:
				if (focusedElement->getTransform().transformRect(focusedElement->getLocalBounds()).contains(mousePosView))
				{
					clickHitElement = true; // Acertou o elemento focado
				}

				// Se o clique não acertou o elemento focado, e ele ainda está focado, desfoca.
				if (!clickHitElement)
				{
					// Força a perda de foco global
					gui::FocusElementManager::getInstance().clearFocus();
				}
			}
		}
	}

	// 3. Lógica de ATIVAÇÃO DO CHAT (Enter)

}

void SettingsState::updateEvents(sf::Event &sfEvent)
{
	updateKeyboardInput(sfEvent);

		// 3. Lógica de ATIVAÇÃO DO CHAT (Enter)
	if (auto keyPressed = sfEvent.getIf<sf::Event::KeyPressed>()) // Esse tava ali em cima
	{
		if (keyPressed->code == sf::Keyboard::Key::Enter)
		{
			gui::GuiElement *currentFocus = gui::FocusElementManager::getInstance().getFocusedElement();

			// Situação A: Inicia tudo desfocado. Se clicar Enter, ativa o chat.
			if (currentFocus == nullptr)
			{
				cout << "Current focus NULL e textBox Selected" << endl;
				// Ativa o TextBox de chat.
				// O setSelected(true) do chat fará o FocusManager rastreá-lo.
				textBox->setSelected(true);
				return; // Consome o evento para evitar outra lógica
			}

			// Situação B: Se outro elemento GUI (não o chat) estiver focado, nada acontece.
			// Isso é resolvido pelo chatTextBox.updateEvents:
			// Se currentFocus != chatTextBox, significa que currentFocus é o TextBox2
			// e, como m_submitsOnEnter é false para o TextBox2, o Enter é ignorado
			// dentro do TextBox2::updateEvents (como definido no Passo 1).

			// Se chegamos aqui, o evento já foi consumido por um dos TextBox
		}
	}

	for (auto &it : buttons)
		it.second->updateEvents(sfEvent, mousePosView);

	soundSlider->updateEvents(sfEvent, mousePosView);

	listView->updateEvents(sfEvent, mousePosView);

	select->updateEvents(sfEvent, mousePosView);

	scroll->updateEvents(sfEvent, mousePosView);

	textBox->updateEvents(sfEvent, mousePosView);

	textBox2->updateEvents(sfEvent, mousePosView);
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
	buttons["BACK"]->setPosition({window_size.x - 150.f - gap, window_size.y - 50.f - gap});
	gui::Button &firstBtn = *buttons["BACK"];
	buttons["APPLY"]->setPosition({firstBtn.getLeft() - 150.f - gap, window_size.y - 50.f - gap});

	// Sound Slider
	float soundTextX = (window_center.x / 2.f) - (soundText.getGlobalBounds().size.x / 2.f);
	soundText.setPosition({soundTextX, gap});

	float soundValueX = (3.f * window_center.x / 2.f);
	soundValue.setPosition({soundValueX, gap});

	float sliderXStart = soundText.getPosition().x + soundText.getGlobalBounds().size.x + gap;
	float sliderY = gap + (soundText.getGlobalBounds().size.y / 2.f);
	soundSlider->setPosition({sliderXStart, sliderY});

	float sliderXEnd = soundValue.getPosition().x - gap;
	float newWidth = sliderXEnd - sliderXStart;
	if (newWidth > 0)
		soundSlider->setSize({newWidth, 16.f});
}

void SettingsState::updateGui(sf::Time deltaTime) const
{
	/* Updates all the gui in the state and handles their functionality */
	for (auto &it : buttons)
		it.second->update(deltaTime);

	soundSlider->update(deltaTime);
	listView->update(deltaTime);
	select->update(deltaTime);
	scroll->update(deltaTime);
	textBox->update(deltaTime);
	textBox2->update(deltaTime);
}

void SettingsState::update(sf::Time deltaTime)
{
	updateMousePositions();

	updateGui(deltaTime);

	debugLinePositionText.setString(std::to_string(mousePosWindow.x) + " " + std::to_string(mousePosWindow.y));
	// debugLinePositionText.setPosition({mousePosWindow.x + 6.f, mousePosWindow.y - 20.f});
}

void SettingsState::renderGui(sf::RenderTarget &target) const
{
	for (auto &it : buttons)
		target.draw(*it.second);

	target.draw(soundText);
	target.draw(soundValue);
	target.draw(*soundSlider);

	target.draw(*listView);
	target.draw(*select);
	target.draw(*scroll);
	target.draw(*textBox);
	target.draw(*textBox2);
}

void SettingsState::render(sf::RenderTarget &target)
{
	target.draw(background);

	renderGui(target);

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
	buttons["BACK"] = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 100.f),
		sf::Vector2f(150.f, 50.f),
		&font, "Back", 32);

	buttons["APPLY"] = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 200.f),
		sf::Vector2f(150.f, 50.f),
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

	soundSlider = std::make_unique<gui::Slider>(sf::Vector2f(100.f, 100.f), sf::Vector2f(250.f, 16.f), 0, 100, 50);
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

	listView = std::make_unique<gui::ListView>(sf::Vector2f(200.f, 200.f), sf::Vector2f(200.f, 300.f), std::move(produtoAdapter));

	// SELECT ====================================
	select = std::make_unique<gui::Select>(sf::Vector2f(500.f, 400.f), sf::Vector2f(200.f, 100.f));

	// SCROLL ====================================
	scroll = std::make_unique<gui::Scroll>(sf::Vector2f(800.f, 200.f), sf::Vector2f(20.f, 200.f));

	textBox = std::make_unique<gui::TextBox>(sf::Vector2f(100.f, 600.f), sf::Vector2f(400.f, 20.f));

	textBox2 = std::make_unique<gui::TextBox>(sf::Vector2f(500.f, 200.f), sf::Vector2f(200.f, 20.f));
}
