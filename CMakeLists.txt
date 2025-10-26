cmake_minimum_required(VERSION 3.31)
project(PGA_PRACTICA)

set(CMAKE_CXX_STANDARD 20)

#GLAD
file( GLOB_RECURSE MY_FILES source/*.cpp )
file( GLOB GLAD_FILES GLAD/src/*.c )
file( GLOB IMGUI_FILES IMGUI/src/*.cpp )

add_executable( ${PROJECT_NAME} ${MY_FILES}
                                ${GLAD_FILES}
                ${IMGUI_FILES})


# First, tell CMake to find the package.
# Conan will install the packages so that CMake can find it:

#GLFW
find_package(glfw3)

find_package(glm)

find_package(opengl_system)

find_package(assimp)

# Then, link your executable or library with the corresponding package targets:
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include IMGUI/include)

target_link_libraries( ${PROJECT_NAME} glfw
                            opengl::opengl
                            glm::glm
                            assimp::assimp)


