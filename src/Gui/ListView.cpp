#include "../stdafx.hpp"
#include "ListView.hpp"

void gui::ListView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(m_background, states);

	sf::View oldView = target.getView();

	sf::Vector2f position = getPosition();
	sf::Vector2f size = m_background.getSize();

	sf::View renderView(sf::FloatRect(position, size));

	sf::FloatRect viewport({position.x / target.getSize().x,
							position.y / target.getSize().y},
						   {size.x / target.getSize().x,
							size.y / target.getSize().y});

	renderView.setViewport(viewport);

	target.setView(renderView);

	// 2. Preparação e Cálculo
	float itemHeight = m_adapter->getItemHeight();
	int firstVisibleItem = static_cast<int>(m_scrollOffset / itemHeight);
	int itemsToShow = static_cast<int>(m_viewport.size.y / itemHeight) + 1;

	// Limita o loop ao menor valor: itens visíveis OU o tamanho do buffer fixo (20)
	size_t loopLimit = std::min(static_cast<size_t>(itemsToShow), MAX_VIEWS_IN_BUFFER);

	// 3. FASE DE BIND E DESENHO
	for (size_t i = 0; i < loopLimit; ++i)
	{
		size_t dataIndex = firstVisibleItem + i; // Índice do dado na lista completa (0 a 999)

		if (dataIndex >= m_adapter->getItemCount())
			continue;

		// Acessa o componente visual no índice 'i' do nosso buffer fixo (0 a 19)
		ListViewItem *currentView = m_viewBuffer[i].get();

		// **ATUALIZAÇÃO DO CONTEÚDO (O Bind)**
		// O componente [i] do buffer é atualizado com o dado [dataIndex].
		m_adapter->updateView(*currentView, dataIndex);

		// Calcula a posição na tela
		sf::Vector2f itemPosition(
			m_viewport.position.x,
			m_viewport.position.y + (i * itemHeight) - (m_scrollOffset - (firstVisibleItem * itemHeight)));

		currentView->setPosition(itemPosition);
		target.draw(*currentView, states);
	}

	target.setView(oldView);
}

gui::ListView::ListView(const sf::Vector2f &position, const sf::Vector2f &size, std::unique_ptr<const IListViewAdapter> adapter)
	: GuiElement(position, size), m_adapter(std::move(adapter)), m_viewport(position, size)
{
	m_background.setPosition(position);
	m_background.setSize(size);
	m_background.setFillColor(sf::Color::Yellow);
	m_background.setOutlineThickness(1.f);
	m_background.setOutlineColor(sf::Color::Black);

	for (size_t i = 0; i < MAX_VIEWS_IN_BUFFER; ++i)
		m_viewBuffer.push_back(m_adapter->createView());
}

void gui::ListView::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (m_viewport.contains({static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y)}))
		{
			float itemHeight = m_adapter->getItemHeight();
			float scrollDelta = mouseEvent->delta * itemHeight;
			m_scrollOffset -= scrollDelta;

			float totalContentHeight = m_adapter->getItemCount() * itemHeight;
			float maxScroll = totalContentHeight - m_viewport.size.y;
			if (maxScroll < 0)
				maxScroll = 0;

			if (m_scrollOffset < 0)
				m_scrollOffset = 0;
			if (m_scrollOffset > maxScroll)
				m_scrollOffset = maxScroll;
		}
	}
}

void gui::ListView::update(const sf::Vector2f &mousePos)
{
}

sf::FloatRect gui::ListView::getGlobalBounds() const
{
	return m_background.getGlobalBounds();
}
