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
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SRC_FILES src/*.c src/*.cpp src/*.h)

add_subdirectory(dependency/glfw)
include_directories(include)
link_directories(lib)
add_executable(GLPlayground ${SRC_FILES})
target_link_libraries(GLPlayground ${GLFW})