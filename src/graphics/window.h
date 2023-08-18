/*
* window should be created before glew initialization.
*/
#pragma once

#include <cstdint>
#include <string>
#include <GLFW/glfw3.h>

namespace vortex
{
    struct WindowProp
    {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool fullScreen;
        bool vsync;
    };

    class Window
    {
    private:
        uint32_t m_Width;
        uint32_t m_Height;
        GLFWwindow* m_Window;

    public:
        Window(const WindowProp& props)
        {
            
        }

        void SetCallback();
    };
}
