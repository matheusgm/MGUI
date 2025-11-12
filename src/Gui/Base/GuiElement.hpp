#pragma once

namespace gui
{
	class GuiElement : public sf::Drawable, public sf::Transformable
	{
	public:
		GuiElement(sf::Vector2f position, sf::Vector2f size);
		virtual ~GuiElement() = default;

		const float getTop() const { return getGlobalBounds().position.y; }
		const float getBottom() const { return getGlobalBounds().position.y + getGlobalBounds().size.y; }
		const float getLeft() const { return getGlobalBounds().position.x; }
		const float getRight() const { return getGlobalBounds().position.x + getGlobalBounds().size.x; }

		bool contains(const sf::Vector2f &points) const;

		virtual void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) = 0;
		virtual void update(const sf::Vector2f &mousePos) = 0;
		virtual sf::FloatRect getGlobalBounds() const = 0;
		virtual void setSize(const sf::Vector2f &newSize) {}; // Optional to implement

	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
	};
}
