#include "state.h"

#include <GL/glew.h>

namespace vortex
{
    void GraphicsState::EnableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void GraphicsState::DisableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }
}
