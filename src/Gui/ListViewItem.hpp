#pragma once

#include "Base/GuiElement.h"

namespace gui
{
    class ListViewItem
    {
    public:
        ListViewItem() = default;
        virtual ~ListViewItem() = default;

        template <typename T>
        void updateWithData(const T &data, size_t index);

        virtual void setPosition(const float x, const float y) = 0;

        virtual void render(sf::RenderTarget &target) = 0;
    };
}