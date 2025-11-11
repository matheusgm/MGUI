#pragma once

namespace gui
{
	class GuiElement
	{

	private:
		sf::FloatRect rect;
	public:
		GuiElement(sf::Vector2f position, sf::Vector2f size);
		virtual ~GuiElement() = default;

		// Acessors
		const sf::Vector2f getPosition() const;
		const sf::Vector2f getSize() const;
		const float getTop() const;
		const float getBottom() const;
		const float getLeft() const;
		const float getRight() const;
		const float getWidth() const;
		const float getHeight() const;

		// Modifier
		virtual void setPosition(const float x, const float y);
		virtual void setSize(const float width, const float height);

		// Functions
		bool contains(const sf::Vector2f& points);
		virtual void updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos) = 0;
		virtual void update(const sf::Vector2f& mousePos) = 0;
		virtual void render(sf::RenderTarget& target) = 0;
	};
}

