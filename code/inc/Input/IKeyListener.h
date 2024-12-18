#pragma once

class IKeyListener
{
    public:
        virtual ~IKeyListener() {}
        virtual void onKeyPressed(int key) = 0;
        virtual void onKeyReleased(int key) = 0;
};