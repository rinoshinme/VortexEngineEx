cmake_minimum_required(VERSION 3.10)
project(VortexEngine)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Release)

SET(RENDER_API OpenGL)
# SET(RENDER_API D3D11)

# set include and lib dirs
IF (WIN32)
    # setup for windows
    SET(GLFW3_INCLUDE_DIR "D:/Libs/glfw/include/")
    SET(GLFW3_LIBRARY "D:/Libs/glfw/lib/glfw3.lib")
    SET(GLEW_INCLUDE_DIR "D:/Libs/glew/include/")
    SET(GLEW_LIBRARY "D:/Libs/glew/lib/glew32.lib")  
    SET(GLM_INCLUDE_DIR "D:/Libs/glm/include/")
    SET(STBIMAGE_INCLUDE_DIR "D:/Libs/stb_image/include")
    SET(ASSIMP_INCLUDE_DIR "D:/Libs/assimp/include")
    SET(ASSIMP_LIBRARY "D:/Libs/assimp/lib/Release/assimp-vc142-mt.lib")

    SET(FMOD_INCLUDE_DIR "D:/Libs/FMOD_api/inc")
    SET(FMOD_LIBRARY "D:/Libs/FMOD_api/lib/x64/fmod_vc.lib")
ELSE (WIN32)
    # setup for mac/linux
    SET(GLFW3_INCLUDE_DIR "/usr/local/include")
    SET(GLFW3_LIBRARY "/usr/local/lib/libglfw.dylib")
    SET(GLAD_INCLUDE_DIR "/usr/local/include")
    SET(GLM_INCLUDE_DIR "/usr/local/include")
ENDIF (WIN32)

# set additional include dirs
SET(VORTEX_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/")
SET(VENDOR_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/vendor/")

FIND_PACKAGE(OpenGL REQUIRED)

FILE(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/src/*.h")
FILE(GLOB_RECURSE VENDOR_SOURCE_FILES "${PROJECT_SOURCE_DIR}/vendor/*.cpp")
FILE(GLOB_RECURSE VENDOR_C_FILES "${PROJECT_SOURCE_DIR}/vendor/*.c")

ADD_EXECUTABLE(
    vortex
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${VENDOR_SOURCE_FILES} 
    ${VENDOR_C_FILES}
)

TARGET_INCLUDE_DIRECTORIES(
    vortex PRIVATE
    ${GLFW3_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${VORTEX_INCLUDE_DIR}
    ${VENDOR_INCLUDE_DIR}
    ${FMOD_INCLUDE_DIR}
    ${STBIMAGE_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIR}
)

TARGET_LINK_LIBRARIES(
    vortex
    ${GLFW3_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${FMOD_LIBRARY}
    ${ASSIMP_LIBRARY}
)
