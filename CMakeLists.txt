cmake_minimum_required(VERSION 3.13)
project(piv)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(piv)
target_link_libraries(piv PRIVATE sfml-graphics sfml-window sfml-system webp webpdemux)

add_subdirectory(src)
