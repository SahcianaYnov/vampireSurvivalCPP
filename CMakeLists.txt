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

enable_testing()

add_executable(main_exe main.cpp)

target_link_libraries(main_exe
 PRIVATE GTest::GTest GTest::Main # utilisé en interne
)