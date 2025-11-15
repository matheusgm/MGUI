#include "../stdafx.hpp"
#include "ListView.hpp"

gui::ListView::ListView(const sf::Vector2f &position, const sf::Vector2f &size, std::unique_ptr<const IListViewAdapter> adapter)
	: GuiElement(position), m_adapter(std::move(adapter)), m_viewport(position, size)
{
	m_background.setSize(size);
	m_background.setFillColor(sf::Color::Yellow);
	m_background.setOutlineThickness(1.f);
	m_background.setOutlineColor(sf::Color::Black);

	for (size_t i = 0; i < MAX_VIEWS_IN_BUFFER; ++i)
		m_viewBuffer.push_back(m_adapter->createView());

	float scrollWidth = 20.0f;

	m_scrollBar = std::make_unique<gui::Scroll>(sf::Vector2f(size.x - scrollWidth, 0.0f), sf::Vector2f(scrollWidth, size.y));

	m_scrollBar->onValueChange([this]()
							   { this->calculateScrollLayout(); });

	calculateScrollLayout();
	setupScrollBar();
}

void gui::ListView::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	sf::Vector2f listViewLocalMousePos = mapGlobalToLocal(mousePos);

	if (m_scrollBar)
	{
		m_scrollBar->updateEvents(sfEvent, listViewLocalMousePos);
	}

	if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (getGlobalBounds().contains(mousePos) && m_scrollBar)
		{
			int scrollValue = m_scrollBar->getValue();
			scrollValue -= static_cast<int>(mouseEvent->delta);

			m_scrollBar->setValue(scrollValue);

			calculateScrollLayout();
		}
	}
}

void gui::ListView::update(const sf::Vector2f &mousePos)
{
	sf::Vector2f listViewLocalMousePos = mapGlobalToLocal(mousePos);

	if (m_scrollBar)
	{
		m_scrollBar->update(listViewLocalMousePos);
	}
}

sf::FloatRect gui::ListView::getGlobalBounds() const
{
	sf::FloatRect localBounds = m_background.getLocalBounds();
	return getTransform().transformRect(localBounds);
}

void gui::ListView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(m_background, states);

	sf::View oldView = target.getView();

	sf::Vector2f size = m_background.getSize();
	sf::Vector2u targetSize = target.getSize();

	sf::Vector2f globalPosition = states.transform.transformPoint({0.f, 0.f});

	sf::FloatRect viewport({globalPosition.x / (float)targetSize.x,
							globalPosition.y / (float)targetSize.y},
						   {size.x / (float)targetSize.x,
							size.y / (float)targetSize.y});

	sf::View renderView(sf::FloatRect({0.f, 0.f}, {size.x, size.y}));
	renderView.setViewport(viewport);
	target.setView(renderView);

	// Limita o loop ao menor valor: itens visíveis OU o tamanho do buffer fixo (20)
	size_t loopLimit = std::min(static_cast<size_t>(m_itemsToShow), MAX_VIEWS_IN_BUFFER);
	float itemHeight = m_adapter->getItemHeight();

	// 3. FASE DE BIND E DESENHO
	for (size_t i = 0; i < loopLimit; ++i)
	{
		size_t dataIndex = m_firstVisibleItem + i;

		if (dataIndex >= m_adapter->getItemCount())
			continue;

		ListViewItem *currentView = m_viewBuffer[i].get();
		m_adapter->updateView(*currentView, dataIndex);

		sf::Vector2f itemPosition(0.f, (i * itemHeight) - (m_scrollOffset - (m_firstVisibleItem * itemHeight)));
		currentView->setPosition(itemPosition);

		target.draw(*currentView, sf::RenderStates::Default);
	}

	target.setView(oldView);

	if (m_scrollBar)
		target.draw(*m_scrollBar, states);
}

void gui::ListView::calculateScrollLayout()
{
	float itemHeight = m_adapter->getItemHeight();

	// 1. Deriva o m_scrollOffset do valor inteiro do Scrollbar (Sincronização)
	if (m_scrollBar)
	{
		int scrollValue = m_scrollBar->getValue();
		m_scrollOffset = static_cast<float>(scrollValue) * itemHeight;

		m_scrollOffset = std::max(0.0f, m_scrollOffset);
	}

	// 2. Calcula os índices de desenho (Cache)
	if (m_viewport.size.y > 0 && itemHeight > 0)
	{
		m_firstVisibleItem = static_cast<int>(m_scrollOffset / itemHeight);
		m_itemsToShow = static_cast<int>(m_viewport.size.y / itemHeight) + 1;
	}
	else
	{
		m_firstVisibleItem = 0;
		m_itemsToShow = 0;
	}
}

void gui::ListView::setupScrollBar()
{
	if (!m_scrollBar)
		return;

	float itemHeight = m_adapter->getItemHeight();
	size_t itemCount = m_adapter->getItemCount();

	int maxIndex = 0;

	if (itemCount > 0 && itemHeight > 0)
	{
		int itemsInView = static_cast<int>(m_viewport.size.y / itemHeight);
		maxIndex = std::max(0, static_cast<int>(itemCount) - itemsInView);
	}

	// 1. Atualiza os limites (Range) do Scroll
	m_scrollBar->setMinValue(0);
	m_scrollBar->setMaxValue(maxIndex);

	// 2. Sincroniza o valor inicial do Scrollbar com o offset
	// Usamos round() para maior precisão na conversão float->int
	m_scrollBar->setValue(static_cast<int>(std::round(m_scrollOffset / itemHeight)));

	// 3. Calcula o tamanho do indicador (Thumb) com base na proporção
	float totalContentHeight = itemCount * itemHeight;
	float contentRatio = m_viewport.size.y / totalContentHeight;

	m_scrollBar->setIndicatorHeightRatio(contentRatio);
}
