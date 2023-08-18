#pragma once

#include "graphics/window.h"
#include "audio/audio_manager.h"
#include "graphics/camera.h"
#include "core/frustum.h"
#include "utils/timer.h"
#include "graphics/api.h"
#include "graphics/mesh.h"

namespace vortex
{
    class Engine
    {
    private:
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<AudioManager> m_AudioManager;
        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<Frustum> m_Frustum;
        std::shared_ptr<FpsCounter> m_FpsCounter;


        GLuint m_VAO;
        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Texture2D> m_Texture;

    public:
        Engine();
        void Run();
        void Update();
        void ProcessEvents(float elapsed_seconds);
        
    private:
    };
}
