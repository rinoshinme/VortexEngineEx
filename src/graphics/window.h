/*
* window should be the first to be created, and is 
* responsible for window initialization and glew initialization
*/
#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <GL/glew.h>
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
        std::string m_Title;
        uint32_t m_Width;
        uint32_t m_Height;
        GLFWwindow* m_Window;

    public:
        Window(const WindowProp& props);
        ~Window();

        void Resize(int width, int height);
        void SetWindowTitle(const std::string& title);
        GLFWwindow* Handle() const { return m_Window; }
        bool Closed() const;
        void Shutdown() const;
        void Update() const;
        std::pair<int, int> WindowDimension() const;

    public:
        // callbacks
    };
}
