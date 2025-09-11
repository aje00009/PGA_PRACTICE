cmake_minimum_required(VERSION 3.31)
project(PGA_PRACTICA)

set(CMAKE_CXX_STANDARD 20)

#GLAD
file( GLOB MY_FILES *.cpp )
file( GLOB GLAD_FILES GLAD/src/*.c )

add_executable( ${PROJECT_NAME} ${MY_FILES}
                                ${GLAD_FILES} )


# First, tell CMake to find the package.
# Conan will install the packages so that CMake can find it:

#GLFW
find_package(glfw3)

find_package(opengl_system)

# Then, link your executable or library with the corresponding package targets:
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include )

target_link_libraries( ${PROJECT_NAME} glfw
                            opengl::opengl)



