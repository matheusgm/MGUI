#include "stdafx.h"
#include "DialogBoxState.h"
#include "DialogTree.h"

void DialogBoxState::initVariables()
{
	// this->modes = sf::VideoMode::getFullscreenModes();
}

void DialogBoxState::initKeybinds()
{

	/*std::ifstream ifs("src/Config/mainmenustate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();*/
}

void DialogBoxState::initGui()
{
	this->buttons["BACK"] = std::make_unique<gui::Button>(
		100.f, 100.f,
		150.f, 50.f, "Back", 32);

	// Button functionality
	// Quit the game
	this->buttons["BACK"]->onPressed([this]
									 { this->endState(); });

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

	this->dialogTree = std::make_unique<gui::DialogTree>(root);
	this->dialogBox = std::make_unique<gui::DialogBox>(150.f, 200.f, 600.f, 250.f);
	this->dialogBox->setPosition(150, 200);
	this->dialogBox->loadNode(dialogTree->current());

	this->dialogBox->setChoiceCallback([&](const std::string &choice)
									   {
		dialogTree->choose(choice);
		this->dialogBox->loadNode(dialogTree->current()); });

	// std::vector<std::string> dialogTexts = { "Texto 1", "Texto 2", "Texto 3" };
	// size_t currentDialogIndex = 0;
	// this->dialogBox = gui::DialogBox::CreateNextPrevDialog(200.f, 200.f, 600.f, 200.f);
	// this->dialogBox->setText(dialogTexts.back());

	// Callback do Next
	// this->dialogBox->getButton("Next")->onPressed([this] {
	//	if (currentDialogIndex + 1 < dialogTexts.size() - 1) {
	//		currentDialogIndex++;
	//		this->dialogBox->setText(this->dialogTexts[this->currentDialogIndex]);
	//	}
	//	else {
	//		// �ltimo texto: troca para Yes/No
	//		delete this->dialogBox;
	//		this->dialogBox = gui::DialogBox::CreateYesNoDialog(200.f, 200.f, 600.f, 200.f);
	//		this->dialogBox->setText(dialogTexts.back());
	//		// Adicione callbacks para Yes/No
	//	}
	//	});

	// Callback do Previous
	/*this->dialogBox->getButton("Previous")->onPressed([this] {
		if (currentDialogIndex > 0) {
			currentDialogIndex--;
			this->dialogBox->setText(this->dialogTexts[this->currentDialogIndex]);
		}
		});*/
}

DialogBoxState::DialogBoxState(StateData &state_data)
	: State(state_data)
{
	this->initVariables();
	this->initKeybinds();
	this->initGui();
	this->onResizeWindow();

	this->background.setFillColor(sf::Color::Green);
}

void DialogBoxState::updateKeyboardInput(sf::Event &sfEvent)
{
}

void DialogBoxState::updateEvents(sf::Event &sfEvent)
{
	this->updateKeyboardInput(sfEvent);
	for (auto &it : this->buttons)
		it.second->updateEvents(sfEvent, this->mousePosView);

	this->dialogBox->updateEvents(sfEvent, this->mousePosView);
}

void DialogBoxState::onResizeWindow()
{
	sf::Vector2f window_center = this->getWindowCenter();
	sf::Vector2u window_size = this->data.window->getSize();

	float gap = 50.f;

	// Background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(window_size.x),
			static_cast<float>(window_size.y)));

	// Buttons
	this->buttons["BACK"]->setPosition(window_size.x - 150.f - gap, window_size.y - 50.f - gap);
}

void DialogBoxState::updateGui(float dt) const
{
	/* Updates all the gui in the state and handles their functionality */
	// Buttons
	for (auto &it : this->buttons)
		it.second->update(this->mousePosView);

	this->dialogBox->update(this->mousePosView);
}

void DialogBoxState::update(float dt)
{
	this->updateMousePositions();

	this->updateGui(dt);
}

void DialogBoxState::renderGui(sf::RenderTarget &target) const
{
	for (auto &it : this->buttons)
		it.second->render(target);

	this->dialogBox->render(target);
}

void DialogBoxState::render(sf::RenderTarget &target)
{
	target.draw(this->background);

	this->renderGui(target);
}
