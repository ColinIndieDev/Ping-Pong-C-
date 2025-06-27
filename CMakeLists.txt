cmake_minimum_required(VERSION 3.31)
project(Ping_Pong)

set(CMAKE_CXX_STANDARD 20)

# Detect if we're using Emscripten
if("${CMAKE_CXX_COMPILER}" MATCHES "em\\+\\+")
    set(PLATFORM "Web")
endif()

set(RAYLIB_VERSION 5.5)

# Download raylib if needed
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

add_executable(Ping_Pong main.cpp)
target_link_libraries(Ping_Pong PRIVATE raylib)

# Web-specific options
if(PLATFORM STREQUAL "Web")
    set_target_properties(Ping_Pong PROPERTIES SUFFIX ".html")

    target_link_options(Ping_Pong PRIVATE
            "-sUSE_GLFW=3"
            "-sASYNCIFY"
            "-sFULL_ES2=1"
            "-sFORCE_FILESYSTEM=1"
    )
endif()