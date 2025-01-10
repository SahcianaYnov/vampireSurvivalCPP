cmake_minimum_required(VERSION 3.24)
project(vampireSurvivalCPP LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

execute_process(
    COMMAND conan install ${CMAKE_SOURCE_DIR}/conanfile.txt
    --build=missing
    -of ${CMAKE_SOURCE_DIR}/build
)

find_package(boost REQUIRED)
find_package(GTest REQUIRED) 
find_package(SFML 2.6.2 REQUIRED COMPONENTS system window graphics)

enable_testing()
add_subdirectory(src)