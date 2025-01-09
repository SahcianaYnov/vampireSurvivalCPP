cmake_minimum_required(VERSION 3.24)
project(vampireSurvivalCPP LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_TOOLCHAIN_FILE build/conan_toolchain.cmake)

find_package(boost REQUIRED)
find_package(GTest REQUIRED)

add_executable(main_exe main.cpp)




