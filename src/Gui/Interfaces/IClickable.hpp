#pragma once

namespace gui
{
    class IClickable
    {
    public:
        virtual ~IClickable() = default;
        virtual void executeClickAction() = 0;
    };
}