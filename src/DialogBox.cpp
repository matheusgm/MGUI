#include "stdafx.h"
#include "DialogBox.h"

std::unique_ptr<sf::Font> gui::DialogBox::defaultFont;

gui::DialogBox::DialogBox(float x, float y, float width, float height)
	: GuiElement({x, y}, {width, height}),
	  text(loadFont())
{
	// Shape
	shape.setPosition({x, y});
	shape.setSize({width, height});
	shape.setFillColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	// Text
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(24U);

	// Close Button
	closeButton = gui::Button(
		getLeft() + 10.f, getBottom() - 35.f,
		100.f, 25.f, "Close", 16U);

	setPosition(x, y);
}

void gui::DialogBox::updateText(const std::string &textStr)
{
	text.setString(textStr);
};

sf::Font &gui::DialogBox::loadFont()
{
	if (!defaultFont)
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

void gui::DialogBox::setPosition(float x, float y)
{
	GuiElement::setPosition(x, y);
	shape.setPosition(getPosition());

	text.setOrigin({-20.f, -20.f});
	text.setPosition({getLeft(), getTop()});

	closeButton.setPosition(getLeft() + 10.f, getBottom() - 35.f);
}

void gui::DialogBox::setSize(float width, float height)
{
	GuiElement::setSize(width, height);
	shape.setSize(getSize());
	setPosition(getLeft(), getTop());
}

void gui::DialogBox::loadNode(const std::shared_ptr<DialogNode> &node)
{
	dialogType = node->type;
	updateText(node->message);

	buttons.clear();
	buttons.reserve(node->options.size());

	constexpr float btnWidth = 100.f;
	constexpr float btnHeight = 30.f;
	constexpr float btnSpacing = 10.f;
	constexpr float margin = 10.f;

	const float totalWidth = node->options.size() * (btnWidth + btnSpacing) - btnSpacing;

	float x = shape.getPosition().x + shape.getSize().x - totalWidth - margin;
	float y = shape.getPosition().y + shape.getSize().y - btnHeight - margin;

	for (const auto &[label, _] : node->options)
	{
		gui::Button btn(x, y, btnWidth, btnHeight, label);
		btn.onPressed([this, label]()
					  { if (choiceCallback) choiceCallback(label); });
		buttons.emplace_back(std::move(btn));
		x += btnWidth + btnSpacing;
	}
}

void gui::DialogBox::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	closeButton.updateEvents(sfEvent, mousePos);

	for (auto &btn : buttons)
		btn.updateEvents(sfEvent, mousePos);
}

void gui::DialogBox::update(const sf::Vector2f &mousePos)
{
	closeButton.update(mousePos);

	for (auto &btn : buttons)
		btn.update(mousePos);
}

void gui::DialogBox::render(sf::RenderTarget &target)
{
	target.draw(shape);
	target.draw(text);

	closeButton.render(target);

	for (auto &btn : buttons)
		btn.render(target);

	// sf::Vertex verticalLine[2];
	// verticalLine[0].position = sf::Vector2f(150.f, 0.f);
	// verticalLine[0].color = sf::Color::Red;
	// verticalLine[1].position = sf::Vector2f(150.f, 800.f);
	// verticalLine[1].color = sf::Color::Red;

	// sf::Vertex horizontalLine[2];
	// horizontalLine[0].position = sf::Vector2f(0.f, 200.f);
	// horizontalLine[0].color = sf::Color::Blue;
	// horizontalLine[1].position = sf::Vector2f(1200.f, 200.f);
	// horizontalLine[1].color = sf::Color::Blue;

	// target.draw(verticalLine, 2, sf::PrimitiveType::Lines);
	// target.draw(horizontalLine, 2, sf::PrimitiveType::Lines);
}
