#pragma once

class WindowManager
{
    public:
        virtual void closeWindow(void) = 0;
        virtual void toggleFullscreen(void) = 0;
};
