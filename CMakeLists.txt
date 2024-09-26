cmake_minimum_required(VERSION 3.13)
include(FetchContent)

project(piv)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FetchContent_Declare(box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG df7373c08a41b7a4ba6edd5d4be200675a948176
    GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(box2d)

add_executable(piv)
target_link_libraries(piv PRIVATE sfml-graphics sfml-window sfml-system webp webpdemux box2d)

add_subdirectory(src)
