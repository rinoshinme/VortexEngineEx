#include "window.h"
#include "stb_image_write.h"

namespace vortex
{
    void windowErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "GLFW error: %d msg: %s\n", error, description);
    }

    Window::Window(const WindowProp& props)
        : m_Title(props.title), m_Width(props.width), m_Height(props.height)
    {
        glfwSetErrorCallback(windowErrorCallback);
        if (!glfwInit())
        {
            fprintf(stderr, "ERROR: could not initialize GLFW\n");
            exit(-1);
        }

        // initialize glfw
#ifdef MAC
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        glfwWindowHint(GLFW_SAMPLES, 4);
        if (!props.fullScreen)
        {
            m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
        }
        else
        {
            // full screen support.
            GLFWmonitor* mon = glfwGetPrimaryMonitor();
            const GLFWvidmode* vmode = glfwGetVideoMode(mon);
            m_Window = glfwCreateWindow(vmode->width, vmode->height, "FullScreen Hello", mon, NULL);
            m_Width = vmode->width;
            m_Height = vmode->height;
        }

        glfwMakeContextCurrent(m_Window);

        // initialize glew
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return;
        }
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::Resize(int width, int height)
    {
        m_Width = width;
        m_Height = height;
        glViewport(0, 0, m_Width, m_Height);
    }

    bool Window::Closed() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::Shutdown() const
    {
        glfwSetWindowShouldClose(m_Window, 1);
    }

    std::pair<int, int> Window::WindowDimension() const
    {
        return std::make_pair(m_Width, m_Height);
    }

    void Window::SetWindowTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    void Window::Update() const
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void Window::CaptureScreen(const std::string& filePath)
    {
        uint32_t buffer_size = m_Width * m_Height * 3;
        unsigned char* buffer = new unsigned char[buffer_size];
        glReadPixels(0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        int stride = 3 * m_Width;
        unsigned char* last_line = buffer + (m_Height - 1) * stride;
        // from bottom to top
        stbi_write_png(filePath.c_str(), m_Width, m_Height, 3, last_line, -stride);
        delete[] buffer;
    }
}
