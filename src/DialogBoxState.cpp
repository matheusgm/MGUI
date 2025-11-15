#include "stdafx.hpp"
#include "DialogBoxState.hpp"
#include "Gui/DialogTree.hpp"

DialogBoxState::DialogBoxState(StateData &state_data)
	: State(state_data)
{
	initVariables();
	initKeybinds();
	initGui();
	onResizeWindow();

	background.setFillColor(sf::Color::Green);
}

void DialogBoxState::updateKeyboardInput(sf::Event &sfEvent)
{
}

void DialogBoxState::updateEvents(sf::Event &sfEvent)
{
	updateKeyboardInput(sfEvent);
	for (auto &it : buttons)
		it.second->updateEvents(sfEvent, mousePosView);

	dialogBox->updateEvents(sfEvent, mousePosView);
}

void DialogBoxState::onResizeWindow()
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
}

void DialogBoxState::updateGui(float dt) const
{
	/* Updates all the gui in the state and handles their functionality */
	// Buttons
	for (auto &it : buttons)
		it.second->update(mousePosView);

	dialogBox->update(mousePosView);
}

void DialogBoxState::update(float dt)
{
	updateMousePositions();

	updateGui(dt);
}

void DialogBoxState::renderGui(sf::RenderTarget &target) const
{
	for (auto &it : buttons)
		target.draw(*it.second);

	target.draw(*dialogBox);
}

void DialogBoxState::render(sf::RenderTarget &target)
{
	target.draw(background);

	renderGui(target);
}

void DialogBoxState::initVariables()
{
	// modes = sf::VideoMode::getFullscreenModes();
}

void DialogBoxState::initKeybinds()
{

	/*std::ifstream ifs("src/Config/mainmenustate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			keybinds[key] = supportedKeys->at(key2);
		}
	}

	ifs.close();*/
}

void DialogBoxState::initGui()
{
	buttons["BACK"] = std::make_unique<gui::Button>(
		sf::Vector2f(100.f, 100.f),
		sf::Vector2f(150.f, 50.f), "Back", 32);

	// Button functionality
	// Quit the game
	buttons["BACK"]->onPressed([this]
							   { endState(); });

	// DIALOG BOX ====================================

	// Criar nós
	auto root = std::make_shared<DialogNode>();
	root->message = "Voce encontrou um bau misterioso.";
	root->type = DialogType::YesNo;

	auto nodeYes = std::make_shared<DialogNode>();
	nodeYes->message = "Voce abriu o bau...";
	nodeYes->type = DialogType::OK;

	auto nodeNo = std::make_shared<DialogNode>();
	nodeNo->message = "Voce ignorou o bau e seguiu em frente.";
	nodeNo->type = DialogType::OK;

	root->options["Yes"] = nodeYes;
	root->options["No"] = nodeNo;

	dialogTree = std::make_unique<gui::DialogTree>(root);
	dialogBox = std::make_unique<gui::DialogBox>(sf::Vector2f(150.f, 200.f), sf::Vector2f(600.f, 250.f));
	dialogBox->setPosition({150, 200});
	dialogBox->loadNode(dialogTree->current());

	dialogBox->setChoiceCallback([&](const std::string &choice)
								 {
		dialogTree->choose(choice);
		dialogBox->loadNode(dialogTree->current()); });

	// std::vector<std::string> dialogTexts = { "Texto 1", "Texto 2", "Texto 3" };
	// size_t currentDialogIndex = 0;
	// dialogBox = gui::DialogBox::CreateNextPrevDialog(200.f, 200.f, 600.f, 200.f);
	// dialogBox->setText(dialogTexts.back());

	// Callback do Next
	// dialogBox->getButton("Next")->onPressed([this] {
	//	if (currentDialogIndex + 1 < dialogTexts.size() - 1) {
	//		currentDialogIndex++;
	//		dialogBox->setText(dialogTexts[currentDialogIndex]);
	//	}
	//	else {
	//		// �ltimo texto: troca para Yes/No
	//		delete dialogBox;
	//		dialogBox = gui::DialogBox::CreateYesNoDialog(200.f, 200.f, 600.f, 200.f);
	//		dialogBox->setText(dialogTexts.back());
	//		// Adicione callbacks para Yes/No
	//	}
	//	});

	// Callback do Previous
	/*dialogBox->getButton("Previous")->onPressed([this] {
		if (currentDialogIndex > 0) {
			currentDialogIndex--;
			dialogBox->setText(dialogTexts[currentDialogIndex]);
		}
		});*/
}
