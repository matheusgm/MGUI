#pragma once

#include "Base/GuiElement.hpp"

namespace gui
{
    class ListViewItem : public GuiElement
    {
    public:
        ListViewItem();
        virtual ~ListViewItem() = default;

        template <typename T>
        void updateWithData(const T &data, size_t index);
    };
}