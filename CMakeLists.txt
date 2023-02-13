cmake_minimum_required(VERSION 3.13)
project(piv)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(piv "")

add_subdirectory(src)

target_link_libraries(piv PRIVATE sfml-graphics sfml-window sfml-system webp webpdemux)
