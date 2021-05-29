cmake_minimum_required(VERSION 3.14.0)
project(SpaceWars)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Debug)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
    set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
    set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
)
FetchContent_GetProperties(freetype)

if(NOT freetype_POPULATED)
    FetchContent_Populate(freetype)
    add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
endif()


find_package(OpenGL REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*/*.h" "src/*/*.cpp" "src/*.cpp" "src/*.h" "deps/stb_image.h")

file(GLOB_RECURSE models_data LIST_DIRECTORIES true resources/models/*)
file(GLOB_RECURSE shaders_data LIST_DIRECTORIES true src/graphic_engine/shaders/*)
file(GLOB_RECURSE textures_data LIST_DIRECTORIES true resources/textures/*)
file(GLOB_RECURSE fonts_data LIST_DIRECTORIES true resources/fonts/*)

link_directories(${CMAKE_SOURCE_DIR}/deps/)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} glfw glad glm freetype)

file(COPY ${models_data} DESTINATION resources/models/)
file(COPY ${textures_data} DESTINATION resources/textures)
file(COPY ${fonts_data} DESTINATION resources/fonts)
file(COPY ${shaders_data} DESTINATION src/graphic_engine/shaders/)
