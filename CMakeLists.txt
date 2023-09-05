cmake_minimum_required(VERSION 3.13)
project(piv)
#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(piv "")

add_subdirectory(src)

target_include_directories(piv PRIVATE "/usr/local/include")
target_link_directories(piv PRIVATE "/usr/local/lib")
target_link_libraries(piv PRIVATE sfml-graphics sfml-window sfml-system webp webpdemux)
