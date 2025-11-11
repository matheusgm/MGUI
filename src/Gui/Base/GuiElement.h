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
		sf::Vector2f getPosition() const { return rect.position; };
		sf::Vector2f getSize() const { return rect.size; };
		float getTop() const { return rect.position.y; };
		float getBottom() const { return rect.position.y + rect.size.y; };
		float getLeft() const { return rect.position.x; };
		float getRight() const { return rect.position.x + rect.size.x; };
		float getWidth() const { return rect.size.x; };
		float getHeight() const { return rect.size.y; };

		// Modifier
		virtual void setPosition(float x, float y);
		virtual void setSize(float width, float height);

		// Functions
		bool contains(const sf::Vector2f &points) { return rect.contains(points); };
		virtual void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) = 0;
		virtual void update(const sf::Vector2f &mousePos) = 0;
		virtual void render(sf::RenderTarget &target) = 0;
	};
}
