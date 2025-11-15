#include "../stdafx.hpp"
#include "DialogBox.hpp"

std::unique_ptr<sf::Font> gui::DialogBox::defaultFont;

gui::DialogBox::DialogBox(sf::Vector2f position, sf::Vector2f size)
	: GuiElement(position),
	  text(loadFont())
{
	// Shape
	shape.setSize(size);
	shape.setFillColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);
	shape.setPosition(position);

	// Text
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(24U);

	// Close Button
	closeButton = Button(
		{position.x + 10.f, (position.y + size.y) - 35.f},
		{100.f, 25.f}, "Close", 16U);
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

sf::FloatRect gui::DialogBox::getGlobalBounds() const
{
	return shape.getGlobalBounds();
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
		gui::Button btn({x, y}, {btnWidth, btnHeight}, label);
		btn.onPressed([this, label]()
					  { if (choiceCallback) choiceCallback(label); });
		buttons.emplace_back(std::move(btn));
		x += btnWidth + btnSpacing;
	}
}

void gui::DialogBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(shape, states);
	target.draw(text, states);

	target.draw(closeButton, states);

	for (auto &btn : buttons)
		target.draw(btn, states);

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