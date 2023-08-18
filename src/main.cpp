#if 1
#include "engine.h"


int main()
{
    vortex::Engine engine;

    engine.Run();
}

#else

#include <stdio.h>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/utils.h"
#include "utils/logging.h"
#include "utils/math_funcs.h"
#include "utils/obj_loader.h"
#include "graphics/api.h"
#include "graphics/camera.h"
#include "core/ray.h"
#include "core/frustum.h"
#include "stb_image_write.h"
#include "graphics/mesh.h"


int g_WindowWidth = 640;
int g_WindowHeight = 480;
int g_SelectionIndex = -1;
float near = 0.1f; // clipping plane
float far = 100.0f; // clipping plane
float fov = float(67.0f * ONE_DEG_IN_RAD); // vertical viewing range
float aspect = (float)g_WindowWidth / (float)g_WindowHeight; // aspect ratio
vortex::Camera camera;

#define DRAW_SPHERES 0
#define DRAW_SQUARE 1

std::vector<glm::vec3> sphere_positions = {
    glm::vec3(0.0f, 0.0f, -5.0f),
    glm::vec3(0.0f, 2.0f, -4.0f),
    glm::vec3(1.5f, -1.0f, -10.0f),
    glm::vec3(-1.5f, -1.5f, -3.0f),
};

void glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
    g_WindowWidth = width;
    g_WindowHeight = height;
    glViewport(0, 0, g_WindowWidth, g_WindowHeight);
    aspect = (float)g_WindowWidth / (float)g_WindowHeight;
}

void glfw_error_callback(int error, const char* description)
{
    vortex::GLLogging::LogError("GLFW Error: code %i msg: %s\n", error, description);
}

void glfw_mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // get intersection index
        glm::vec3 p1 = glm::vec3(0.0f, 0.0f, 0.0f);
        float y_range = near * tan(fov / 2);
        float y = y_range * (g_WindowHeight / 2 - float(ypos)) / (g_WindowHeight / 2);
        float x_range = y_range * aspect;
        float x = x_range * (float(xpos) - g_WindowWidth / 2) / (g_WindowWidth / 2);
        glm::vec3 p2 = glm::vec3(x, y, -near);
        vortex::Ray ray(p1, p2);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 inv_view = glm::inverse(view);

        // transform ray into world coordinate
        // and do intersection test.
        ray.Transform(inv_view);
        float tmin = -1.0f;
        float sphere_radius = 1.0f;
        for (int i = 0; i < sphere_positions.size(); ++i)
        {
            vortex::Sphere s(sphere_positions[i], sphere_radius);
            float t = ray.Intersect(s);
            if (t < 0) continue;
            tmin = t;
            g_SelectionIndex = i;
        }
    }
}

void update_fps_counter(GLFWwindow* window)
{
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25)
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count += 1;
}

void capture_screen(const std::string& filePath)
{
    uint32_t buffer_size = g_WindowWidth * g_WindowHeight * 3;
    unsigned char* buffer = new unsigned char[buffer_size];
    glReadPixels(0, 0, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    // stb image 
    stbi_write_png(filePath.c_str(), g_WindowWidth, g_WindowHeight, 3, buffer, 3 * g_WindowWidth);
    delete[] buffer;
}

int main()
{
    assert(vortex::GLLogging::Restart());
    vortex::GLLogging::Log("starting GLFW\n%s\n", glfwGetVersionString());

    vortex::GLLogging::LogGLParams();
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3");
        return -1;
    }

#ifdef MAC
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

#if 1
    GLFWwindow* window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "Hello", NULL, NULL);
#else
    // full screen support.
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* vmode = glfwGetVideoMode(mon);
    GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "FullScreen Hello", mon, NULL);
#endif

    // set callbacks
    glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_click_callback);

    if (!window)
    {
        fprintf(stderr, "ERROR: could not create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version: %s\n", version);

    if (GLEW_ARB_uniform_buffer_object)
        printf("GLEW_ARB_uniform_buffer_object = YES\n");
    else
        printf("GLEW_ARB_uniform_buffer_object = NO\n");
    GLint blocks = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &blocks);
    printf("GL_MAX_UNIFORM_BUFFER_BINDINGS = %i\n", blocks);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

#ifdef DRAW_SQUARE

#endif


#if DRAW_SPHERES
    // define geometry
    std::vector<float> points;
    std::vector<float> texCoords;
    std::vector<float> normals;
    const std::string objFilePath("F:/VortexLibs/VortexEngine/resources/objs/sphere.obj");

    vortex::LoadObjFile(objFilePath, points, texCoords, normals);
    int numVertices = static_cast<int>(points.size() / 3);

    GLuint position_vbo;
    glGenBuffers(1, &position_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
    GLuint normal_vbo;
    glGenBuffers(1, &normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
#else
    vortex::Mesh mesh;
    mesh.LoadFromFile("F:/VortexLibs/VortexEngine/resources/objs/monkey.obj");
    const float* vertex_data = mesh.DataPtr();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.NumVertices() * sizeof(vortex::Vertex), vertex_data, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vortex::Vertex), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vortex::Vertex), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vortex::Vertex), (void*)(5 * sizeof(float)));
#endif

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
    std::shared_ptr<vortex::Shader> shader = vortex::Shader::FromSources(sources);

    // set config
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);

    std::shared_ptr<vortex::Texture2D> tex_diff = vortex::Texture2D::FromFile(vortex::GetResourcePath("/textures/boulder_diff.png"));
    std::shared_ptr<vortex::Texture2D> tex_spec = vortex::Texture2D::FromFile(vortex::GetResourcePath("/textures/boulder_spec.png"));
    std::shared_ptr<vortex::Texture2D> tex_amb = vortex::Texture2D::FromFile(vortex::GetResourcePath("/textures/ao.png"));
    std::shared_ptr<vortex::Texture2D> tex_emiss = vortex::Texture2D::FromFile(vortex::GetResourcePath("/textures/tileable9b_emiss.png"));
    tex_diff->Bind(0);
    tex_spec->Bind(1);
    tex_amb->Bind(2);
    tex_emiss->Bind(3);

    /* create VIEW MATRIX */
    float cam_speed = 1.0f; // 1 unit per second
    float cam_yaw_speed = 10.0f; // 10 degrees per second

    // draw
    glClearColor(0.3f, 0.5f, 0.1f, 1.0f);

    glm::vec3 sphere_color1 = glm::vec3(1.0f, 0.2f, 0.4f);
    glm::vec3 sphere_color2 = glm::vec3(0.5f, 0.2f, 0.3f);

    while (!glfwWindowShouldClose(window))
    {
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        float elapsed_seconds = static_cast<float>(current_seconds - previous_seconds);
        previous_seconds = current_seconds;

        update_fps_counter(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* update view matrix */
        shader->Bind();
        glm::mat4 view = camera.GetViewMatrix();
        vortex::PerspectiveFrustum frustum(fov, near, far, g_WindowWidth, g_WindowHeight);
        glm::mat4 proj = frustum.GetProjectionMatrix();
        // glm::mat4 proj = glm::perspective(fov, aspect, near, far);
        shader->SetUniformMatrix4("view", &view[0][0]);
        shader->SetUniformMatrix4("proj", &proj[0][0]);
        shader->SetUniformInt("diffuse_map", 0);
        shader->SetUniformInt("specular_map", 1);
        shader->SetUniformInt("ambient_map", 2);
        shader->SetUniformInt("emission_map", 3);

        shader->SetUniformFloat("time", current_seconds);
      
        glBindVertexArray(vao);

#if DRAW_SPHERES
        for (int i = 0; i < sphere_positions.size(); ++i)
        {
            glm::mat4 model_matrix(1.0f);
            model_matrix = glm::translate(model_matrix, sphere_positions[i]);
            shader->SetUniformMatrix4("model", &model_matrix[0][0]);
            // if (i == g_SelectionIndex)
            //     shader->SetUniformVec3("color", &sphere_color1[0]);
            // else
            //     shader->SetUniformVec3("color", &sphere_color2[0]);
            glDrawArrays(GL_TRIANGLES, 0, numVertices);
        }
#else
        glm::mat4 model(1.0f);
        shader->SetUniformMatrix4("model", &model[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, mesh.NumVertices());
#endif


        // do event handling
        if (glfwGetKey(window, GLFW_KEY_A)) {
            // cam_pos[0] -= cam_speed * elapsed_seconds;
            camera.Translate(-cam_speed * elapsed_seconds, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            camera.Translate(cam_speed * elapsed_seconds, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP)) {
            camera.Translate(0, cam_speed * elapsed_seconds, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN)) {
            camera.Translate(0, -cam_speed * elapsed_seconds, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            camera.Translate(0, 0, cam_speed * elapsed_seconds);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            camera.Translate(0, 0, -cam_speed * elapsed_seconds);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            camera.Rotate(cam_yaw_speed * elapsed_seconds, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            camera.Rotate(-cam_yaw_speed * elapsed_seconds, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_H))
        {
            camera.Home();
        }

        // window cleared each frame, so this should be placed at the bottom
        if (glfwGetKey(window, GLFW_KEY_P))
        {
            capture_screen("screen.png");
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    
    glfwTerminate();
    return 0;
}
#endif
