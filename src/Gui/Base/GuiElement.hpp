#pragma once

// Declaração forward para evitar dependência circular
namespace gui
{
	class Canvas;
}

namespace gui
{
	class GuiElement : public sf::Drawable, public sf::Transformable
	{
	public:
		GuiElement(sf::Vector2f position);
		virtual ~GuiElement() = default;

		virtual void update(sf::Time deltaTime) = 0;

		virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) = 0;

		virtual sf::FloatRect getLocalBounds() const = 0;
		virtual sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }
		bool contains(const sf::Vector2f &points) const { return getGlobalBounds().contains(points); }

		sf::FloatRect RectUnion(const sf::FloatRect &a, const sf::FloatRect &b) const;

		const float getTop() const { return getGlobalBounds().position.y; }
		const float getBottom() const { return getGlobalBounds().position.y + getGlobalBounds().size.y; }
		const float getLeft() const { return getGlobalBounds().position.x; }
		const float getRight() const { return getGlobalBounds().position.x + getGlobalBounds().size.x; }

		unsigned int getId() const { return m_id; }

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;

		sf::Vector2f mapGlobalToLocal(const sf::Vector2f &globalMousePos) const { return getInverseTransform().transformPoint(globalMousePos); };

	private:
		// friend class Canvas; // Concede acesso total aos membros privados e protegidos ao Canvas
		static std::atomic<unsigned int> s_nextId;
		const unsigned int m_id;
	};
}
