#pragma once


namespace vortex
{
#define GL_LOG_FILE "gl.log"

    class GLLogging
    {
    public:
        static bool Restart();
        static bool Log(const char* message, ...);
        static bool LogError(const char* message, ...);
        static void LogGLParams();
    };
}
