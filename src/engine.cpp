#include "engine.h"
#include <glm/glm.hpp>
#include "utils/utils.h"

namespace vortex
{
    Engine::Engine()
    {
        WindowProp props { "VortexEngine", 640, 480, false, false };
        m_Window = std::make_shared<Window>(props);
        m_AudioManager = std::make_shared<AudioManager>();
        m_FpsCounter = std::make_shared<FpsCounter>();
        m_Camera = std::make_shared<Camera>();

        std::pair<int, int> window_dimension = m_Window->WindowDimension();
        float fov = glm::radians(67.0f);
        float near = 0.1f;
        float far = 100.0f;
        m_Frustum = std::make_shared<PerspectiveFrustum>(fov, near, far, window_dimension.first, window_dimension.second);
        // m_Frustum = std::make_shared<OrthogonalFrustum>(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);

        // initialize resources
        m_Mesh = std::make_shared<Mesh>();
        m_Mesh->LoadFromFile("F:/VortexLibs/VortexEngine/resources/objs/monkey.obj");
        const float* vertex_data = m_Mesh->DataPtr();

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, m_Mesh->NumVertices() * sizeof(vortex::Vertex), vertex_data, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vortex::Vertex), NULL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vortex::Vertex), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vortex::Vertex), (void*)(5 * sizeof(float)));

        std::vector<vortex::ShaderSource> sources = {
            {
                vortex::ShaderType::SHADER_TYPE_VERTEX,
                "F:/VortexLibs/VortexEngine/resources/shaders/basic.vs"
            },
            {
                vortex::ShaderType::SHADER_TYPE_PIXEL,
                "F:/VortexLibs/VortexEngine/resources/shaders/basic.fs"
            }
        };
        m_Shader = vortex::Shader::FromSources(sources);
        m_Texture = vortex::Texture2D::FromFile(vortex::GetResourcePath("textures/ao.png"));

        std::vector<std::string> cubemapTexturePaths = {
            vortex::GetResourcePath("textures/cubemap/negz.jpg"),
            vortex::GetResourcePath("textures/cubemap/posz.jpg"),
            vortex::GetResourcePath("textures/cubemap/posy.jpg"),
            vortex::GetResourcePath("textures/cubemap/negy.jpg"),
            vortex::GetResourcePath("textures/cubemap/negx.jpg"),
            vortex::GetResourcePath("textures/cubemap/posx.jpg"),
        };
        std::string cubemapVertexShader = vortex::GetResourcePath("shaders/skybox.vs");
        std::string cubemapFragmentShader = vortex::GetResourcePath("shaders/skybox.fs");
        m_SkyBox = std::make_shared<SkyBox>(cubemapTexturePaths, 0, cubemapVertexShader, cubemapFragmentShader);

        glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void Engine::Update()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update window dimension for viewport.
        std::pair<int, int> size = m_Window->WindowDimension();
        m_Frustum->SetWindowSize(size.first, size.second);

        glm::mat4 view = m_Camera->GetViewMatrix();
        glm::mat4 view1 = m_Camera->GetViewMatrixOrigin();
        glm::mat4 proj = m_Frustum->GetProjectionMatrix();
        glm::mat4 model(1.0f);

        m_SkyBox->Draw(view1, proj);

        m_Shader->Bind();
        m_Shader->SetUniformMatrix4("view", &view[0][0]);
        m_Shader->SetUniformMatrix4("proj", &proj[0][0]);
        m_Shader->SetUniformMatrix4("model", &model[0][0]);
        
        m_Texture->Bind(0);
        m_Shader->SetUniformInt("basic_texture", 0);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_Mesh->NumVertices());

        m_Window->Update();
    }

    void Engine::Run()
    {
        while (!m_Window->Closed())
        {
            static double previous_seconds = glfwGetTime();
            double current_seconds = glfwGetTime();
            float elapsed_seconds = static_cast<float>(current_seconds - previous_seconds);
            previous_seconds = current_seconds;

            ProcessEvents(elapsed_seconds);
            Update();                
        }
    }

    void Engine::ProcessEvents(float elapsed_seconds)
    {
        float cam_speed = 1.0f; // 1 unit per second
        float cam_yaw_speed = 10.0f; // 10 degrees per second
        GLFWwindow* window = m_Window->Handle();

        if (glfwGetKey(window, GLFW_KEY_A)) {
            // cam_pos[0] -= cam_speed * elapsed_seconds;
            m_Camera->Translate(-cam_speed * elapsed_seconds, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            m_Camera->Translate(cam_speed * elapsed_seconds, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP)) {
            m_Camera->Translate(0, cam_speed * elapsed_seconds, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN)) {
            m_Camera->Translate(0, -cam_speed * elapsed_seconds, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            m_Camera->Translate(0, 0, cam_speed * elapsed_seconds);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            m_Camera->Translate(0, 0, -cam_speed * elapsed_seconds);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            m_Camera->Rotate(cam_yaw_speed * elapsed_seconds, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            m_Camera->Rotate(-cam_yaw_speed * elapsed_seconds, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_H))
        {
            m_Camera->Home();
        }

        // window cleared each frame, so this should be placed at the bottom
        if (glfwGetKey(window, GLFW_KEY_P))
        {
            m_Window->CaptureScreen("screen.png");
        }
    }

}

