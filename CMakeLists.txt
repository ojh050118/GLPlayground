cmake_minimum_required(VERSION 3.23)
project(GLPlayground)

set(CMAKE_CXX_STANDARD 14)
set(GLFW glfw)
set(GLM glm)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit")
    # -framework OpenGL -framework CoreVideo
    add_compile_definitions(GL_SILENCE_DEPRECATION)
    set(GLFW glfw3)

endif()

# Make shader files copy to build dir
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(dependency/glfw)
include_directories(include)
link_directories(lib)
add_executable(GLPlayground src/main.cpp src/glad.c
        src/shader.cpp
        src/shader.h
        src/shader_storage.cpp
        src/shader_storage.h
        src/stb_image.h)
target_link_libraries(GLPlayground ${GLFW})