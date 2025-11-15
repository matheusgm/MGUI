#pragma once

namespace gui
{
	class GuiElement : public sf::Drawable, public sf::Transformable
	{
	public:
		GuiElement(sf::Vector2f position);
		virtual ~GuiElement() = default;

		virtual void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) = 0;
		virtual void update(const sf::Vector2f &mousePos) = 0;

		virtual sf::FloatRect getLocalBounds() const = 0;
		virtual sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }
		virtual void setSize(const sf::Vector2f &newSize) {}; // Optional to implement

		bool contains(const sf::Vector2f &points) const { return getGlobalBounds().contains(points); }
		sf::FloatRect RectUnion(const sf::FloatRect &a, const sf::FloatRect &b) const;

		const float getTop() const { return getGlobalBounds().position.y; }
		const float getBottom() const { return getGlobalBounds().position.y + getGlobalBounds().size.y; }
		const float getLeft() const { return getGlobalBounds().position.x; }
		const float getRight() const { return getGlobalBounds().position.x + getGlobalBounds().size.x; }

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;

		sf::Vector2f mapGlobalToLocal(const sf::Vector2f &globalMousePos) const { return getInverseTransform().transformPoint(globalMousePos); };
	};
}
