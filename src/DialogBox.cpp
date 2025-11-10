#include "stdafx.h"
#include "DialogBox.h"

std::unique_ptr<sf::Font> gui::DialogBox::defaultFont;

gui::DialogBox::DialogBox(float x, float y, float width, float height, std::map<std::string, gui::Button *> buttons)
	: BaseGui(sf::Vector2f(x, y), sf::Vector2f(width, height)),
	  text(loadFont())
{
	// Shape
	this->shape.setFillColor(sf::Color::Red);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(sf::Color::Black);

	// Text
	this->text.setFillColor(sf::Color::Black);
	this->text.setCharacterSize(24U);

	this->setSize(width, height);
	this->setPosition(x, y);

	// Close Button
	this->closeButton = gui::Button(
		this->getLeft() + 10.f, this->getBottom() - 35.f,
		100.f, 25.f, "Close", 16U);
}

sf::Font &gui::DialogBox::loadFont()
{
	if (defaultFont == nullptr)
	{
		defaultFont = std::make_unique<sf::Font>();
		if (!defaultFont->openFromFile("src/Fonts/MochiyPopPOne-Regular.ttf"))
			throw std::runtime_error("Failed to load default font!");
	}

	return *defaultFont;
}

// gui::DialogBox* gui::DialogBox::CreateOkDialog(float x, float y, float w, float h) {
//	std::map<std::string, gui::Button*> buttons;
//	buttons["OK"] = new gui::Button(w - 90.f, h - 32.f, 80.f, 24.f, "Ok", 20U);
//	return new DialogBox(x, y, w, h, buttons);
// }
//
// gui::DialogBox* gui::DialogBox::CreateYesNoDialog(float x, float y, float w, float h) {
//	std::map<std::string, gui::Button*> buttons;
//	buttons["YES"] = new gui::Button(w - 80.f - 100.f, h - 32.f, 80.f, 24.f, "Yes", 20U);
//	buttons["NO"] = new gui::Button(w - 90.f, h - 32.f, 80.f, 24.f, "No", 20U);
//	return new DialogBox(x, y, w, h, buttons);
// }
//
// gui::DialogBox* gui::DialogBox::CreateNextPrevDialog(float x, float y, float w, float h) {
//	std::map<std::string, gui::Button*> buttons;
//	buttons["PREVIOUS"] = new gui::Button(w - 80.f - 100.f, h - 32.f, 80.f, 24.f, "Previous", 20U);
//	buttons["NEXT"] = new gui::Button(w - 90.f, h - 32.f, 80.f, 24.f, "Next", 20U);
//	return new DialogBox(x, y, w, h, buttons);
// }

void gui::DialogBox::setPosition(const float x, const float y)
{
	this->shape.setPosition(sf::Vector2f(x, y));

	this->text.setPosition({this->getLeft() + 20.f, this->getTop() + 20.f});
}

void gui::DialogBox::setSize(const float width, const float height)
{
	this->shape.setSize(sf::Vector2f(width, height));
}

void gui::DialogBox::loadNode(std::shared_ptr<DialogNode> node)
{
	message = node->message;
	currentIndex = 0;
	dialogType = node->type;
	updateText();

	buttons.clear();

	for (const auto &option : node->options)
	{
		const std::string &label = option.first;
		gui::Button btn(0.f, 0.f, 100.f, 30.f, label);
		btn.onPressed([this, label]()
					  {
			if (choiceCallback) choiceCallback(label); });
		buttons.push_back(btn);
	}
}

void gui::DialogBox::setChoiceCallback(std::function<void(const std::string &)> callback)
{
	choiceCallback = callback;
}

void gui::DialogBox::updateText()
{
	if (message.size() > 0)
	{
		text.setString(message);
	}
}

void gui::DialogBox::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	closeButton.updateEvents(sfEvent, mousePos);

	for (auto &btn : this->buttons)
	{
		btn.updateEvents(sfEvent, mousePos);
	}
}

void gui::DialogBox::update(const sf::Vector2f &mousePos)
{
	closeButton.update(mousePos);

	for (auto &btn : this->buttons)
	{
		btn.update(mousePos);
	}
}

void gui::DialogBox::render(sf::RenderTarget &target)
{
	target.draw(this->shape);
	target.draw(this->text);

	closeButton.render(target);

	float x = this->shape.getPosition().x + this->shape.getSize().x - (buttons.size() * 110);
	float y = this->shape.getPosition().y + this->shape.getSize().y - 40;

	for (auto &btn : this->buttons)
	{
		btn.setPosition(x, y);
		btn.render(target);
		x += 110;
	}
}
