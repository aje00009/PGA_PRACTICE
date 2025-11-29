cmake_minimum_required(VERSION 3.31)
project(PGA_PRACTICA)

set(CMAKE_CXX_STANDARD 20)

file( GLOB_RECURSE MY_FILES source/*.cpp )
file( GLOB GLAD_FILES GLAD/src/*.c )
file( GLOB IMGUI_FILES IMGUI/src/*.cpp )
file( GLOB LODEPNG LODEPNG/src/*.cpp)

add_executable( ${PROJECT_NAME} ${MY_FILES}
                                ${GLAD_FILES}
                                ${IMGUI_FILES}
                                ${LODEPNG})

# Then, link your executable or library with the corresponding package targets:
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include IMGUI/include LODEPNG/include)

target_link_libraries( ${PROJECT_NAME} GL glfw assimp)


