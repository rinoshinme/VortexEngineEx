#include "logging.h"
#include <stdio.h>
#include <time.h>
#include <varargs.h>
#include "GL/glew.h"


namespace vortex
{
    bool GLLogging::Restart()
    {
        FILE* file = fopen(GL_LOG_FILE, "w");
        if (!file)
        {
            fprintf(stderr, "ERROR: could not open log file %s for writing\n", GL_LOG_FILE);
            return false;
        }

        time_t now = time(NULL);
        char* date = ctime(&now);
        fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
        fclose(file);
        return true;
    }

    bool GLLogging::Log(const char* message, ...)
    {
        FILE* file = fopen(GL_LOG_FILE, "a");
        if (!file)
        {
            fprintf(stderr, "ERROR: could not open log file %s for writing\n", GL_LOG_FILE);
            return false;
        }

        va_list argptr;
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
        fclose(file);
        return true;
    }

    bool GLLogging::LogError(const char* message, ...)
    {
        FILE* file = fopen(GL_LOG_FILE, "a");
        if (!file)
        {
            fprintf(stderr, "ERROR: could not open log file %s for writing\n", GL_LOG_FILE);
            return false;
        }
        va_list argptr;
        
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);

        va_start(argptr, message);
        vfprintf(stderr, message, argptr);
        va_end(argptr);
        
        fclose(file);
        return true;
    }

    void GLLogging::LogGLParams()
    {
        GLenum params[] = {
            GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
            GL_MAX_CUBE_MAP_TEXTURE_SIZE,
            GL_MAX_DRAW_BUFFERS,
            GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
            GL_MAX_TEXTURE_IMAGE_UNITS,
            GL_MAX_TEXTURE_SIZE,
            GL_MAX_VARYING_FLOATS,
            GL_MAX_VERTEX_ATTRIBS,
            GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
            GL_MAX_VERTEX_UNIFORM_COMPONENTS,
            GL_MAX_VIEWPORT_DIMS,
            GL_STEREO,
        };
        const char* names[] = {
                "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
                "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
                "GL_MAX_DRAW_BUFFERS",
                "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
                "GL_MAX_TEXTURE_IMAGE_UNITS",
                "GL_MAX_TEXTURE_SIZE",
                "GL_MAX_VARYING_FLOATS",
                "GL_MAX_VERTEX_ATTRIBS",
                "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
                "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
                "GL_MAX_VIEWPORT_DIMS",
                "GL_STEREO",
        };
        GLLogging::Log("GL Context Params:\n");
        char msg[256];
        // integers - only works if the order is 0-10 integer return types
        for (int i = 0; i < 10; i++) {
            int v = 0;
            glGetIntegerv (params[i], &v);
            GLLogging::Log("%s %i", names[i], v);
        }
        // others
        int v[2];
        v[0] = v[1] = 0;
        glGetIntegerv (params[10], v);
        GLLogging::Log("%s %i %i\n", names[10], v[0], v[1]);
        unsigned char s = 0;
        glGetBooleanv (params[11], &s);
        GLLogging::Log("%s %u\n", names[11], (unsigned int)s);
        GLLogging::Log("-----------------------------\n");
    }
}
